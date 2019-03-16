#include "all_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cacertinmem.h"
#include "bngo_ir.h"
#include "bngo_ir_learn.h"

//红外学习接受的数据，最大存储64字节
struct learn_data {
	unsigned int count;
	unsigned int times[512];
};


//保存码库名称 在code_list.txt
int save_code_list(char *t,char *code_v,char *code_name)
{
	char code_list_path[100] = "code_list_";
	strcat(code_list_path,t);
	strcat(code_list_path,".txt");
	
	char add_code_list[512] = {0};
	strcat(add_code_list,"echo \"");
	strcat(add_code_list,code_v);
	strcat(add_code_list,":");
	strcat(add_code_list,code_name);
	strcat(add_code_list,"\"");
	strcat(add_code_list," >> ");
	strcat(add_code_list,CODEPATH);
	strcat(add_code_list,code_list_path);
	system(add_code_list);


	//printf("[%s] add_code_list:%s ### \n",__func__,add_code_list);
	return  0;
}

//删除码库名称 从code_list.txt
int delete_code_list(char *t,char *del_code_name)
{
    char file_from[100] = "/usr/data/code_list_";
    char file_to[100]  = "/usr/data/code_list_";
	
	strcat(file_from,t);
	strcat(file_from,".txt");

	strcat(file_to,t);
	strcat(file_to,".bk");
	
    //char* file_from = "/usr/data/code_list.txt";
    //char* file_to = "/usr/data/code_list.bk";

	if(access(file_from,F_OK) != 0)
		return CODE_NOT_EXIST;

	int line = 0;
    FILE *fpf, *fpt,*fd;
    char line_str[200] = {0};
	char line_pre_str[200] = {0};
	
    fpf = fopen(file_from, "r+");
    fpt = fopen(file_to, "w+");
    if(fpf == NULL || fpt == NULL)
    {
        printf("open file error!\n");
        return 2;
    }
	//获取一行
    while((fgets(line_str,200,fpf)) != NULL)
    {	
        if(strstr(line_str,del_code_name) == NULL){
            fprintf(fpt, "%s", line_str);
			strcpy(line_pre_str,line_str);
			++line;
        }
    }

	//printf("[%s] line:%d #####\n",__func__,line);
	
    fclose(fpf);
    fclose(fpt);

	rename(file_to,file_from);

	//设置语音默认遥控器
	if(line == 1){
		char *src = strstr(line_pre_str,"\n");
		if(src != NULL)
			src[0] = '\0';
		
		char str_v[10] = {0};
		char *token;	
		token = strtok(line_pre_str,":");
		if(token == NULL){
			return(INVALID_COMMAND);
		}
		strcpy(str_v,token);

		//码库名称
		char file[128] = {0};
		token = strtok( NULL,":");
		if(token == NULL)
			return(INVALID_COMMAND);
		strcpy(file,token);
		
		
		
		char asr_path[100] ={0};
		sprintf(asr_path,"%s%s%s","/usr/data/",t,".txt");

		fd = fopen(asr_path,"w+");
		if(fd == NULL){
			printf("open asr path.txt error!!!!.\n");
			return OPEN_FILE_ERR;
		}
		
		fwrite(str_v,10,1,fd);
		fwrite(file,strlen(file)+1,1,fd);
		fclose(fd); 	
		//printf("[%s] set ir success! #########\n",__func__);

	}else if(line == 0){
		//如果文件为空  删除文件
		remove(file_from);
	}
	
	return  0;
}

int led_handler_func(char  *n_path,char *token,char *ASR_COMMAND){
	int ret = 0;
	FILE *fd;
	
	char ledMode[5] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(ledMode,token);	
	if(strcmp(ledMode,"LED") != 0)
		return INVALID_COMMAND;
	//获取led命令
	char new_command[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(new_command,token);

	if(server_use_led(new_command,ASR_COMMAND) != 0)
		return(INVALID_COMMAND);

	return 0;
}

//机顶盒
int box_handler_func(char *t,char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND)
{
	int ret = 0;

	//if(strcmp(v,"1") != 0)
		//return CODE_V_ERR;
	
	//获取电视命令
	char box_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(box_command,token);
	
	///////////////////////////////////////////////////////////
	//printf("[%s] [%d] str_v:%s  command:%s ####\n",__func__,__LINE__,v,token);
		
	if(box_command[0] >= '0' && box_command[0] <= '9'){
		if(atoi(box_command) > 999)
			return INVALID_COMMAND;
		
		strcat(ASR_COMMAND,"已调到");
		strcat(ASR_COMMAND,box_command);
		strcat(ASR_COMMAND,"台");
		char num[5] = {0};
		num[0] = box_command[0]; 
		//发送第一位数字
		ret = send_Complete_code_other(name,num);
		if(ret != 0)
			return ret;
		usleep(400000);
		//发送第二位数字
		if(box_command[1] >= '0' && box_command[1] <= '9'){
			memset(num,0,5);
			num[0] = box_command[1]; 
			ret = send_Complete_code_other(name,num);
			if(ret != 0)
				return ret;	
			usleep(400000);
			//发送第三位数字
			if(box_command[2] >= '0' && box_command[2] <= '9'){
				memset(num,0,5);
				num[0] = box_command[2]; 
				ret = send_Complete_code_other(name,num);
				if(ret != 0)
					return ret;	
			}
		}
		
	}else if(strcmp(box_command,"ch-") == 0){
		strcpy(ASR_COMMAND,"已调到上一个频道");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"ch+") == 0){
		strcpy(ASR_COMMAND,"已调到下一个频道");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"vol-") == 0){
		int i;
		if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"电视音量已减小");
			i = 10;
			while(i--){
				ret = send_Complete_code_other(name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}

		} else if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"机顶盒音量已减小");
			i = 3;
			while(i--){
				ret = send_Complete_code_other(name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}

		} else {
			strcpy(ASR_COMMAND,"音量已减小");
			ret = send_Complete_code_other(name,box_command);
		}

	}else if(strcmp(box_command,"vol+") == 0){
		int i;
		if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"电视音量已增大");
			i = 10;
			while(i--){
				ret = send_Complete_code_other(name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}

		} else if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"机顶盒音量已增大");
			i = 3;
			while(i--){
				ret = send_Complete_code_other(name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}

		} else {
			strcpy(ASR_COMMAND,"音量已增大");
			ret = send_Complete_code_other(name,box_command);
		}
		
	}else if(strcmp(box_command,"up") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"down") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"left") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"right") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"exit") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"menu") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"mute") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"ok") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"power") == 0){
		if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"已打开机顶盒");
		} else if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"已打开电视");
		} else if(strcmp(t,"5") == 0){
			strcpy(ASR_COMMAND,"已打开投影仪");
		} else if(strcmp(t,"8") == 0){
			strcpy(ASR_COMMAND,"已打开智能灯泡");
		} else if(strcmp(t,"13") == 0){
			strcpy(ASR_COMMAND,"已打开音响");
		} else if(strcmp(t,"12") == 0){
			strcpy(ASR_COMMAND,"已打开扫地机");
		} else if(strcmp(t,"15") == 0){
			strcpy(ASR_COMMAND,"已打开空气净化器");
		} else {
			return INVALID_COMMAND;
		}
		
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"poweroff") == 0){

		if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"已关闭机顶盒");
		} else if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"已关闭电视");
		} else if(strcmp(t,"5") == 0){
			strcpy(ASR_COMMAND,"已关闭投影仪");
		} else if(strcmp(t,"8") == 0){
			strcpy(ASR_COMMAND,"已关闭智能灯泡");
		} else if(strcmp(t,"13") == 0){
			strcpy(ASR_COMMAND,"已关闭音响");
		} else if(strcmp(t,"12") == 0){
			strcpy(ASR_COMMAND,"已关闭扫地机");
		} else if(strcmp(t,"15") == 0){
			strcpy(ASR_COMMAND,"已关闭空气净化器");
		} else {
			return INVALID_COMMAND;
		}
		
		ret = send_Complete_code_other(name,box_command);
		if(ret == 26)
			ret = send_Complete_code_other(name,"power");
	}else if(strcmp(box_command,"signal") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"back") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");	
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"boot") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,box_command);
	}else if(strcmp(box_command,"noch") == 0){
		return NOCH_ERR;
	} else {
			return INVALID_COMMAND;
	}
	
	return ret;
}

//电视
int tv_handler_func(char *v,char *name,char  *n_path,char *token)
{

	int ret = 0;

	if(strcmp(v,"1") != 0)
		return CODE_V_ERR;
	
	//获取电视命令
	char tv_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(tv_command,token);
	///////////////////////////////////////////////////////////
	//printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);

	if(strcmp(tv_command,"0") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"1") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"2") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"3") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"4") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"5") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"6") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"7") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"8") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"9") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"ch-") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"ch+") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"vol-") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"vol+") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"up") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"down") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"left") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"right") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"exit") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"menu") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"mute") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"ok") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"power") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"signal") == 0){
		ret = send_Complete_code_other(name,tv_command);
	}else if(strcmp(tv_command,"back") == 0){
		ret = send_Complete_code_other(name,tv_command);
	} else {
			return INVALID_COMMAND;
	}
	if(ret != 0)
		return ret;

	return 0;
}

//网络盒子
int tvbox_handler_func(char *v,char *name,char  *n_path,char *token)
{
	int ret = 0;

	if(strcmp(v,"1") != 0)
		return CODE_V_ERR;
	
	//获取电视盒子命令
	char tvbox_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(tvbox_command,token);
	
	///////////////////////////////////////////////////////////
	//printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);

	
	if(strcmp(tvbox_command,"0") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"1") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"2") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"3") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"4") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"5") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"6") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"7") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"8") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"9") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"ch-") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"ch+") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"vol-") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"vol+") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"up") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"down") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"left") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"right") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"exit") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"menu") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"mute") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"ok") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"power") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"signal") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"back") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	}else if(strcmp(tvbox_command,"boot") == 0){
		ret = send_Complete_code_other(name,tvbox_command);
	} else {
			return INVALID_COMMAND;
	}
	if(ret != 0)
		return ret;

	return 0;
}


//风扇
int fan_handler_func(char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND)
{

	int ret = 0;

	//if(strcmp(v,"1") != 0)
		//return CODE_V_ERR;
	
	//获取风扇命令
	char fan_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(fan_command,token);
	///////////////////////////////////////////////////////////
	//printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);


	//模式
	if(strcmp(fan_command,"mode") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,fan_command);
	//摇头
	}else if(strcmp(fan_command,"oscillation") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,fan_command);
	//开关
	}else if(strcmp(fan_command,"power") == 0){
		strcpy(ASR_COMMAND,"已打开风扇");
		ret = send_Complete_code_other(name,fan_command);
	//开关
	}else if(strcmp(fan_command,"poweroff") == 0){
		strcpy(ASR_COMMAND,"已关闭风扇");
		ret = send_Complete_code_other(name,fan_command);
		if(ret == 26)
			ret = send_Complete_code_other(name,"power");
	//定时
	}else if(strcmp(fan_command,"timer") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = send_Complete_code_other(name,fan_command);
	//风速
	}else if(strcmp(fan_command,"fanspeed") == 0){
		strcpy(ASR_COMMAND,"已调节风速");
		ret = send_Complete_code_other(name,fan_command);
	} else {
			return INVALID_COMMAND;
	}


	return ret;
}


