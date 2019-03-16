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
int save_code_msg(char *code_v,char *code_name)
{
	char add_code_list[512] = {0};
	strcat(add_code_list,"echo \"");
	strcat(add_code_list,code_v);
	strcat(add_code_list,":");
	strcat(add_code_list,code_name);
	strcat(add_code_list,"\"");
	strcat(add_code_list," >> ");
	strcat(add_code_list,CODEPATH);
	strcat(add_code_list,"code_list.txt");
	system(add_code_list);


	//printf("[%s] add_code_list:%s ### \n",__func__,add_code_list);
	return  0;
}

//删除码库名称 从code_list.txt
int delete_code_name(char *del_code_name)
{
	
    char* file_from = "/usr/data/code_list.txt";
    char* file_to = "/usr/data/code_list.bk";

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
		
		
		
		char asr_path[60] ={0};
		sprintf(asr_path,"%s","/usr/data/7.txt");

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
int box_handler_func(char *v,char *name,char  *n_path,char *token)
{
	int ret = 0;

	if(strcmp(v,"1") != 0)
		return CODE_V_ERR;
	
	//获取电视命令
	char box_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(box_command,token);
	
	///////////////////////////////////////////////////////////
	//printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);

	
	if(strcmp(box_command,"0")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"1")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"2")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"3")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"4")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"5")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"6")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"7")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"8")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"9")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"ch-")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"ch+")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"vol-")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"vol+")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"up")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"down")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"left")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"right")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"exit")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"menu")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"mute")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"ok")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"power")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"signal")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"back")){
		ret = send_Complete_code(name,box_command);
	}else if(strcmp(box_command,"boot")){
		ret = send_Complete_code(name,box_command);
	} else {
			return INVALID_COMMAND;
	}
	if(ret != 0)
		return ret;

	return 0;
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

	
	if(strcmp(tv_command,"0")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"1")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"2")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"3")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"4")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"5")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"6")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"7")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"8")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"9")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"ch-")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"ch+")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"vol-")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"vol+")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"up")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"down")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"left")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"right")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"exit")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"menu")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"mute")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"ok")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"power")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"signal")){
		ret = send_Complete_code(name,tv_command);
	}else if(strcmp(tv_command,"back")){
		ret = send_Complete_code(name,tv_command);
	} else {
			return INVALID_COMMAND;
	}
	if(ret != 0)
		return ret;

	return 0;
}

int send_Fan_code(int key)
{
	 struct learn_data l_date;
	 l_date.count = 0;
	 memset(l_date.times,0,512);
	 int flag = 1;
	 
	 FILE *l_fd;
	 l_fd = fopen("/usr/data/fan.txt","r+");
	 if(l_fd == NULL){
		 printf("open secret_fan.txt error!!!!.\n");
		 return 10;
	 }
	 fseek(l_fd,key*sizeof(l_date),SEEK_SET);
	 fread(&l_date,sizeof(l_date),1,l_fd);
	 fclose(l_fd);

	 
	 //printf("count:%d . key:%d ----\n",l_date.count,key);
	 //没有学习到数据
	 if(l_date.count == 0)
	 	return 9;
	 int i;
	 for(i = 0;i<l_date.count;i++){
		 //printf("%lld ",p_date.times[i]); 
		 //printf("%d ",l_date.times[i]); 
		 IR_SEND(l_date.times[i],flag);
		 flag ^= 1;
	 }	 

	return 0;
}