//空调
int air_handler_func(char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND)
{
	code_info_t code_info_msg;
	memset(&code_info_msg,0,sizeof(code_info_t));
	
	//printf("[%s] str_v:%s n_path:%s ####\n",__func__,v,n_path);
	int i;
	FILE *fd;

	///////////获取历史命令/////////////////////////////////////
	fd = fopen(n_path,"r+");
	if(fd == NULL){
		printf("%s: open command.record error!!!!.\n",__func__);
		return(OPEN_FILE_ERR);
	}
	fread(&code_info_msg,sizeof(code_info_t),1,fd);
	fclose(fd);
	
	//打印历史命令
	//printf("######  code_info_msg.key:%d  code_info_msg.old_command:%s  #####\n",code_info_msg.key,code_info_msg.old_command);
	
	
	if(strcmp(v,"3") == 0 || strcmp(v,"4") == 0){
		//power: open或者close
		char power[10] = {0};
		token = strtok( NULL,"=");
		if(token == NULL)
			return(INVALID_COMMAND);
		strcpy(power,token);
		if(strcmp(power,"OPEN") == 0){
			//#############解析空调命令#################
				//服务器发过来的命令: SERVER-7-OPEN-a-NO-NO-NO
				char model[10] = {0};
				char wind[10] = {0};
				char temperature[10] = {0};
				char sweep[10] = {0};
				
				//*.record 中的历史命令
				char old_model[10] = {0};
				char old_wind[10] = {0};
				char old_temperature[10] = {0};
				char old_lrsweep[10] = {0};
				char old_upsweep[10] = {0};
				char old_sleep[10] = {0};
				//新命令
				char new_command[50] = {0};
				//获取历史命令，历史命令存在
				char old_command[50] = {0};
				//历史命令分隔
				char *old_token = NULL;	

				i = 0;
	  		 	while( token != NULL )
	  		 	{
	  		 		token = strtok( NULL,"=");
	   				if(i ==0)
						strcpy(model,token);
					else if(i ==1)
						strcpy(wind,token);
					else if(i ==2)
						strcpy(temperature,token);
					else if(i ==3)
						strcpy(sweep,token);
		  			++i;
	  		 	}
				if(i != 5)
					return(INVALID_COMMAND);
				//服务器数据
				//printf("model:%s wind:%s temperature:%s sweep:%s \n",model,wind,temperature,sweep);
				
				//获取第一级: 模式
				if(strcmp(model,"NO") != 0){
					//获取上一次数据内容
					if (code_info_msg.old_command[0] == '\0'){
						if(strcmp(model,"a") == 0){
							strcpy(ASR_COMMAND,"已设到自动模式");
							//if(send_Complete_code(name,"a_s0__u0_l0_p0") != 0)
							return(send_Complete_code(name,"a_s0__u0_l0_p0"));	
						} else if(strcmp(model,"d") == 0){
							strcpy(ASR_COMMAND,"已设到抽湿模式");
							//if(send_Complete_code(name,"d_s1__u0_l0_p0") != 0)
							return(send_Complete_code(name,"d_s1__u0_l0_p0"));
						} else if(strcmp(model,"w") == 0){
							strcpy(ASR_COMMAND,"已设到送风模式");
							//if(send_Complete_code(name,"w_s1_26_u0_l0_p0") != 0)
							return(send_Complete_code(name,"w_s1_26_u0_l0_p0"));
						} else if(strcmp(model,"h") == 0){
							strcpy(ASR_COMMAND,"已设到制热模式");
							//if(send_Complete_code(name,"h_s0_26_u0_l0_p0") != 0)
							return(send_Complete_code(name,"h_s0_26_u0_l0_p0"));
						} else if(strcmp(model,"r") == 0){
							strcpy(ASR_COMMAND,"已设到制冷模式");
							//if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
							return(send_Complete_code(name,"r_s0_26_u0_l0_p0"));
						} else {
								return(INVALID_COMMAND);
						}
					
					} else {
						//获取*.record 里面上一次命令 old_command
						//fd = fopen(n_path,"r+");
						//if(fd == NULL){
							//printf("%s: open command.record error!!!!.\n",__func__);
							//return(OPEN_FILE_ERR);
						//}
						//fread(old_command,50,1,fd);
						//fclose(fd);
						//打印历史命令
						//printf("old_command:%s  #####\n",code_info_msg.old_command);
						

						//判断是否自动模式
						if(strcmp(model,"a") == 0){
							strcpy(ASR_COMMAND,"已设到自动模式");
							if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){
								code_info_msg.old_command[0] = 'a';
								//if(send_Complete_code(name,old_command) != 0)
								return(send_Complete_code(name,code_info_msg.old_command));
							} else if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL){
								old_token = strtok(code_info_msg.old_command,"_");
								if(old_token == NULL)
									return(INVALID_OLD_COMMAND);
									
								strcpy(old_model,old_token);
								
								i = 0;
								while(old_token != NULL){
									old_token = strtok( NULL,"_");
									if(i ==0)
										strcpy(old_wind,old_token);
									else if(i ==1)
										strcpy(old_temperature,old_token);
									else if(i ==2)
										strcpy(old_upsweep,old_token);
									else if(i ==3)
										strcpy(old_lrsweep,old_token);
									else if(i ==4)
										strcpy(old_sleep,old_token);
									++i;
								}
								
								if(i != 6)
									return(INVALID_OLD_COMMAND);
								
								sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s","a","_",old_wind,"_","_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
								//printf("new_command:%s ########\n",new_command);
								//if(send_Complete_code(name,new_command) != 0)
								return(send_Complete_code(name,new_command));
							}else {
								return(INVALID_OLD_COMMAND);
							}
											
						} else if(strcmp(model,"d") == 0){

							strcpy(ASR_COMMAND,"已设到抽湿模式");
							if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){
									//抽湿模式：风量只有s1
									//获取上下扫风
									if(strstr(code_info_msg.old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(code_info_msg.old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(code_info_msg.old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(code_info_msg.old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}		

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s","d","_","s1","_","_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									//printf("new_command:%s ########\n",new_command);
									//if(send_Complete_code(name,"d_s1__u0_l0_p0") != 0)
									return(send_Complete_code(name,"d_s1__u0_l0_p0"));
							} else if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL){
								old_token = strtok(code_info_msg.old_command,"_");
								if(old_token == NULL)
									return(INVALID_OLD_COMMAND);
									
								strcpy(old_model,old_token);
								
								i = 0;
								while(old_token != NULL){
									old_token = strtok( NULL,"_");
									if(i ==0)
										strcpy(old_wind,old_token);
									else if(i ==1)
										strcpy(old_temperature,old_token);
									else if(i ==2)
										strcpy(old_upsweep,old_token);
									else if(i ==3)
										strcpy(old_lrsweep,old_token);
									else if(i ==4)
										strcpy(old_sleep,old_token);
									++i;
								}
								
								if(i != 6)
									return(INVALID_OLD_COMMAND);
								
								sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s","d","_","s1","_","_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
								//printf("new_command:%s ########\n",new_command);
								//if(send_Complete_code(name,new_command) != 0)
								return(send_Complete_code(name,new_command));
							}	else {
								return(INVALID_OLD_COMMAND);
							}						
						} else if(strcmp(model,"w") == 0){
							strcpy(ASR_COMMAND,"已设到送风模式");
							if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL ){
								code_info_msg.old_command[0] = 'w';
								char *strWind = strstr(code_info_msg.old_command,"s");
								if(strWind[1] == '0')
									strWind[1] = '1';
								//if(send_Complete_code(name,old_command) != 0)
								return(send_Complete_code(name,code_info_msg.old_command));
							} else if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){

									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//获取风量
									if(strstr(code_info_msg.old_command,"s0") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(code_info_msg.old_command,"s1") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(code_info_msg.old_command,"s2") != NULL){
										strcpy(old_wind,"s2");
									}else if(strstr(code_info_msg.old_command,"s3") != NULL){
										strcpy(old_wind,"s3");
									}

									//获取上下扫风
									if(strstr(code_info_msg.old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(code_info_msg.old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(code_info_msg.old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(code_info_msg.old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","w","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									//printf("new_command:%s ########\n",new_command);
									//if(send_Complete_code(name,new_command) != 0)
									return(send_Complete_code(name,new_command));
												
							}	else {
									return(INVALID_OLD_COMMAND);
							}						
						} else if(strcmp(model,"h") == 0){

							strcpy(ASR_COMMAND,"已设到制热模式");
							if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL ){
								code_info_msg.old_command[0] = 'h';
								//if(send_Complete_code(name,old_command) != 0)
								return(send_Complete_code(name,code_info_msg.old_command));
							} else if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){
								
									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//获取风量
									if(strstr(code_info_msg.old_command,"s0") != NULL){
										strcpy(old_wind,"s0");
									}else if(strstr(code_info_msg.old_command,"s1") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(code_info_msg.old_command,"s2") != NULL){
										strcpy(old_wind,"s2");
									}else if(strstr(code_info_msg.old_command,"s3") != NULL){
										strcpy(old_wind,"s3");
									}

									//获取上下扫风
									if(strstr(code_info_msg.old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(code_info_msg.old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(code_info_msg.old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(code_info_msg.old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","h","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									//printf("new_command:%s ########\n",new_command);
									//if(send_Complete_code(name,new_command) != 0)
									return(send_Complete_code(name,new_command));
								
								
							}	else {
								return(INVALID_OLD_COMMAND);
							}							
						} else if(strcmp(model,"r") == 0){
							strcpy(ASR_COMMAND,"已设到制冷模式");
							if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL ){
								code_info_msg.old_command[0] = 'r';
								//if(send_Complete_code(name,old_command) != 0)
								return(send_Complete_code(name,code_info_msg.old_command));
							} else if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){
									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//获取风量
									if(strstr(code_info_msg.old_command,"a") != NULL){
										if(strstr(code_info_msg.old_command,"s0") != NULL){
											strcpy(old_wind,"s0");
										}else if(strstr(code_info_msg.old_command,"s1") != NULL){
											strcpy(old_wind,"s1");
										}else if(strstr(code_info_msg.old_command,"s2") != NULL){
											strcpy(old_wind,"s2");
										}else if(strstr(code_info_msg.old_command,"s3") != NULL){
											strcpy(old_wind,"s3");
										}
									}else if(strstr(code_info_msg.old_command,"d") != NULL){
										strcpy(old_wind,"s1");
									}

									//获取上下扫风
									if(strstr(code_info_msg.old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(code_info_msg.old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(code_info_msg.old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(code_info_msg.old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									//printf("new_command:%s ########\n",new_command);
									//if(send_Complete_code(name,new_command) != 0)
										return(send_Complete_code(name,new_command));
								} else {
									return(INVALID_OLD_COMMAND);								
								}															
						}else{
							return(INVALID_COMMAND);
						}

					}
				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") != 0){
					  	//获取第二级: 风量
				  		//历史命令不存在 *.record 发送初始值
						if (code_info_msg.old_command[0] == '\0'){
					  		  if(strstr(wind,"s0") != NULL){
							  	   strcpy(ASR_COMMAND,"已设到自动风速");
								   sprintf(new_command,"%s","r_s0_26_u0_l0_p0");
							  } else if(strstr(wind,"s1") != NULL){
							  	   strcpy(ASR_COMMAND,"已设到低速风");
								   sprintf(new_command,"%s","r_s1_26_u0_l0_p0");
							  } else if(strstr(wind,"s2") != NULL){
							 	   strcpy(ASR_COMMAND,"已设到中速风");
								   sprintf(new_command,"%s","r_s2_26_u0_l0_p0");
							  } else if(strstr(wind,"s3") != NULL){
								   strcpy(ASR_COMMAND,"已设到高速风");
								   sprintf(new_command,"%s","r_s3_26_u0_l0_p0");
							  } else if(strstr(wind,"s4") != NULL || strstr(wind,"s5") != NULL){
								   strcpy(ASR_COMMAND,"已设到自动风速");
								   sprintf(new_command,"%s","r_s0_26_u0_l0_p0");
							  } else {
								  return(INVALID_COMMAND);
							  }
							 
							  //printf("new_command:%s ########\n",new_command);
							  //if(send_Complete_code(name,new_command) != 0)
							  return(send_Complete_code(name,new_command));
						} else {
								//获取*.record 里面上一次命令 old_command
								/*
								fd = fopen(n_path,"r+");
								if(fd == NULL){
										printf("%s: open command.record error!!!!.\n",__func__);
										return(OPEN_FILE_ERR);
								}
								fread(old_command,50,1,fd);
								fclose(fd);
								//打印历史命令
								printf("old_command:%s	#####\n",old_command);
								*/
								
								//获取风量
								char *strWind2 = strstr(code_info_msg.old_command,"s");
								if(strWind2 == NULL)
									return(INVALID_OLD_COMMAND);
								
								if(wind[1] == '0'){
									strcpy(ASR_COMMAND,"已设到自动风速");
								} else if(wind[1] == '1'){
									strcpy(ASR_COMMAND,"已设到低速风");
								} else if(wind[1] == '2'){
									strcpy(ASR_COMMAND,"已设到中速风");
								} else if(wind[1] == '3'){
									strcpy(ASR_COMMAND,"已设到高速风");
								} else if(wind[1] == '4'){
									if(strWind2[1] == '3'){
										strcpy(ASR_COMMAND,"已设到中速风");
										wind[1] = '2';
									} else if(strWind2[1] == '2'){
										strcpy(ASR_COMMAND,"已设到低速风");
										wind[1] = '1';
									} else if(strWind2[1] == '1'){
										strcpy(ASR_COMMAND,"已是最低风速");
										wind[1] = '1';
										return 0;
									} else if(strWind2[1] == '0'){
										strcpy(ASR_COMMAND,"已设到低速风");
										wind[1] = '1';
									} else{
										return(INVALID_OLD_COMMAND);
									}
									
								} else if(wind[1] == '5'){

									if(strWind2[1] == '0'){
										strcpy(ASR_COMMAND,"已设到中速风");
										wind[1] = '2';
									} else if(strWind2[1] == '1'){
										strcpy(ASR_COMMAND,"已设到中速风");
										wind[1] = '2';
									} else if(strWind2[1] == '2'){
										strcpy(ASR_COMMAND,"已设到高速风");
										wind[1] = '3';
									} else if(strWind2[1] == '3'){
										strcpy(ASR_COMMAND,"已是最高风速");
										wind[1] = '3';
										return 0;
									} else{
										return(INVALID_OLD_COMMAND);
									}
									
								} else{
									return(INVALID_OLD_COMMAND);
								}
								
								if(strstr(code_info_msg.old_command,"d") != NULL){
										strWind2[1] = '1';
								} else if(strstr(code_info_msg.old_command,"w") != NULL){
										if(wind[1] == '0')
											strWind2[1] = '1';
										else 
											strWind2[1] = wind[1];
								} else {
									strWind2[1] = wind[1];
								}

								//printf("new_command:%s ########\n",code_info_msg.old_command);
								//if(send_Complete_code(name,code_info_msg.old_command) != 0)
								return(send_Complete_code(name,code_info_msg.old_command));

								
						}

				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") != 0){
					  //获取第三级: 温度
				  if(temperature[0] < '0' || temperature[0] > '9')
				  		return(INVALID_COMMAND);
				  int temp = atoi(temperature);
				  if(temp > 15 && temp < 31){
				  	  strcpy(ASR_COMMAND,"已设到");
					  strcat(ASR_COMMAND,temperature);
					  strcat(ASR_COMMAND,"度");
					  //如果历史命令不存在
					  if (code_info_msg.old_command[0] == '\0'){
							  sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_",temperature,"_","u0","_","l0","_","p0");
							  //printf("new_command:%s ########\n",new_command);
							  
							  //if(send_Complete_code(name,new_command) != 0)
							  return(send_Complete_code(name,new_command));
					  } else {
							  //获取*.record 里面上一次命令 code_info_msg.old_command
							  /*
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  return(OPEN_FILE_ERR);
							  }
							  fread(code_info_msg.old_command,50,1,fd);
							  fclose(fd);
							  //打印历史命令
							  */
							  //printf("code_info_msg.old_command:%s  #####\n",code_info_msg.old_command);
					  
							 
					  		  //获取历史命令中的模式
					  		  if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL ){
									code_info_msg.old_command[5] = temperature[0];
									code_info_msg.old_command[6] = temperature[1];
									
									//printf("new_command:%s ########\n",code_info_msg.old_command);
									//if(send_Complete_code(name,code_info_msg.old_command) != 0)
									return(send_Complete_code(name,code_info_msg.old_command));		
									
							  } else if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){

							 		strcpy(old_sleep,"p0");
							  		strcpy(old_temperature,temperature);
							  		//获取风量
									if(strstr(code_info_msg.old_command,"a") != NULL){
										if(strstr(code_info_msg.old_command,"s0") != NULL){
											strcpy(old_wind,"s0");
										}else if(strstr(code_info_msg.old_command,"s1") != NULL){
											strcpy(old_wind,"s1");
										}else if(strstr(code_info_msg.old_command,"s2") != NULL){
											strcpy(old_wind,"s2");
										}else if(strstr(code_info_msg.old_command,"s3") != NULL){
											strcpy(old_wind,"s3");
										}
									}else if(strstr(code_info_msg.old_command,"d") != NULL){
										strcpy(old_wind,"s1");
									}

							  
							  		//获取上下扫风
							  		if(strstr(code_info_msg.old_command,"u0") != NULL){
								  		strcpy(old_upsweep,"u0");
							  		}else if(strstr(code_info_msg.old_command,"u1") != NULL){
								  		strcpy(old_upsweep,"u1");
							  		}
							  
							  		//获取左右扫风
							  		if(strstr(code_info_msg.old_command,"l0") != NULL){
								 		 strcpy(old_lrsweep,"l0");
							  		}else if(strstr(code_info_msg.old_command,"l1") != NULL){
								  		strcpy(old_lrsweep,"l1");
							 		}
							  
							 		 sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
							 		 //printf("new_command:%s ########\n",new_command);
							 		 //if(send_Complete_code(name,new_command) != 0)
								  	return(send_Complete_code(name,new_command));
							  }
							  
					  }
					}else if(temp > 50 && temp < 60) { 
							//温度升高
 							//如果历史命令不存在
							  if (code_info_msg.old_command[0] == '\0'){
									  strcpy(ASR_COMMAND,"已设到26度");
									  sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_","26","_","u0","_","l0","_","p0");
									  //printf("new_command:%s ########\n",new_command);
									  
									  //if(send_Complete_code(name,new_command) != 0)
									  return(send_Complete_code(name,new_command));
							  } else {
							  		  /*
									  //获取*.record 里面上一次命令 code_info_msg.old_command
									  fd = fopen(n_path,"r+");
									  if(fd == NULL){
											  printf("%s: open command.record error!!!!.\n",__func__);
											  return(OPEN_FILE_ERR);
									  }
									  fread(code_info_msg.old_command,50,1,fd);
									  fclose(fd);
									  */
									  //打印历史命令
									  //printf("code_info_msg.old_command:%s  #####\n",code_info_msg.old_command);
							  
									 
							  		  //获取历史命令中的模式
							  		  if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL ){
											int t = atoi(code_info_msg.old_command + 5);
											if((t + (temp%10))<31){
												//t += 1;
												memset(temperature,0,10);
												sprintf(temperature,"%d",(t + (temp%10)));
												code_info_msg.old_command[5] = temperature[0];
												code_info_msg.old_command[6] = temperature[1];	

											} else if((t + (temp%10)) > 30){
												memset(temperature,0,10);
												sprintf(temperature,"%d",30);
												code_info_msg.old_command[5] = temperature[0];
												code_info_msg.old_command[6] = temperature[1];											
											} else {
												return(INVALID_COMMAND);
											}
										  	  strcpy(ASR_COMMAND,"已设到");
											  strcat(ASR_COMMAND,temperature);
											  strcat(ASR_COMMAND,"度");											
											//printf("new_command:%s ########\n",code_info_msg.old_command);
											//if(send_Complete_code(name,code_info_msg.old_command) != 0)
											return(send_Complete_code(name,code_info_msg.old_command));		
											
									  } else if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){

									 		strcpy(old_sleep,"p0");
									  		strcpy(old_temperature,"26");
											strcpy(ASR_COMMAND,"已设到26度");
									  		//获取风量
											if(strstr(code_info_msg.old_command,"a") != NULL){
												if(strstr(code_info_msg.old_command,"s0") != NULL){
													strcpy(old_wind,"s0");
												}else if(strstr(code_info_msg.old_command,"s1") != NULL){
													strcpy(old_wind,"s1");
												}else if(strstr(code_info_msg.old_command,"s2") != NULL){
													strcpy(old_wind,"s2");
												}else if(strstr(code_info_msg.old_command,"s3") != NULL){
													strcpy(old_wind,"s3");
												}
											}else if(strstr(code_info_msg.old_command,"d") != NULL){
												strcpy(old_wind,"s1");
											}

									  
									  		//获取上下扫风
									  		if(strstr(code_info_msg.old_command,"u0") != NULL){
										  		strcpy(old_upsweep,"u0");
									  		}else if(strstr(code_info_msg.old_command,"u1") != NULL){
										  		strcpy(old_upsweep,"u1");
									  		}
									  
									  		//获取左右扫风
									  		if(strstr(code_info_msg.old_command,"l0") != NULL){
										 		 strcpy(old_lrsweep,"l0");
									  		}else if(strstr(code_info_msg.old_command,"l1") != NULL){
										  		strcpy(old_lrsweep,"l1");
									 		}
									  
									 		 sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									 		 //printf("new_command:%s ########\n",new_command);
									 		 //if(send_Complete_code(name,new_command) != 0)
										  	 return(send_Complete_code(name,new_command));
									  }
									  
							  }							
					}else if(temp > 40 && temp < 50) { 	
							//温度降低
 							//如果历史命令不存在
							  if (code_info_msg.old_command[0] == '\0'){
									  strcpy(ASR_COMMAND,"已设到26度");							  	
									  sprintf(new_command,"%s","r_s0_26_u0_l0_p0");
									  //printf("new_command:%s ########\n",new_command);
									  
									  //if(send_Complete_code(name,new_command) != 0)
								      return(send_Complete_code(name,new_command));
							  } else {
							  		  /*
									  //获取*.record 里面上一次命令 code_info_msg.old_command
									  fd = fopen(n_path,"r+");
									  if(fd == NULL){
											  printf("%s: open command.record error!!!!.\n",__func__);
											  return(OPEN_FILE_ERR);
									  }
									  fread(code_info_msg.old_command,50,1,fd);
									  fclose(fd);
									  */
									  //打印历史命令
									 // printf("code_info_msg.old_command:%s  #####\n",code_info_msg.old_command);
							  
									 
							  		  //获取历史命令中的模式
							  		  if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL ){
											int t = atoi(code_info_msg.old_command + 5);
											if((t - (temp%10))>15){
												//t -= 1;
												memset(temperature,0,10);
												sprintf(temperature,"%d",(t - (temp%10)));
												code_info_msg.old_command[5] = temperature[0];
												code_info_msg.old_command[6] = temperature[1];
											} else if((t - (temp%10)) < 16){
												memset(temperature,0,10);
												sprintf(temperature,"%d",16);
												code_info_msg.old_command[5] = temperature[0];
												code_info_msg.old_command[6] = temperature[1];
											} else {
												return(INVALID_COMMAND);

											}
										  	  strcpy(ASR_COMMAND,"已设到");
											  strcat(ASR_COMMAND,temperature);
											  strcat(ASR_COMMAND,"度");												
											//printf("new_command:%s ########\n",code_info_msg.old_command);
											//if(send_Complete_code(name,code_info_msg.old_command) != 0)
											return(send_Complete_code(name,code_info_msg.old_command));		
											
									  } else if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL){

									 		strcpy(old_sleep,"p0");
									  		strcpy(old_temperature,"26");
									  		//获取风量
											if(strstr(code_info_msg.old_command,"a") != NULL){
												if(strstr(code_info_msg.old_command,"s0") != NULL){
													strcpy(old_wind,"s0");
												}else if(strstr(code_info_msg.old_command,"s1") != NULL){
													strcpy(old_wind,"s1");
												}else if(strstr(code_info_msg.old_command,"s2") != NULL){
													strcpy(old_wind,"s2");
												}else if(strstr(code_info_msg.old_command,"s3") != NULL){
													strcpy(old_wind,"s3");
												}
											}else if(strstr(code_info_msg.old_command,"d") != NULL){
												strcpy(old_wind,"s1");
											}

									  
									  		//获取上下扫风
									  		if(strstr(code_info_msg.old_command,"u0") != NULL){
										  		strcpy(old_upsweep,"u0");
									  		}else if(strstr(code_info_msg.old_command,"u1") != NULL){
										  		strcpy(old_upsweep,"u1");
									  		}
									  
									  		//获取左右扫风
									  		if(strstr(code_info_msg.old_command,"l0") != NULL){
										 		 strcpy(old_lrsweep,"l0");
									  		}else if(strstr(code_info_msg.old_command,"l1") != NULL){
										  		strcpy(old_lrsweep,"l1");
									 		}
									   		 strcpy(ASR_COMMAND,"已设到26度");	
									 		 sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									 		 //printf("new_command:%s ########\n",new_command);
									 		 //if(send_Complete_code(name,new_command) != 0)
										  	 return(send_Complete_code(name,new_command));
									  }
									  
							  }							
				  	}else {
						return(INVALID_COMMAND);
					}


				}

				  //获取第四级: 扫风
				  else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") != 0){
					  //如果历史命令不存在
					  if(code_info_msg.key == 0){
							//特殊按键
							if(strstr(sweep,"u0") != NULL){
							  	   strcpy(ASR_COMMAND,"已关闭扫风");	
								   sprintf(new_command,"%s","*_*_*_u0_*_*");

							  } else if(strstr(sweep,"u1") != NULL){
							 	 strcpy(ASR_COMMAND,"已开启扫风");	
								   sprintf(new_command,"%s","*_*_*_u1_*_*");
							  } else {
								  return(INVALID_COMMAND);
							  }	

							  //printf("new_command:%s ########\n",new_command);
							  //if(send_Complete_code(name,new_command) != 0)
							  return(send_Complete_code(name,new_command));
					  } 

					  
					  if (code_info_msg.old_command[0] == '\0'){
					  		  if(strstr(sweep,"u0") != NULL){
							  	   strcpy(ASR_COMMAND,"已关闭扫风");	
								   sprintf(new_command,"%s","r_s0_26_u0_l0_p0");

							  } else if(strstr(sweep,"u1") != NULL){
							 	 	strcpy(ASR_COMMAND,"已开启扫风");	
								    sprintf(new_command,"%s","r_s0_26_u1_l1_p0");
							  } else {
								  return(INVALID_COMMAND);
							  }
							 
							  //printf("new_command:%s ########\n",new_command);
							  //if(send_Complete_code(name,new_command) != 0)
							  return(send_Complete_code(name,new_command));
					  } else{
					  		  /*
							  //获取*.record 里面上一次命令 code_info_msg.old_command
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  return(OPEN_FILE_ERR);
							  }
							  fread(code_info_msg.old_command,50,1,fd);
							  fclose(fd);
							  */
							  
							  //打印历史命令
							  //printf("code_info_msg.old_command:%s  #####\n",code_info_msg.old_command);	

							  if(strstr(code_info_msg.old_command,"w") != NULL || strstr(code_info_msg.old_command,"h") != NULL || strstr(code_info_msg.old_command,"r") != NULL ){
							  			if(strstr(sweep,"u0") != NULL){
											strcpy(ASR_COMMAND,"已关闭扫风");	
												code_info_msg.old_command[9] = '0';
												code_info_msg.old_command[12] = '0';
										} else if(strstr(sweep,"u1") != NULL){
										strcpy(ASR_COMMAND,"已开启扫风");	
											code_info_msg.old_command[9] = '1';
											code_info_msg.old_command[12] = '1';

										} else {
											return(INVALID_COMMAND);
										}

							 			 //printf("new_command:%s ########\n",code_info_msg.old_command);
							 			 //if(send_Complete_code(name,code_info_msg.old_command) != 0)
								 		 return(send_Complete_code(name,code_info_msg.old_command));										

							  } else if(strstr(code_info_msg.old_command,"a") != NULL || strstr(code_info_msg.old_command,"d") != NULL ){
								 		 if(strstr(sweep,"u0") != NULL){
										 	strcpy(ASR_COMMAND,"已关闭扫风");	
										  	code_info_msg.old_command[7] = '0';
										 	 code_info_msg.old_command[10] = '0';
								 	 	} else if(strstr(sweep,"u1") != NULL){
								 	 	strcpy(ASR_COMMAND,"已开启扫风");	
									  		code_info_msg.old_command[7] = '1';
									  		code_info_msg.old_command[10] = '1';
								  
								 		 } else {
									  		return(INVALID_COMMAND);
								  		}
								  
								   		//printf("new_command:%s ########\n",code_info_msg.old_command);
								   		//if(send_Complete_code(name,code_info_msg.old_command) != 0)
									   	return(send_Complete_code(name,code_info_msg.old_command)); 

							  } else {
										return(INVALID_OLD_COMMAND);
							  }

					  }					

				} 
				  //没有设置参数
				  else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") == 0){
					  //如果历史命令不存在
					  strcpy(ASR_COMMAND,"已打开空调");	
					  if (code_info_msg.old_command[0] == '\0'){
							  //printf("new_command:%s ########\n","r_s0_26_u0_l0_p0");
							  //if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								  return(send_Complete_code(name,"r_s0_26_u0_l0_p0"));
					  } else{
							/*
					  		//获取*.record 里面上一次命令 code_info_msg.old_command
					 		fd = fopen(n_path,"r+");
					 		if(fd == NULL){
							  	printf("%s: open command.record error!!!!.\n",__func__);
							 	return(OPEN_FILE_ERR);
					  		}
					  		fread(code_info_msg.old_command,50,1,fd);
					  		fclose(fd);
					  		*/
					  		
					  		//打印历史命令

							if(code_info_msg.old_command[0] != 'a' && code_info_msg.old_command[0] != 'd' && code_info_msg.old_command[0] != 'w' && code_info_msg.old_command[0] != 'r' && code_info_msg.old_command[0] != 'h'){
								 //printf("new_command:%s ########\n","r_s0_26_u0_l0_p0");
							 	 //if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								  	return(send_Complete_code(name,"r_s0_26_u0_l0_p0"));
							} else {
								//printf("new_command:%s ########\n",code_info_msg.old_command);
								//if(send_Complete_code(name,code_info_msg.old_command) != 0)
									return(send_Complete_code(name,code_info_msg.old_command));
							}


					  }


				} else {
					//其他命令无效
					return(INVALID_COMMAND);
				}
			
		} else if(strcmp(power,"CLOSE") == 0){
			strcpy(ASR_COMMAND,"已关闭空调");	
			//if(send_Complete_code(name,"off") != 0)
					return(send_Complete_code(name,"off"));
		} else {
			//其他命令无效
					return(INVALID_COMMAND);	
		}
		
		/***********************************************
				    上面是码库v3版本的解析	
		************************************************/

	} else if(strcmp(v,"1") == 0){

		//power: open或者close
		char power[10] = {0};
		token = strtok( NULL,"=");
		if(token == NULL)
			return(INVALID_COMMAND);
		strcpy(power,token);
		if(strcmp(power,"OPEN") == 0){
				//#############解析空调命令#################
				//服务器发过来的命令: SERVER-7-OPEN-a-NO-NO-NO
				char model[10] = {0};
				char wind[10] = {0};
				char temperature[10] = {0};
				char sweep[10] = {0};

				//新命令
				char new_command[50] = {0};
				//获取历史命令，历史命令存在
				char old_command[50] = {0};

				i = 0;
	  		 	while( token != NULL )
	  		 	{
	  		 		token = strtok( NULL,"=");
	   				if(i ==0)
						strcpy(model,token);
					else if(i ==1)
						strcpy(wind,token);
					else if(i ==2)
						strcpy(temperature,token);
					else if(i ==3)
						strcpy(sweep,token);
		  			++i;
	  		 	}
				if(i != 5)
					return(INVALID_COMMAND);

				//printf("model:%s wind:%s temperature:%s sweep:%s \n",model,wind,temperature,sweep);
				if(strcmp(model,"NO") != 0){
					//获取上一次数据内容
					if (code_info_msg.old_command[0] == '\0'){
						if(strcmp(model,"a") == 0){
							//if(send_Complete_code(name,"aa") != 0)
							strcpy(ASR_COMMAND,"已设到自动模式");
							return(send_Complete_code(name,"aa"));	
						} else if(strcmp(model,"d") == 0){
							strcpy(ASR_COMMAND,"已设到抽湿模式");
							//if(send_Complete_code(name,"ad") != 0)
							return(send_Complete_code(name,"ad"));
						} else if(strcmp(model,"w") == 0){
							strcpy(ASR_COMMAND,"已设到送风模式");
							//if(send_Complete_code(name,"aw") != 0)
							return(send_Complete_code(name,"aw"));
						} else if(strcmp(model,"h") == 0){
							strcpy(ASR_COMMAND,"已设到制热模式");
							//if(send_Complete_code(name,"ah26") != 0)
							return(send_Complete_code(name,"ah26"));
						} else if(strcmp(model,"r") == 0){
							strcpy(ASR_COMMAND,"已设到制冷模式");
							//if(send_Complete_code(name,"ar26") != 0)
							return(send_Complete_code(name,"ar26"));
						} else {
								return(INVALID_COMMAND);
						}
					
					} else {
						//获取*.record 里面上一次命令 old_command
						/*
						fd = fopen(n_path,"r+");
						if(fd == NULL){
							printf("%s: open command.record error!!!!.\n",__func__);
							return(OPEN_FILE_ERR);
						}
						fread(code_info_msg.old_command,50,1,fd);
						fclose(fd);
						*/
						//打印历史命令
						printf("code_info_msg.old_command:%s  #####\n",code_info_msg.old_command);

						//判断是否自动模式
						if(strcmp(model,"a") == 0){
							strcpy(ASR_COMMAND,"已设到自动模式");
							strcpy(new_command,"aa");
							//if(send_Complete_code(name,new_command) != 0)
							return(send_Complete_code(name,new_command));
											
						} else if(strcmp(model,"d") == 0){
							strcpy(ASR_COMMAND,"已设到抽湿模式");
							strcpy(new_command,"ad");
							//if(send_Complete_code(name,new_command) != 0)
							return(send_Complete_code(name,new_command));	
							
						} else if(strcmp(model,"w") == 0){
							strcpy(ASR_COMMAND,"已设到送风模式");
							strcpy(new_command,"aw");
							//if(send_Complete_code(name,new_command) != 0)
							return(send_Complete_code(name,new_command));
							
						} else if(strcmp(model,"h") == 0){
							strcpy(ASR_COMMAND,"已设到制热模式");
							if(code_info_msg.old_command[1] == 'h' || code_info_msg.old_command[1] == 'r'){
								code_info_msg.old_command[1] = 'h';
								//if(send_Complete_code(name,code_info_msg.old_command) != 0)
								return(send_Complete_code(name,code_info_msg.old_command));
							} else if(code_info_msg.old_command[1] == 'a' || code_info_msg.old_command[1] == 'd' || code_info_msg.old_command[1] == 'w'){
								strcpy(new_command,"ah26");
								//if(send_Complete_code(name,new_command) != 0)
								return(send_Complete_code(name,new_command));

							}	else {
								return(INVALID_OLD_COMMAND);
							}							
						} else if(strcmp(model,"r") == 0){
							strcpy(ASR_COMMAND,"已设到制冷模式");
							if(code_info_msg.old_command[1] == 'h' || code_info_msg.old_command[1] == 'r'){
								code_info_msg.old_command[1] = 'r';
								//if(send_Complete_code(name,code_info_msg.old_command) != 0)
								return(send_Complete_code(name,code_info_msg.old_command));
							} else if(code_info_msg.old_command[1] == 'a' || code_info_msg.old_command[1] == 'd' || code_info_msg.old_command[1] == 'w'){
								strcpy(new_command,"ar26");
								//if(send_Complete_code(name,new_command) != 0)
								return(send_Complete_code(name,new_command));

							}	else {
								return(INVALID_OLD_COMMAND);
							}
														
						}else{
							return(INVALID_COMMAND);
						}

					}
					
				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") != 0){
						//v1 版本没有风量 控制
						strcpy(ASR_COMMAND,"很抱歉，该版本码库没有风量控制功能");
						return 0;
				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") != 0){
						//获取第三级: 温度
						if(temperature[0] < '0' || temperature[0] > '9')
				  			return(INVALID_COMMAND);
						
						int temp = atoi(temperature);
						if(temp > 15 && temp < 31){
							  	strcpy(ASR_COMMAND,"已设到");
					  			strcat(ASR_COMMAND,temperature);
					  			strcat(ASR_COMMAND,"度");
								
						//如果历史命令不存在
							if (code_info_msg.old_command[0] == '\0'){
									sprintf(new_command,"%s%s","ar",temperature);
									//if(send_Complete_code(name,new_command) != 0)
									return(send_Complete_code(name,new_command));
							} else {
									/*
									//获取*.record 里面上一次命令 code_info_msg.old_command
									fd = fopen(n_path,"r+");
									if(fd == NULL){
											printf("%s: open command.record error!!!!.\n",__func__);
											return(OPEN_FILE_ERR);
									}
									fread(code_info_msg.old_command,50,1,fd);
									fclose(fd);
									*/
									//打印历史命令
									//printf("code_info_msg.old_command:%s	#####\n",code_info_msg.old_command);
							
								   
									//获取历史命令中的模式
									if(code_info_msg.old_command[1] == 'h' || code_info_msg.old_command[1] == 'r'){
										  code_info_msg.old_command[2] = temperature[0];
										  code_info_msg.old_command[3] = temperature[1];
										  
										  //printf("new_command:%s ########\n",code_info_msg.old_command);
										  //if(send_Complete_code(name,code_info_msg.old_command) != 0)
										  return(send_Complete_code(name,code_info_msg.old_command));	  
										  
									} else if(code_info_msg.old_command[1] == 'a' || code_info_msg.old_command[1] == 'd' || code_info_msg.old_command[1] == 'w'){
										  sprintf(new_command,"%s%s","ar",temperature);
										  //if(send_Complete_code(name,new_command) != 0)
										  return(send_Complete_code(name,new_command));
									} else {
										  return(INVALID_OLD_COMMAND);
									}
									
							 }
							
						  }else if(temp > 50 && temp < 60){
								//温度升高 1度
								//如果历史命令不存在
								if (code_info_msg.old_command[0] == '\0'){
										strcpy(ASR_COMMAND,"已设到26度");
										sprintf(new_command,"%s","ar26");
										//if(send_Complete_code(name,new_command) != 0)
										return(send_Complete_code(name,new_command));
								} else {
										/*
										//获取*.record 里面上一次命令 code_info_msg.old_command
										fd = fopen(n_path,"r+");
										if(fd == NULL){
												printf("%s: open command.record error!!!!.\n",__func__);
												return(OPEN_FILE_ERR);
										}
										fread(code_info_msg.old_command,50,1,fd);
										fclose(fd);
										*/
										//打印历史命令
										//printf("code_info_msg.old_command:%s	#####\n",code_info_msg.old_command);
								
									   
										//获取历史命令中的模式
										if(code_info_msg.old_command[1] == 'h' || code_info_msg.old_command[1] == 'r'){
											  int old_temperature = atoi(code_info_msg.old_command+2);
											  if((old_temperature + (temp%10))<31){
											  	 old_temperature += (temp%10);
												 memset(temperature,0,10);
												 sprintf(temperature,"%d",old_temperature);
												 code_info_msg.old_command[2] = temperature[0];
												 code_info_msg.old_command[3] = temperature[1];
											  } else if((old_temperature + (temp%10)) > 30){
												   old_temperature = 30;
												   memset(temperature,0,10);
												   sprintf(temperature,"%d",old_temperature);
												   code_info_msg.old_command[2] = temperature[0];
												   code_info_msg.old_command[3] = temperature[1];

											  } else {
												  return(INVALID_OLD_COMMAND);
											  }

											  strcpy(ASR_COMMAND,"已设到");
											  strcat(ASR_COMMAND,temperature);
											  strcat(ASR_COMMAND,"度");	
											  
											  //printf("new_command:%s ########\n",code_info_msg.old_command);
											  //if(send_Complete_code(name,code_info_msg.old_command) != 0)
											  return(send_Complete_code(name,code_info_msg.old_command));	  
											  
										} else if(code_info_msg.old_command[1] == 'a' || code_info_msg.old_command[1] == 'd' || code_info_msg.old_command[1] == 'w'){
											  strcpy(new_command,"ar26");
											  strcpy(ASR_COMMAND,"已设到26度");
											  //if(send_Complete_code(name,new_command) != 0)
											  return(send_Complete_code(name,new_command));
										} else {
											  return(INVALID_OLD_COMMAND);
										}
										
								 }												
						  }else if(temp > 40 && temp < 50){
							    //温度降低
								//如果历史命令不存在
								if (code_info_msg.old_command[0] == '\0'){
										sprintf(new_command,"%s","ar26");
										strcpy(ASR_COMMAND,"已设到26度");	
										//if(send_Complete_code(name,new_command) != 0)
										return(send_Complete_code(name,new_command));
								} else {
										/*
										//获取*.record 里面上一次命令 code_info_msg.old_command
										fd = fopen(n_path,"r+");
										if(fd == NULL){
												printf("%s: open command.record error!!!!.\n",__func__);
												return(OPEN_FILE_ERR);
										}
										fread(code_info_msg.old_command,50,1,fd);
										fclose(fd);
										*/
										//打印历史命令
										//printf("code_info_msg.old_command:%s	#####\n",code_info_msg.old_command);
								
									   
										//获取历史命令中的模式
										if(code_info_msg.old_command[1] == 'h' || code_info_msg.old_command[1] == 'r'){
											  int old_temperature = atoi(code_info_msg.old_command+2);
											  if((old_temperature - (temp%10))> 15){
											  	 old_temperature -= (temp%10);
												 memset(temperature,0,10);
												 sprintf(temperature,"%d",old_temperature);
												 code_info_msg.old_command[2] = temperature[0];
												 code_info_msg.old_command[3] = temperature[1];
											  } else if((old_temperature - (temp%10)) < 16){
												  old_temperature = 16;
												  memset(temperature,0,10);
												  sprintf(temperature,"%d",old_temperature);
												  code_info_msg.old_command[2] = temperature[0];
												  code_info_msg.old_command[3] = temperature[1];

											  } else {
												  return(INVALID_OLD_COMMAND);
											  }

											  strcpy(ASR_COMMAND,"已设到");
											  strcat(ASR_COMMAND,temperature);
											  strcat(ASR_COMMAND,"度");	
											  
											  //printf("new_command:%s ########\n",code_info_msg.old_command);
											  //if(send_Complete_code(name,code_info_msg.old_command) != 0)
											  return(send_Complete_code(name,code_info_msg.old_command));	  
											  
										} else if(code_info_msg.old_command[1] == 'a' || code_info_msg.old_command[1] == 'd' || code_info_msg.old_command[1] == 'w'){
											  strcpy(new_command,"ar26");
											  strcpy(ASR_COMMAND,"已设到26度");	
											  //if(send_Complete_code(name,new_command) != 0)
											  return(send_Complete_code(name,new_command));
										} else {
											  return(INVALID_OLD_COMMAND);
										}
										
								 }


						  }else {
							  return(INVALID_COMMAND);
						  }			

				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") != 0){
						//v1 版本没有扫风 控制
						strcpy(ASR_COMMAND,"很抱歉，该版本码库没有扫风控制功能");
						return 0;
				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") == 0){
					  //如果历史命令不存在
					  strcpy(ASR_COMMAND,"已打开空调");	
					  if (code_info_msg.old_command[0] == '\0'){
							  strcpy(new_command,"aa");
							  //if(send_Complete_code(name,new_command) != 0)
							  return(send_Complete_code(name,new_command));
					  } else{
							/*
					  		//获取*.record 里面上一次命令 code_info_msg.old_command
					 		fd = fopen(n_path,"r+");
					 		if(fd == NULL){
							  	printf("%s: open command.record error!!!!.\n",__func__);
							 	return(OPEN_FILE_ERR);
					  		}
					  		fread(code_info_msg.old_command,50,1,fd);
					  		fclose(fd);
					  		*/
					  		//打印历史命令
					
							//printf("new_command:%s ########\n",code_info_msg.old_command);
							//if(send_Complete_code(name,code_info_msg.old_command) != 0)
						 	return(send_Complete_code(name,code_info_msg.old_command));

					  }
				} else {
						//其他命令无效
						return(INVALID_COMMAND);
				}
				

		} else if(strcmp(power,"CLOSE") == 0){
			//if(send_Complete_code(name,"off") != 0)
			strcpy(ASR_COMMAND,"已关闭空调");
			return(send_Complete_code(name,"off"));
		} else {
			return(INVALID_COMMAND);
		}
	}

	return 0;
}



//匹配模式
int match_handler_func(char *token,result_to_mozart_t *sds_results_msg)
{
	//进入匹配模式 获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	if(strcmp(t,"OK") == 0){
		//获取完整码库 匹配成功
		
		if(get_Complete_code(token,sds_results_msg) != 0)
			return(COMPLETE_GCODE_ERR);
		
		strcat(sds_results_msg->ASR_COMMAND,"码库获取成功，请最少尝试三个按键");
		
	} else {
		if(t[0] < '0' || t[0]>'9')
			return(INVALID_COMMAND);

	
		//发送匹配数据
		char bid[10] = {0};
		char number[10] = {0};
		//发送键值
		char src[10] = {0};
	
		int i = 0;
		while( token != NULL )
		{
			token = strtok( NULL,"=");
			if(i ==0)
				strcpy(bid,token);
			else if(i ==1)
				strcpy(number,token);
			else if(i ==2)
				strcpy(src,token);
			++i;
		}
		if(i != 4)
			return(INVALID_COMMAND); //命令无效
	
		//printf("t:%d bid:%s number:%d  src:%s ########\n",atoi(t),bid,atoi(number),src);


		//判断是否是同一个品牌的数据，避免重复下载，或者是一键匹配数据
		char strMatch[RESPONSE_SIZE] = {0};
		//判断匹配码库是否存在
		if (access("/usr/data/match_code.txt",F_OK) == 0){
				FILE *fd;
				fd = fopen("/usr/data/match_code.txt","r+");
				if(fd == NULL){
					printf("open match_code.txt error!!!!.\n");
					return -1;
				}
				fread(strMatch,RESPONSE_SIZE,1,fd);
				
				fclose(fd);

				char check_info[50] = {0};
				sprintf(check_info,"###%s%s%s###",bid,t,"4");

				if(strstr(strMatch,check_info) != NULL){
					//发送测试码
					if(send_Match_code(atoi(t),atoi(number),src,sds_results_msg) !=0)
						return(MATCH_SCODE_ERR);
				} else {
					//获取匹配码库
					if(get_Match_code(bid,t,"4") != 0)
						return(MATCH_GCODE_ERR);
					
					
					//发送测试码
					if(send_Match_code(atoi(t),atoi(number),src,sds_results_msg) !=0)
						return(MATCH_SCODE_ERR);
				}

				strcpy(sds_results_msg->ASR_COMMAND,"红外匹配数据发送成功");
			
		} else {

				//获取匹配码库
				if(get_Match_code(bid,t,"4") != 0)
					return(MATCH_GCODE_ERR);
				
				
				//发送测试码
				if(send_Match_code(atoi(t),atoi(number),src,sds_results_msg) !=0)
					return(MATCH_SCODE_ERR);

				strcpy(sds_results_msg->ASR_COMMAND,"红外匹配数据发送成功");
		}
		
	}
	return 0;
}

//一键匹配发送模式
int akmsend_handler_func(char *token,result_to_mozart_t *sds_results_msg)
{
	//进入匹配模式 获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	
	if(strcmp(t,"OK") == 0){
		
		if(get_amatch_Complete_code(token,sds_results_msg) != 0)
			return(COMPLETE_GCODE_ERR);
		
		strcpy(sds_results_msg->ASR_COMMAND,"码库获取成功，请最少尝试三个按键");
		
	} else {
		return(INVALID_COMMAND);
	}
	return 0;
}



//一键匹配模式
int akeymatch_handler_func(char *token,char *ASR_COMMAND)
{
	int ret = 0;
	//进入匹配模式 获取电器类型
	char t[10] = {0};
	//发送匹配数据
	char bid[10] = {0};
	char src[10] = {0};

	int i = 0;
	while( token != NULL )
	{
		token = strtok( NULL,"=");
		if(i ==0)
			strcpy(t,token);
		else if(i ==1)
			strcpy(bid,token);
		else if(i ==2)
			strcpy(src,token);
		++i;
	}
	if(i != 4)
		return(INVALID_COMMAND); //命令无效

	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);

	//printf("t:%d bid:%s  src:%s ########\n",atoi(t),bid,src);


	//进入一键匹配模式
	ret = ont_key_match(bid,t,src);
	if(ret != 0){
		return ret;
	}
	
	//删除空调中多余的数据，返回APP	
	/*
	if(strcmp(t,"7") == 0){
		ret = akeymatch_to_app_air(ASR_COMMAND);
	} else 
	*/
	//一键匹配方式，获取码库之后，把全部码库的rid 和 码库版本，返回给APP.，然后在AKMSEND里面获取码库之后不返还rid 和 v。
	if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"3") == 0 || strcmp(t,"5") == 0 || strcmp(t,"6") == 0 || strcmp(t,"7") == 0 || strcmp(t,"8") == 0 || strcmp(t,"10") == 0 || strcmp(t,"12") == 0 || strcmp(t,"13") == 0 || strcmp(t,"15") == 0){
		ret = akeymatch_to_app_other(ASR_COMMAND);
	}else {
		strcpy(ASR_COMMAND,"没有此电器类型");
		return INVALID_COMMAND;
	}
	
	if(ret != 0){
		return ret;
	}	

	return 0;
}