//风扇
int fan_handler_func(char *v,char *name,char  *n_path,char *token)
{

	int ret = 0;

	if(strcmp(v,"1") != 0)
		return CODE_V_ERR;
	
	//获取风扇命令
	char fan_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(fan_command,token);
	///////////////////////////////////////////////////////////
	//printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);

	
	//摇头
	if(!strcmp(fan_command,"oscillation")){
		ret = send_Fan_code(2);
	//开关
	}else if(!strcmp(fan_command,"power")){
		ret = send_Fan_code(0);
	//风速
	}else if(!strcmp(fan_command,"fanspeed")){
		ret = send_Fan_code(1);
	} else {
			return INVALID_COMMAND;
	}
	if(ret != 0)
		return ret;
	#if 0
	//模式
	if(strcmp(fan_command,"mode")){
		ret = send_Complete_code(name,fan_command);
	//摇头
	}else if(strcmp(fan_command,"oscillation")){
		ret = send_Complete_code(name,fan_command);
	//开关
	}else if(strcmp(fan_command,"power")){
		ret = send_Complete_code(name,fan_command);
	//定时
	}else if(strcmp(fan_command,"timer")){
		ret = send_Complete_code(name,fan_command);
	//风速
	}else if(strcmp(fan_command,"fanspeed")){
		ret = send_Complete_code(name,fan_command);
	} else {
			return INVALID_COMMAND;
	}
	if(ret != 0)
		return ret;
	#endif 
	
	return 0;
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
	
	
	if(strcmp(v,"3") == 0){
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
		
		char list[10] = {0};
		token = strtok( NULL,"=");
		if(token == NULL)
			return(INVALID_COMMAND);
		strcpy(list,token);
		
		if(get_Complete_code(list,sds_results_msg) != 0)
			return(COMPLETE_GCODE_ERR);
		
		strcat(sds_results_msg->ASR_COMMAND,"码库获取成功，请最少尝试三个按键");
		
	} else {
		if(t[0] < '0' || t[0]>'9')
			return(INVALID_COMMAND);

	
		//发送匹配数据
		char bid[10] = {0};
		char number[10] = {0};
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
		#if 0
		if(t[0] < '0' || t[0]>'9')
			return(INVALID_COMMAND);
	
		//发送匹配数据
		char bid[10] = {0};
		char number[10] = {0};
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

				char check_AKEYMATCH_info[50] = {0};
				sprintf(check_AKEYMATCH_info,"###%s%s%s###",bid,t,src);

				if(strstr(strMatch,check_AKEYMATCH_info) != NULL){
					//发送测试码
					if(send_Match_code(atoi(t),atoi(number),src,sds_results_msg) !=0)
						return(MATCH_SCODE_ERR);
						
				} else {
					//printf("[%s] check check_AKEYMATCH_info error \n",__func__);
					return(INVALID_COMMAND);
				}

				strcpy(sds_results_msg->ASR_COMMAND,"红外匹配数据发送成功");
			
		} else {
			//printf("[%s] match_code.txt not exit \n",__func__);
			return(CODE_NOT_EXIST);
		}

		#endif
		
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
	if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"3") == 0 || strcmp(t,"5") == 0 || strcmp(t,"6") == 0 || strcmp(t,"7") == 0 || strcmp(t,"8") == 0 || strcmp(t,"10") == 0 || strcmp(t,"12") == 0 || strcmp(t,"13") == 0 || strcmp(t,"15") == 0){
		ret = akeymatch_to_app_other(ASR_COMMAND);
	}else {
		strcpy(ASR_COMMAND,"没有此电器类型");
		return INVALID_COMMAND;
	}
	
	if(ret != 0){
		return ret;
	}	
	
	//发送测试码
	//if(send_Match_code(atoi(t),0,src) !=0)
		//return(MATCH_SCODE_ERR);

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

	//空调 机顶盒 电视 风扇
	//if(strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"3") == 0 || strcmp(t,"5") == 0 || strcmp(t,"6") == 0 || strcmp(t,"7") == 0 || strcmp(t,"8") == 0 || strcmp(t,"10") == 0 || strcmp(t,"12") == 0 || strcmp(t,"13") == 0 || strcmp(t,"15") == 0){

	if(strcmp(t,"7") == 0){
			//printf("APPUSE:  app_name :%s  appCommand:%s \n",app_name,appCommand);
			ret = send_Complete_code(app_name,appCommand);
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

//服务器使用码库
int server_handler_func(char *token,char *ASR_COMMAND)
{
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
	if(strcmp(t,"51") != 0 && strcmp(t,"6") != 0){
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
	int ret;

	if(strcmp(t,"7") == 0){ 
		ret = air_handler_func(str_v,name,n_path,token,ASR_COMMAND);			
	}else if(strcmp(t,"51") == 0){ 
		ret = led_handler_func(n_path,token,ASR_COMMAND);			
	}
	/*
	if(strcmp(t,"1") == 0){
		ret = box_handler_func(str_v,name,n_path,token);
	}else if(strcmp(t,"2") == 0){ 
		ret = tv_handler_func(str_v,name,n_path,token);		
	}else if(strcmp(t,"7") == 0){ 
		ret = air_handler_func(str_v,name,n_path,token,ASR_COMMAND);			
	}else if(strcmp(t,"6") == 0){ 
		//ret = fan_handler_func(str_v,name,n_path,token);	
		ret = fan_handler_func("1",name,n_path,token);	
	}else if(strcmp(t,"51") == 0){ 
		ret = led_handler_func(n_path,token,ASR_COMMAND);			
	}
	*/
	//出错返回
	if(ret != 0)
		return(ret);	

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

		if(strstr(name,file) != NULL){
			remove(t_path);
		}
	}

	delete_code_name(file);

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
		delete_code_name(file);
		save_code_msg(str_v,file);
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

	//判断rid:遥控id是否存在
	//if (access(d_path,F_OK) != 0)
		//return SETIR_FILE_ERR;


	//判断码库是否存在
	char file_path[200] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file,".txt");

	if(access(file_path,F_OK) != 0)
		return CODE_NOT_EXIST;

	//////////////////////////////////////////////////////////////////
	char strMatch[RESPONSE_SIZE] = {0};
	FILE *fd;
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
		printf("### [%s] [%d] app_v:%s code_v:%d ###\n",__func__,__LINE__,str_v,code_list_v);
		return CODE_V_ERR;
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