//app 使用码库
int use_handler_func(char *token)
{
	//APP使用 t:电器类型 name:码库名字 appCommand:获取命令
	int ret;
	char t[10] = {0};		
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);
	
	char app_name[128] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(app_name,token);
	
	char appCommand[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(appCommand,token);

	if(strcmp(t,"7") == 0){
			//printf("APPUSE:  app_name :%s  appCommand:%s \n",app_name,appCommand);
			//空调类
			ret = send_Complete_code(app_name,appCommand);
			if(ret != 0)
				return(ret);
	} else if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"6") == 0 || strcmp(t,"10") == 0){
			//非空调类
			ret = send_Complete_code_other(app_name,appCommand);
			if(ret != 0)
				return(ret);
	}else if(strcmp(t,"99") == 0){
			if(atoi(appCommand) >= 0 && atoi(appCommand) <= 16){
				//printf("SEND LEARN DATA:  app_name :%s  appCommand:%s \n",app_name,appCommand);
				if(SEND_IR_LEARN(app_name,atoi(appCommand)) != 0)
					return(SEND_IR_LEARN_ERR);
		}
	} else if(strcmp(t,"51") == 0){
		if(app_use_led(appCommand) != 0)
			return(INVALID_COMMAND);
		
		//printf("### [%s] [%d] ### \n",__func__,__LINE__);
	}	else {
		return(INVALID_COMMAND);
	}

	#if 0
	if(strstr(appCommand,"w") != NULL || strstr(appCommand,"h") != NULL || strstr(appCommand,"r") != NULL || strstr(appCommand,"a") != NULL || strstr(appCommand,"d") != NULL || strstr(appCommand,"off") != NULL){
			//发送码库数据
			printf("APPUSE:  app_name :%s  appCommand:%s \n",app_name,appCommand);
			if(send_Complete_code(app_name,appCommand) != 0)
				return(COMPLETE_SCODE_ERR);
	
	} else if(atoi(appCommand) >= 0 && atoi(appCommand) <= 16){
			//发送学习数据
			if(SEND_IR_LEARN(app_name,atoi(appCommand)) != 0)
				return(SEND_IR_LEARN_ERR);
			
	} else {
			return(INVALID_COMMAND);
	}
	#endif 
	
	return 0;
}

#if 0
//进入学习模式
int learn_handler_func(char *token)
{

	//获取电器类型
	char l_t[10] = {0};
	//获取学习模式文件名
	char l_name[20] = {0};
	//获取键值
	char l_key[10] = {0};
	//读取已经测试过的学习数据
	struct learn_data l_data;
	memset(&l_data,0,sizeof(struct learn_data));
	
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(l_t,token);	
	//获取文件名
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(l_name,token);
	//获取键值	
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(l_key,token);
	int k =atoi(l_key);
	
	//printf("t:%s  name:%s  key:%s  ####\n",l_t,l_name,l_key);
	
	if(strcmp(l_t,"OK") == 0){
		FILE *learnFd;
		//生成文件路径
		char l_path[50] = {0};
		sprintf(l_path,"%s%s%s","/usr/data/",l_name,".txt");
		//如果文件不存在
		if (access(l_path,F_OK) != 0){
				//判断键值是否有效
				if(k < 0 || k > 16)
					return(INVALID_COMMAND);
	
				//判断学习配置文件是否为空
				if (access("/usr/data/learn.txt",F_OK) != 0)
					return(LEARN_PATH_EMPTY);
				
				//读取测试成功的学习数据		
				learnFd = fopen("/usr/data/learn.txt","r+");
				if(learnFd == NULL){
					printf("open secret_key.txt error!!!!.\n");
					return(OPEN_FILE_ERR);
				}
				fread(&l_data,sizeof(l_data),1,learnFd);
				fclose(learnFd);
	
				
				//将学习到的数据写入
				learnFd = fopen(l_path,"w+");
				if(learnFd == NULL){
					printf("%s: open Complete_code.txt error!!!!.\n",__func__);
					return(OPEN_FILE_ERR);
				}
				
				fseek(learnFd,k*sizeof(l_data),SEEK_SET);
				fwrite(&l_data,sizeof(l_data),1,learnFd);
				fclose(learnFd);				
		} else {
				//判断键值是否有效
				if(k < 0 || k > 16)
					return(INVALID_COMMAND);
			
				//判断学习配置文件是否为空
				if (access("/usr/data/learn.txt",F_OK) != 0)
					return(LEARN_PATH_EMPTY);
			
				//读取测试成功的学习数据
			
				learnFd = fopen("/usr/data/learn.txt","r+");
				if(learnFd == NULL){
					printf("open secret_key.txt error!!!!.\n");
					return(OPEN_FILE_ERR);
				}
				fread(&l_data,sizeof(l_data),1,learnFd);
				fclose(learnFd);
			
			
				//将学习到的数据写入
				learnFd = fopen(l_path,"rw+");
				if(learnFd == NULL){
					printf("%s: open Complete_code.txt error!!!!.\n",__func__);
					return(OPEN_FILE_ERR);
				}
			
				fseek(learnFd,k*sizeof(l_data),SEEK_SET);
				fwrite(&l_data,sizeof(l_data),1,learnFd);
				fflush(learnFd);
				fclose(learnFd);
		}
		
	
	} else if(atoi(l_key) >=0 && atoi(l_key) < 17) {
		//进入学习模式，同时将学习到的数据保存到本地 红灯亮
		IR_LEARN_SET(1);
			//return LEARN_DATA_EMPTY;
		usleep(100);
			
		if(READ_IR_LEARN() != 0)
			return(READ_IR_LEARN_ERR);
	
	} else {
		return(INVALID_COMMAND);
	}		

	return 0;
}
#endif 

//保存红外学习数据  LEARN = T =  CMD  /  LEARN = OK = T = NAME
int learn_handler_func(char *token,char *ASR_COMMAND)
{
	int ret = 0;
	//1.获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);
	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);

	//printf("###log: [%s] [%d] ### \n",__func__,__LINE__);

	//2.获取品牌id
	char bid[502] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(bid,token);
	
	//3.获取命令
	char appCommand[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(appCommand,token);

	//红外学习头部信息
	learn_head_t learn_info;
	memset(&learn_info,0,sizeof(learn_head_t));

	if(access("/usr/data/learnIR.txt",F_OK) == 0){
		//获取电器类型
			if(get_learn_info(&learn_info,"learnIR") != 0){
				strcpy(ASR_COMMAND,"读取学习码库失败，请重新尝试");
				return LEARN_MOD_ERR;
			}
			//printf("###log: [%s] [%d] [t:%s] [bid:%s] [learn_info.t:%s] [learn_info.bid:%s] ### \n",__func__,__LINE__,t,bid,learn_info.t,learn_info.bid);

			if(strcmp(t,learn_info.t) != 0 || strcmp(bid,learn_info.bid) != 0){
				remove("/usr/data/learnIR.txt");
			}
	}
	
	//自学习和修改函数
	if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"6") == 0 || strcmp(t,"7") == 0 || strcmp(t,"10") == 0 || strcmp(t,"5") == 0 || strcmp(t,"8") == 0 || strcmp(t,"13") == 0 || strcmp(t,"12") == 0 || strcmp(t,"15") == 0){
		ret = Ele_learn_func(t,bid,"learnIR",appCommand,ASR_COMMAND);
	} else {
		strcpy(ASR_COMMAND,"很抱歉，小乐暂时不支持学习该电器类型");
		return LEARN_MOD_ERR;
	}

	strcpy(ASR_COMMAND,"学习成功");

	return ret;
}


//发送红外学习数据  LENSEND = T = NAME = CMD
int lensend_handler_func(char *token,char *ASR_COMMAND)
{
	int ret = 0;
	//1.获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);

	//2.获取NAME
	char file_name[512] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);

	strcpy(file_name,token);

	//3.获取命令
	char appCommand[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(appCommand,token);

	//file_path 为文件绝对路径
	char file_path[512] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file_name,".txt");

	//判断本地学习码库是否存在
	if(access(file_path,F_OK) != 0){
		strcpy(ASR_COMMAND,"本地码库不存在，请重新尝试");
		return LEARN_MOD_ERR;
	}

	//自学习发送函数
	ret = Ele_send_func(t,file_name,appCommand);
	
	strcpy(ASR_COMMAND,"红外按键发送成功");

	return ret;
}

//修改学习过的红外码库数据 LENMOD = T = bid = NAME = power
int lenmod_handler_func(char *token,char *ASR_COMMAND)
{
	int ret = 0;
	//1.获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);
	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);

	//printf("###log: [%s] [%d] ### \n",__func__,__LINE__);

	//2.获取品牌id
	char bid[502] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(bid,token);

	//3.获取NAME
	char file_name[512] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);

	strcpy(file_name,token);

	//file_path 为文件绝对路径
	char file_path[512] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file_name,".txt");
	
	//4.获取命令
	char appCommand[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(appCommand,token);

	//如果码库不存在 返回
	if(access(file_path,F_OK) != 0){
			//获取电器类型
			return CODE_NOT_EXIST;	
	}

	//读取头部信息 红外学习头部信息
	learn_head_t learn_info;
	memset(&learn_info,0,sizeof(learn_head_t));	

	if(get_learn_info(&learn_info,file_name) != 0){
		strcpy(ASR_COMMAND,"读取学习码库失败，请重新尝试");
		return LEARN_MOD_ERR;
	}
	//printf("###log: [%s] [%d] [t:%s] [bid:%s] [learn_info.t:%s] [learn_info.bid:%s] ### \n",__func__,__LINE__,t,bid,learn_info.t,learn_info.bid);

	//if(strcmp(t,learn_info.t) != 0 || strcmp(bid,learn_info.bid) != 0){
	if(strcmp(t,learn_info.t) != 0){
		strcpy(ASR_COMMAND,"电器信息校验失败，请重新尝试");
		return LEARN_MOD_ERR;
	}
	
	//自学习和修改函数
	if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"6") == 0 || strcmp(t,"7") == 0 || strcmp(t,"10") == 0 || strcmp(t,"5") == 0 || strcmp(t,"8") == 0 || strcmp(t,"13") == 0 || strcmp(t,"12") == 0 || strcmp(t,"15") == 0){
		ret = Ele_learn_func(t,bid,file_name,appCommand,ASR_COMMAND);
	} else {
		strcpy(ASR_COMMAND,"很抱歉，小乐暂时不支持学习该电器类型");
		return LEARN_MOD_ERR;
	}

	strcpy(ASR_COMMAND,"学习成功");

	return ret;
}
//添加学习过的红外码库数据
int lenadd_handler_func(char *token,char *ASR_COMMAND)
{

	int ret = 0;
	//1.获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);
	
	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);

	//1.获取品牌电器类型
	char bid[502] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(bid,token);


	//2.获取NAME
	char file_name[512] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);

	strcpy(file_name,token);


	//红外学习头部信息
	learn_head_t learn_info;
	memset(&learn_info,0,sizeof(learn_head_t));
	
	//是 添加到默认遥控器 和 电器列表
	if(access("/usr/data/learnIR.txt",F_OK) != 0){
		strcpy(ASR_COMMAND,"本地码库不存在，请重新尝试");
		return LEARN_MOD_ERR;
	}

	if(get_learn_info(&learn_info,"learnIR") != 0){
		strcpy(ASR_COMMAND,"学习码库添加失败，请重新尝试");
		return LEARN_MOD_ERR;

	}
	//printf("###log: [%s] [%d] [t:%s] [bid:%s] [learn_info.t:%s] [learn_info.bid:%s] ### \n",__func__,__LINE__,t,bid,learn_info.t,learn_info.bid);

	if(strcmp(t,learn_info.t) != 0 || strcmp(bid,learn_info.bid) != 0){
		strcpy(ASR_COMMAND,"电器信息校验错误，请重新尝试");
		return LEARN_MOD_ERR;
	}

	//1.设置默认遥控器

	//file_path 为文件绝对路径
	char file_path[512] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file_name,".txt");

 	FILE *fd;

	//如果是空调，生成一个历史命令文件 默认ar16
	if(strcmp(t,"7") == 0){
			//生成上一次红外历史命令路径
			char n_path[200] = {0};
			sprintf(n_path,"%s%s%s","/usr/data/",file_name,".record");

			fd = fopen(n_path,"w+");
			if(fd == NULL){
				printf("open asr path.txt error!!!!.\n");
				return OPEN_FILE_ERR;
			}
			fwrite("ar16",strlen("ar26")+1,1,fd);
			fclose(fd);					
	}

	char v[10]  = "900";
	
	//码库已经存在 不写入码库列表中
	if(access(file_path,F_OK) != 0){
		//删除同名的文件
		delete_code_list(t,file_name);
		save_code_list(t,v,file_name);
	}


	rename("/usr/data/learnIR.txt", file_path);

	/**************语音控制配置文件*****************/
	char asr_path[60] ={0};
	sprintf(asr_path,"%s%s%s","/usr/data/",t,".txt");
	

	fd = fopen(asr_path,"w+");
	if(fd == NULL){
		printf("open asr path.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fwrite(v,10,1,fd);
	fwrite(file_name,strlen(file_name)+1,1,fd);
	fclose(fd);	

	
	//2.添加到对应的电器列表中
	strcpy(ASR_COMMAND,"码库添加成功,请使用");
	
	return 0;
}



//服务器使用码库
int server_handler_func(char *token,char *ASR_COMMAND)
{
	int ret = 0;

	//获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);
	
	char name[128] = {0};
	char str_v[10] = {0};
	if(strcmp(t,"51") != 0){
			//获取语音控制码库路径
			char t_path[50] = {0};
			sprintf(t_path,"%s%s%s","/usr/data/",t,".txt");
			
			//判断语音控制码库不存在，退出,需要先匹配码库
			if (access(t_path,F_OK) != 0)
				return(SET_ASR_IR);
			
			//获取语音控制码库名称
			//char name[60] = {0};
			
			FILE *fd;
			fd = fopen(t_path,"r+");
			if(fd == NULL){
				printf("%s: open Complete_code.txt error!!!!.\n",__func__);
				return(OPEN_FILE_ERR);
			}
			fread(str_v,10,1,fd);
			fread(name,128,1,fd);
			fclose(fd);
	} else {
		strcpy(name,"bngoLed");
	}
	
	//生成上一次红外历史命令路径
	char n_path[200] = {0};
	sprintf(n_path,"%s%s%s","/usr/data/",name,".record");
	
	/* air_handler_func();
	 *	v: 码库的版本  name: 码库名称  n_path: 码库历史命令路径，token: 接受到的指令 
	 */

	if(strcmp(t,"7") == 0){ 
		if(strcmp(str_v,"900") == 0){
			//自学习 
			ret = air_learn_func(t,name,token,ASR_COMMAND);
		} else {
			//码库
			ret = air_handler_func(str_v,name,n_path,token,ASR_COMMAND);			
		}
	}else if(strcmp(t,"51") == 0){ 
		ret = led_handler_func(n_path,token,ASR_COMMAND);			
	} else if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"10") == 0 || strcmp(t,"5") == 0 || strcmp(t,"8") == 0 || strcmp(t,"13") == 0 || strcmp(t,"12") == 0 || strcmp(t,"15") == 0){
		if(strcmp(str_v,"900") == 0){
			//自学习 
			ret = box_learn_func(t,name,token,ASR_COMMAND);
		} else {
			//码库
			ret = box_handler_func(t,str_v,name,n_path,token,ASR_COMMAND);
		}

	} else if(strcmp(t,"6") == 0){ 
		if(strcmp(str_v,"900") == 0){
			//自学习 
			ret = fan_learn_func(t,name,token,ASR_COMMAND);
		} else {
			//码库
			ret = fan_handler_func(str_v,name,n_path,token,ASR_COMMAND);		
		}
	} else {
		return(INVALID_COMMAND);
	}

	return ret;
}

//语音同时打开两种电器
int serverbind_handler_func(char *token,char *ASR_COMMAND)
{
	int ret = 0;

	//1.获取电器类型 判断参数
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	
	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);

	//2.获取获取控制命令cmd 判断cmd(power poweroff)

	char command[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(command,token);

	if(strcmp(command,"power") == 0){
			strcpy(ASR_COMMAND,"已打开电视和机顶盒");
	} else if(strcmp(command,"poweroff") == 0){
			strcpy(ASR_COMMAND,"已关闭电视和机顶盒");
	} else {
		return(INVALID_COMMAND);
	}
	
	//3.判断绑定关系文件是否存在

	if(access("/usr/data/electric_bind.txt",F_OK) != 0){
			return BIND_NOT_EXIST;
	}

	//4.获取绑定关系: 电器类型T 版本V file_name
	char bind_info[1024] = {0};

	FILE *fd;

	///////////获取历史命令/////////////////////////////////////
	fd = fopen("/usr/data/electric_bind.txt","r+");
	if(fd == NULL){
		printf("%s: open command.record error!!!!.\n",__func__);
		return(OPEN_FILE_ERR);
	}
	fread(bind_info,sizeof(bind_info),1,fd);
	fclose(fd);

	int i =0;
	char tv_t[10] = {0};
	char box_t[10] = {0};
	
	char tv_v[10] = {0};
	char box_v[10] = {0};

	char tv_file_name[512] = {0};
	char box_file_name[512] = {0};

	char *bind_token;	
	bind_token = strtok(bind_info,"=");
	if(bind_token == NULL){
		ret = BIND_NOT_EXIST;
	}
	
 	while( bind_token != NULL )
 	{
 		bind_token = strtok( NULL,"=");
		if(i ==0)
			strcpy(tv_t,bind_token);
		else if(i ==1)
			strcpy(tv_v,bind_token);
		else if(i ==2)
			strcpy(tv_file_name,bind_token);
		else if(i ==3)
			strcpy(box_t,bind_token);
		else if(i ==4)
			strcpy(box_v,bind_token);
		else if(i ==5)
			strcpy(box_file_name,bind_token);
		++i;
 	}
	
	//printf("[%s] [%d] [i:%d] tv_t:%s tv_v:%s tv_file_name:%s box_t:%s box_v:%s box_file_name:%s\n",__func__,__LINE__,i,tv_t,tv_v,tv_file_name,box_t,box_v,box_file_name);
	if(i != 7)
		return BIND_NOT_EXIST;

	//5.判断是自学习还是码库 发送电视cmd  发送机顶盒cmd
	
	//发送电视开关
	if(strcmp(tv_v,"900") == 0){
		//自学习 
		ret = Ele_send_func(tv_t,tv_file_name,"power");

		/*
		if(ret == JSON_KEY_NOT_EXIST && strcmp(command,"poweroff") == 0){
			ret = Ele_send_func(tv_t,tv_file_name,"power");
		}
		*/
	} else {
		//码库
		ret = send_Complete_code_other(tv_file_name,command);

		if(ret == JSON_KEY_NOT_EXIST && strcmp(command,"poweroff") == 0){
			ret = send_Complete_code_other(tv_file_name,"power");
		}
	}
	if(ret != 0)
		return ret;
		
	//中间500ms的时间差
	usleep(100000);
	//发送盒子开关
	if(strcmp(box_v,"900") == 0){
		//自学习 
		ret = Ele_send_func(box_t,box_file_name,"power");

		/*
		if(ret == JSON_KEY_NOT_EXIST && strcmp(command,"poweroff") == 0){
			ret = Ele_send_func(box_t,box_file_name,"power");
		}
		*/
		
	} else {
		//码库
		ret = send_Complete_code_other(box_file_name,command);
		
		if(ret == JSON_KEY_NOT_EXIST && strcmp(command,"poweroff") == 0){
			ret = send_Complete_code_other(box_file_name,"power");
		}
	}

	return ret;
}


//app设置绑定关系
int appbind_handler_func(char *token,char *ASR_COMMAND)
{
	//1.获取绑定关系: 电器类型T 版本V file_name
	int i =0;
	char tv_t[10] = {0};
	char box_t[10] = {0};
	
	char tv_v[10] = {0};
	char box_v[10] = {0};

	char tv_file_name[512] = {0};
	char box_file_name[512] = {0};
	
 	while( token != NULL )
 	{
 		token = strtok( NULL,"=");
		if(i ==0)
			strcpy(tv_t,token);
		else if(i ==1)
			strcpy(tv_v,token);
		else if(i ==2)
			strcpy(tv_file_name,token);
		else if(i ==3)
			strcpy(box_t,token);
		else if(i ==4)
			strcpy(box_v,token);
		else if(i ==5)
			strcpy(box_file_name,token);
		++i;
 	}
	//printf("[%s] [%d] [i:%d] tv_t:%s tv_v:%s tv_file_name:%s box_t:%s box_v:%s box_file_name:%s\n",__func__,__LINE__,i,tv_t,tv_v,tv_file_name,box_t,box_v,box_file_name);
	if(i != 7)
		return INVALID_COMMAND;


	//2.判断参数是否正确
	if(strcmp(tv_t,"2") != 0 || tv_v[0] < '0' || tv_v[0] > '9' || (strcmp(box_t,"1") != 0 && strcmp(box_t,"10") != 0) || box_v[0] < '0' || box_v[0] > '9')
		return INVALID_COMMAND;

	//file_path 为文件绝对路径
	char tv_file_path[512] = {0};
	char box_file_path[512] = {0};
	sprintf(tv_file_path,"%s%s%s","/usr/data/",tv_file_name,".txt");
	sprintf(box_file_path,"%s%s%s","/usr/data/",box_file_name,".txt");

	if(access(tv_file_path,F_OK) != 0 || access(box_file_path,F_OK) != 0)
		return CODE_NOT_EXIST;

	char bind_info[1024] = {0};

	sprintf(bind_info,"APPBIND=%s%s%s%s%s%s%s%s%s%s%s",tv_t,"=",tv_v,"=",tv_file_name,"=",box_t,"=",box_v,"=",box_file_name);

	
	//3.将绑定关系保存在electric_bind.txt
	FILE *fd;

	///////////获取历史命令/////////////////////////////////////
	fd = fopen("/usr/data/electric_bind.txt","w+");
	if(fd == NULL){
		printf("%s: open command.record error!!!!.\n",__func__);
		return(OPEN_FILE_ERR);
	}
	fwrite(bind_info,sizeof(bind_info),1,fd);
	fclose(fd);

	strcpy(ASR_COMMAND,"绑定关系添加成功");

	return 0;
}


//app 删除码库
int delete_handler_func(char *token)
{
	//获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);
	
	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);


	char file[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(file,token);

	//获取 删除服务器码库id
	char szhy_code_id[100] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(szhy_code_id,token);
	
	if(szhy_code_id[0] < '0' || szhy_code_id[0] > '9')
		return(INVALID_COMMAND);
	
	

	FILE *fd;
	char name[128] = {0};
	char str_v[10] = {0};

	//生成上一次红外历史命令路径
	char d_path[200] = {0};
	char d_record[200] = {0};
	char t_path[50] = {0};
	sprintf(d_path,"%s%s%s","/usr/data/",file,".txt");	
	sprintf(d_record,"%s%s%s","/usr/data/",file,".record");
	sprintf(t_path,"%s%s%s","/usr/data/",t,".txt");

	//printf("d_path :%s d_record:%s \n",d_path,d_path);

	//删除码库
	if(szhy_get("del_code",szhy_code_id) != 0){
		return(DEL_CODE_ERR);
	}
	
	if(access(d_path,F_OK) == 0){
		remove(d_path);
	}
	
	if(access(d_record,F_OK) == 0){
		remove(d_record);
	}

	if(access(t_path,F_OK) == 0){
		//读取语音配置文件
		fd = fopen(t_path,"r+");
		if(fd == NULL){
			printf("%s: open Complete_code.txt error!!!!.\n",__func__);
			return(DEL_CODE_ERR);
		}
		fread(str_v,10,1,fd);
		fread(name,128,1,fd);
		fclose(fd);

		//如果删除的码库是默认遥控器，则删除默认遥控器配置文件
		if(strstr(name,file) != NULL){
			remove(t_path);
		}
	}

	delete_code_list(t,file);
	

	//如果是电视 、机顶盒、网络盒子 删除绑定关系
	if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"10") == 0){

		if(access("/usr/data/electric_bind.txt",F_OK) == 0){

				//4.获取绑定关系: 电器类型T 版本V file_name
				char bind_info[1024] = {0};
				
				///////////获取历史命令/////////////////////////////////////
				fd = fopen("/usr/data/electric_bind.txt","r+");
				if(fd == NULL){
					printf("%s: open command.record error!!!!.\n",__func__);
					return(OPEN_FILE_ERR);
				}
				fread(bind_info,sizeof(bind_info),1,fd);
				fclose(fd);

				if(strstr(bind_info,file) != NULL){
					//删除绑定关系
					remove("/usr/data/electric_bind.txt");
				}			
		}
		
	}
	
		
	return 0;
}


//app 添加码库
int add_handler_func(char *token)
{

	//获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);


	//码库名称
	char file[512] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);

	strcpy(file,token);

	//校验APP添加的文件与 本地下载的码库 是否相同  file 和 h_path
	//判断rid:遥控id是否存在
	if (access("/usr/data/rid.txt",F_OK) != 0 || access("/usr/data/bngoIR.txt",F_OK) != 0)
		return -1;
	
	//读取rid, c_path:唯一的码库名字
	char rid[128] ={0};
	char h_path[60] ={0};
	char str_v[10] = {0};
	FILE *fd;
	fd = fopen("/usr/data/rid.txt","r+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fread(rid,128,1,fd);
	fread(str_v,10,1,fd);
	fread(h_path,60,1,fd);
	fclose(fd);

	//去掉特殊字符
	char rid_mod_srt[50] = {0};
	char *p = rid_mod_srt;
	char *m = rid;
	
	while(*m != '\0'){
		if(*m >= 'a' && *m <= 'z'){
			*p++ = *m++;
		}else if(*m >= 'A' && *m <= 'Z'){
			*p++ = *m++;
		}else if(*m >= '0' && *m <= '9'){
			*p++ = *m++;
		}else{
			++m;	
		}
	}
	*p = '\0';
	
	//printf("rid_mod_srt : %s ####\n",rid_mod_srt);
	
	if(strcmp(file,h_path) != 0 && strstr(file,rid_mod_srt) == NULL){
		printf("无效命令: app_file[%s] bngo_h_path[%s] rid_mod_srt[%s]\n",file,h_path,rid_mod_srt);
		if(((strstr(file,"美的") || strstr(file,"华凌") || strstr(file,"小天鹅") || strstr(file,"荣事达")) && (strstr(h_path,"美的") || strstr(h_path,"华凌") || strstr(h_path,"小天鹅") || strstr(h_path,"荣事达")))){
			;
		} else {
			return INVALID_COMMAND;
		}

	}


	//文件名路径
	char d_path[200] = {0};
	char d_path_record[200] = {0};
	
	sprintf(d_path,"%s%s%s","/usr/data/",file,".txt");

	sprintf(d_path_record,"%s%s%s","/usr/data/",file,".record");

	//码库已经存在 不写入码库列表中
	if(access(d_path,F_OK) != 0){
		//删除同名的文件
		delete_code_list(t,file);
		save_code_list(t,str_v,file);
	}

	rename("/usr/data/bngoIR.txt", d_path);
	rename("/usr/data/bngoIR.record", d_path_record);

	/**************语音控制配置文件*****************/
	char asr_path[60] ={0};
	sprintf(asr_path,"%s%s%s","/usr/data/",t,".txt");
 	
	
	//if (access(asr_path,F_OK) != 0){
		fd = fopen(asr_path,"w+");
		if(fd == NULL){
			printf("open asr path.txt error!!!!.\n");
			return OPEN_FILE_ERR;
		}
		fwrite(str_v,10,1,fd);
		fwrite(file,strlen(file)+1,1,fd);
		fclose(fd);		
		//printf("[%s] set ir success! #########\n",__func__);
	//}

	return 0;
}

//setir 设置默认遥控器
int setir_handler_func(char *token)
{
	//获取电器类型
	char t[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);

	if(t[0] < '0' || t[0]>'9')
		return(INVALID_COMMAND);

	char str_v[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(str_v,token);

	if(str_v[0] < '0' || str_v[0]>'9')
		return(INVALID_COMMAND);

	char file[128] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(file,token);

	//默认遥控器路径
	char d_path[50] = {0};
	sprintf(d_path,"%s%s%s","/usr/data/",t,".txt");	


	//判断码库是否存在
	char file_path[200] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file,".txt");

	if(access(file_path,F_OK) != 0)
		return CODE_NOT_EXIST;

	//////////////////////////////////////////////////////////////////
	FILE *fd;	
	//如果不是自学习的 判断码库版本是否一致
	if(strcmp(str_v,"900") != 0){
			char strMatch[RESPONSE_SIZE] = {0};

			fd = fopen(file_path,"r+");
			if(fd == NULL){
				printf("open match_code.txt error!!!!.\n");
				return -1;
			}
			fread(strMatch,RESPONSE_SIZE,1,fd);
			fclose(fd);
			
			char *json_str = NULL;
			json_str = strstr(strMatch,"{");
			
			//判断传过来的码库版本，跟本地数据是否一致
			int code_list_v = 0;
			if(get_code_v(json_str,&code_list_v) != 0)
				return CODE_V_ERR;
			
			if(code_list_v != atoi(str_v)){
				//printf("### [%s] [%d] app_v:%s code_v:%d ###\n",__func__,__LINE__,str_v,code_list_v);
				return CODE_V_ERR;
			}

	}

	///////////////////////////////////////////////////////////
	
	//FILE *fd;
	fd = fopen(d_path,"w+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fwrite(str_v,10,1,fd);
	fwrite(file,strlen(file)+1,1,fd);
	fclose(fd); 

	return 0;
}


