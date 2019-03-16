#include "all_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cacertinmem.h"
#include "bngo_ir.h"
#include "bngo_ir_learn.h"
#include "bngo_led.h"


//红外学习接受的数据，最大存储64字节
struct learn_data {
	unsigned int count;
	unsigned int times[512];
};

int led_handler_func(char  *n_path,char *token){
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
	char new_command[5] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(new_command,token);

	char old_command[5] = {0};

	if(strcmp(new_command,"0") == 0){
		//关夜灯
		bngo_led_close("PB15");
		return 0;
	}

	if (access(n_path,F_OK) != 0){
		if(strcmp(new_command,"5") == 0){
			//开夜灯
			bngo_led_open("PB15");
		} 
	} else {
		//获取*.record 里面上一次命令 old_command
		fd = fopen(n_path,"r+");
		if(fd == NULL){
			printf("%s: open command.record error!!!!.\n",__func__);
			return(OPEN_FILE_ERR);
		}
		fread(old_command,5,1,fd);
		fclose(fd);
		//打印历史命令
		printf("old_command:%s	#####\n",old_command);

		if(strcmp(new_command,"5") == 0){
			//关夜灯
			bngo_led_open("PB15");
		} else if(strcmp(new_command,"6") == 0){
			//亮一点
			bngo_led_open("PB15");
		} else if(strcmp(new_command,"7") == 0){
			//暗一点
			bngo_led_open("PB15");
		} else {
			return(INVALID_COMMAND);
		}
	}

	/*************记录历史命令*******************/
	fd = fopen(n_path,"w+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		return -1;
	}
	fwrite(new_command,strlen(new_command),1,fd);
	fclose(fd);
	
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
	printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);

	
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
	printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);

	
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
	printf("[%s] str_v:%s  command:%s ####\n",__func__,v,token);
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

	return 0;
}


//空调
int air_handler_func(char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND)
{
	printf("[%s] str_v:%s ####\n",__func__,v);
	int i;
	FILE *fd;
	
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

				printf("model:%s wind:%s temperature:%s sweep:%s \n",model,wind,temperature,sweep);
				//获取第一级: 模式
				if(strcmp(model,"NO") != 0){
					//获取上一次数据内容
					if (access(n_path,F_OK) != 0){
						if(strcmp(model,"a") == 0){
							strcpy(ASR_COMMAND,"已设到自动模式");
							if(send_Complete_code(name,"a_s0__u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);	
						} else if(strcmp(model,"d") == 0){
							strcpy(ASR_COMMAND,"已设到抽湿模式");
							if(send_Complete_code(name,"d_s1__u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"w") == 0){
							strcpy(ASR_COMMAND,"已设到送风模式");
							if(send_Complete_code(name,"w_s1_26_u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"h") == 0){
							strcpy(ASR_COMMAND,"已设到制热模式");
							if(send_Complete_code(name,"h_s0_26_u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"r") == 0){
							strcpy(ASR_COMMAND,"已设到制冷模式");
							if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else {
								return(INVALID_COMMAND);
						}
					
					} else {
						//获取*.record 里面上一次命令 old_command
						fd = fopen(n_path,"r+");
						if(fd == NULL){
							printf("%s: open command.record error!!!!.\n",__func__);
							return(OPEN_FILE_ERR);
						}
						fread(old_command,50,1,fd);
						fclose(fd);
						//打印历史命令
						printf("old_command:%s  #####\n",old_command);

						//判断是否自动模式
						if(strcmp(model,"a") == 0){
							strcpy(ASR_COMMAND,"已设到自动模式");
							if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
								old_command[0] = 'a';
								if(send_Complete_code(name,old_command) != 0)
									return(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL){
								old_token = strtok(old_command,"_");
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
								printf("new_command:%s ########\n",new_command);
								if(send_Complete_code(name,new_command) != 0)
									return(COMPLETE_SCODE_ERR);
							}else {
								return(INVALID_OLD_COMMAND);

							}
											
						} else if(strcmp(model,"d") == 0){

							strcpy(ASR_COMMAND,"已设到抽湿模式");
							if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
									//抽湿模式：风量只有s1
									//获取上下扫风
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}		

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s","d","_","s1","_","_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,"d_s1__u0_l0_p0") != 0)
										return(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL){
								old_token = strtok(old_command,"_");
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
								printf("new_command:%s ########\n",new_command);
								if(send_Complete_code(name,new_command) != 0)
									return(COMPLETE_SCODE_ERR);
							}	else {
								return(INVALID_OLD_COMMAND);
							}						
						} else if(strcmp(model,"w") == 0){
							strcpy(ASR_COMMAND,"已设到送风模式");
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'w';
								char *strWind = strstr(old_command,"s");
								if(strWind[1] == '0')
									strWind[1] = '1';
								if(send_Complete_code(name,old_command) != 0)
									return(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){

									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//获取风量
									if(strstr(old_command,"s0") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(old_command,"s1") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(old_command,"s2") != NULL){
										strcpy(old_wind,"s2");
									}else if(strstr(old_command,"s3") != NULL){
										strcpy(old_wind,"s3");
									}

									//获取上下扫风
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","w","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,new_command) != 0)
										return(COMPLETE_SCODE_ERR);
												
							}	else {
									return(INVALID_OLD_COMMAND);
							}						
						} else if(strcmp(model,"h") == 0){

							strcpy(ASR_COMMAND,"已设到制热模式");
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'h';
								if(send_Complete_code(name,old_command) != 0)
									return(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
								
									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//获取风量
									if(strstr(old_command,"s0") != NULL){
										strcpy(old_wind,"s0");
									}else if(strstr(old_command,"s1") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(old_command,"s2") != NULL){
										strcpy(old_wind,"s2");
									}else if(strstr(old_command,"s3") != NULL){
										strcpy(old_wind,"s3");
									}

									//获取上下扫风
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","h","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,new_command) != 0)
										return(COMPLETE_SCODE_ERR);
								
								
							}	else {
								return(INVALID_OLD_COMMAND);
							}							
						} else if(strcmp(model,"r") == 0){
							strcpy(ASR_COMMAND,"已设到制冷模式");
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'r';
								if(send_Complete_code(name,old_command) != 0)
									return(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//获取风量
									if(strstr(old_command,"a") != NULL){
										if(strstr(old_command,"s0") != NULL){
											strcpy(old_wind,"s0");
										}else if(strstr(old_command,"s1") != NULL){
											strcpy(old_wind,"s1");
										}else if(strstr(old_command,"s2") != NULL){
											strcpy(old_wind,"s2");
										}else if(strstr(old_command,"s3") != NULL){
											strcpy(old_wind,"s3");
										}
									}else if(strstr(old_command,"d") != NULL){
										strcpy(old_wind,"s1");
									}

									//获取上下扫风
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//获取左右扫风
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,new_command) != 0)
										return(COMPLETE_SCODE_ERR);
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
						if (access(n_path,F_OK) != 0){
					  		  if(strstr(wind,"s0") != NULL){
							  		strcpy(ASR_COMMAND,"已设到自动风速");
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_","26","_","u0","_","l0","_","p0");
							  } else if(strstr(wind,"s1") != NULL){
							  	   strcpy(ASR_COMMAND,"已设到低速风");
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s1","_","26","_","u0","_","l0","_","p0");
							  } else if(strstr(wind,"s2") != NULL){
							 	   strcpy(ASR_COMMAND,"已设到中速风");
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s2","_","26","_","u0","_","l0","_","p0");
							  } else if(strstr(wind,"s3") != NULL){
								   strcpy(ASR_COMMAND,"已设到高速风");
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s3","_","26","_","u0","_","l0","_","p0");								  
							  } else {
								  return(INVALID_COMMAND);
							  }
							 
							  printf("new_command:%s ########\n",new_command);
							  if(send_Complete_code(name,new_command) != 0)
								  return(COMPLETE_SCODE_ERR);
						} else {
								//获取*.record 里面上一次命令 old_command
								fd = fopen(n_path,"r+");
								if(fd == NULL){
										printf("%s: open command.record error!!!!.\n",__func__);
										return(OPEN_FILE_ERR);
								}
								fread(old_command,50,1,fd);
								fclose(fd);
								//打印历史命令
								printf("old_command:%s	#####\n",old_command);

								//获取风量
								char *strWind2 = strstr(old_command,"s");
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
								} else{
									return(INVALID_OLD_COMMAND);
								}
								
								if(strstr(old_command,"d") != NULL){
										strWind2[1] = '1';
								} else if(strstr(old_command,"w") != NULL){
										if(wind[1] == '0')
											strWind2[1] = '1';
										else 
											strWind2[1] = wind[1];
								} else {
									strWind2[1] = wind[1];
								}

								printf("new_command:%s ########\n",old_command);
								if(send_Complete_code(name,old_command) != 0)
										return(COMPLETE_SCODE_ERR);

								
						}

				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") != 0){
					  //获取第三级: 温度
				  int temp = atoi(temperature);
				  if(temp > 15 && temp < 31){
				  	  strcpy(ASR_COMMAND,"已设到");
					  strcat(ASR_COMMAND,temperature);
					  strcat(ASR_COMMAND,"度");
					  //如果历史命令不存在
					  if (access(n_path,F_OK) != 0){  
							  sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_",temperature,"_","u0","_","l0","_","p0");
							  printf("new_command:%s ########\n",new_command);
							  
							  if(send_Complete_code(name,new_command) != 0)
								  return(COMPLETE_SCODE_ERR);
					  } else {
							  //获取*.record 里面上一次命令 old_command
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  return(OPEN_FILE_ERR);
							  }
							  fread(old_command,50,1,fd);
							  fclose(fd);
							  //打印历史命令
							  printf("old_command:%s  #####\n",old_command);
					  
							 
					  		  //获取历史命令中的模式
					  		  if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
									old_command[5] = temperature[0];
									old_command[6] = temperature[1];
									
									printf("new_command:%s ########\n",old_command);
									if(send_Complete_code(name,old_command) != 0)
										return(COMPLETE_SCODE_ERR);		
									
							  } else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){

							 		strcpy(old_sleep,"p0");
							  		strcpy(old_temperature,temperature);
							  		//获取风量
									if(strstr(old_command,"a") != NULL){
										if(strstr(old_command,"s0") != NULL){
											strcpy(old_wind,"s0");
										}else if(strstr(old_command,"s1") != NULL){
											strcpy(old_wind,"s1");
										}else if(strstr(old_command,"s2") != NULL){
											strcpy(old_wind,"s2");
										}else if(strstr(old_command,"s3") != NULL){
											strcpy(old_wind,"s3");
										}
									}else if(strstr(old_command,"d") != NULL){
										strcpy(old_wind,"s1");
									}

							  
							  		//获取上下扫风
							  		if(strstr(old_command,"u0") != NULL){
								  		strcpy(old_upsweep,"u0");
							  		}else if(strstr(old_command,"u1") != NULL){
								  		strcpy(old_upsweep,"u1");
							  		}
							  
							  		//获取左右扫风
							  		if(strstr(old_command,"l0") != NULL){
								 		 strcpy(old_lrsweep,"l0");
							  		}else if(strstr(old_command,"l1") != NULL){
								  		strcpy(old_lrsweep,"l1");
							 		}
							  
							 		 sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
							 		 printf("new_command:%s ########\n",new_command);
							 		 if(send_Complete_code(name,new_command) != 0)
								  		return(COMPLETE_SCODE_ERR);
							  }
							  
					  }
					}else if(temp > 50 && temp < 60) { 
							//温度升高
 							//如果历史命令不存在
							  if (access(n_path,F_OK) != 0){
									  strcpy(ASR_COMMAND,"已设到26度");
									  sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_","26","_","u0","_","l0","_","p0");
									  printf("new_command:%s ########\n",new_command);
									  
									  if(send_Complete_code(name,new_command) != 0)
										  return(COMPLETE_SCODE_ERR);
							  } else {
									  //获取*.record 里面上一次命令 old_command
									  fd = fopen(n_path,"r+");
									  if(fd == NULL){
											  printf("%s: open command.record error!!!!.\n",__func__);
											  return(OPEN_FILE_ERR);
									  }
									  fread(old_command,50,1,fd);
									  fclose(fd);
									  //打印历史命令
									  printf("old_command:%s  #####\n",old_command);
							  
									 
							  		  //获取历史命令中的模式
							  		  if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
											int t = atoi(old_command + 5);
											if((t + (temp%10))<31){
												//t += 1;
												memset(temperature,0,10);
												sprintf(temperature,"%d",(t + (temp%10)));
												old_command[5] = temperature[0];
												old_command[6] = temperature[1];	

											} else if((t + (temp%10)) > 30){
												memset(temperature,0,10);
												sprintf(temperature,"%d",30);
												old_command[5] = temperature[0];
												old_command[6] = temperature[1];											
											} else {
												return(INVALID_COMMAND);
											}
										  	  strcpy(ASR_COMMAND,"已设到");
											  strcat(ASR_COMMAND,temperature);
											  strcat(ASR_COMMAND,"度");											
											printf("new_command:%s ########\n",old_command);
											if(send_Complete_code(name,old_command) != 0)
												return(COMPLETE_SCODE_ERR);		
											
									  } else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){

									 		strcpy(old_sleep,"p0");
									  		strcpy(old_temperature,"26");
											strcpy(ASR_COMMAND,"已设到26度");
									  		//获取风量
											if(strstr(old_command,"a") != NULL){
												if(strstr(old_command,"s0") != NULL){
													strcpy(old_wind,"s0");
												}else if(strstr(old_command,"s1") != NULL){
													strcpy(old_wind,"s1");
												}else if(strstr(old_command,"s2") != NULL){
													strcpy(old_wind,"s2");
												}else if(strstr(old_command,"s3") != NULL){
													strcpy(old_wind,"s3");
												}
											}else if(strstr(old_command,"d") != NULL){
												strcpy(old_wind,"s1");
											}

									  
									  		//获取上下扫风
									  		if(strstr(old_command,"u0") != NULL){
										  		strcpy(old_upsweep,"u0");
									  		}else if(strstr(old_command,"u1") != NULL){
										  		strcpy(old_upsweep,"u1");
									  		}
									  
									  		//获取左右扫风
									  		if(strstr(old_command,"l0") != NULL){
										 		 strcpy(old_lrsweep,"l0");
									  		}else if(strstr(old_command,"l1") != NULL){
										  		strcpy(old_lrsweep,"l1");
									 		}
									  
									 		 sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									 		 printf("new_command:%s ########\n",new_command);
									 		 if(send_Complete_code(name,new_command) != 0)
										  		return(COMPLETE_SCODE_ERR);
									  }
									  
							  }							
					}else if(temp > 40 && temp < 50) { 	
							//温度降低
 							//如果历史命令不存在
							  if (access(n_path,F_OK) != 0){
									  strcpy(ASR_COMMAND,"已设到26度");							  	
									  sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_","26","_","u0","_","l0","_","p0");
									  printf("new_command:%s ########\n",new_command);
									  
									  if(send_Complete_code(name,new_command) != 0)
										  return(COMPLETE_SCODE_ERR);
							  } else {
									  //获取*.record 里面上一次命令 old_command
									  fd = fopen(n_path,"r+");
									  if(fd == NULL){
											  printf("%s: open command.record error!!!!.\n",__func__);
											  return(OPEN_FILE_ERR);
									  }
									  fread(old_command,50,1,fd);
									  fclose(fd);
									  //打印历史命令
									  printf("old_command:%s  #####\n",old_command);
							  
									 
							  		  //获取历史命令中的模式
							  		  if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
											int t = atoi(old_command + 5);
											if((t - (temp%10))>15){
												//t -= 1;
												memset(temperature,0,10);
												sprintf(temperature,"%d",(t - (temp%10)));
												old_command[5] = temperature[0];
												old_command[6] = temperature[1];
											} else if((t - (temp%10)) < 16){
												memset(temperature,0,10);
												sprintf(temperature,"%d",16);
												old_command[5] = temperature[0];
												old_command[6] = temperature[1];
											} else {
												return(INVALID_COMMAND);

											}
										  	  strcpy(ASR_COMMAND,"已设到");
											  strcat(ASR_COMMAND,temperature);
											  strcat(ASR_COMMAND,"度");												
											printf("new_command:%s ########\n",old_command);
											if(send_Complete_code(name,old_command) != 0)
												return(COMPLETE_SCODE_ERR);		
											
									  } else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){

									 		strcpy(old_sleep,"p0");
									  		strcpy(old_temperature,"26");
									  		//获取风量
											if(strstr(old_command,"a") != NULL){
												if(strstr(old_command,"s0") != NULL){
													strcpy(old_wind,"s0");
												}else if(strstr(old_command,"s1") != NULL){
													strcpy(old_wind,"s1");
												}else if(strstr(old_command,"s2") != NULL){
													strcpy(old_wind,"s2");
												}else if(strstr(old_command,"s3") != NULL){
													strcpy(old_wind,"s3");
												}
											}else if(strstr(old_command,"d") != NULL){
												strcpy(old_wind,"s1");
											}

									  
									  		//获取上下扫风
									  		if(strstr(old_command,"u0") != NULL){
										  		strcpy(old_upsweep,"u0");
									  		}else if(strstr(old_command,"u1") != NULL){
										  		strcpy(old_upsweep,"u1");
									  		}
									  
									  		//获取左右扫风
									  		if(strstr(old_command,"l0") != NULL){
										 		 strcpy(old_lrsweep,"l0");
									  		}else if(strstr(old_command,"l1") != NULL){
										  		strcpy(old_lrsweep,"l1");
									 		}
									   		 strcpy(ASR_COMMAND,"已设到26度");	
									 		 sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									 		 printf("new_command:%s ########\n",new_command);
									 		 if(send_Complete_code(name,new_command) != 0)
										  		return(COMPLETE_SCODE_ERR);
									  }
									  
							  }							
				  	}else {
						return(INVALID_COMMAND);
					}


				}

				  //获取第四级: 扫风
				  else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") != 0){
					  //如果历史命令不存在
					  if (access(n_path,F_OK) != 0){
					  		  if(strstr(sweep,"u0") != NULL){
							  		 strcpy(ASR_COMMAND,"已关闭扫风");	
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_",temperature,"_","u0","_","l0","_","p0");

							  } else if(strstr(sweep,"u1") != NULL){
							 	 strcpy(ASR_COMMAND,"已开启扫风");	
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_",temperature,"_","u1","_","l1","_","p0");
							  } else {
								  return(INVALID_COMMAND);
							  }
							 
							  printf("new_command:%s ########\n",new_command);
							  if(send_Complete_code(name,new_command) != 0)
								  return(COMPLETE_SCODE_ERR);
					  } else{
							  //获取*.record 里面上一次命令 old_command
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  return(OPEN_FILE_ERR);
							  }
							  fread(old_command,50,1,fd);
							  fclose(fd);
							  //打印历史命令
							  printf("old_command:%s  #####\n",old_command);	

							  if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
							  			if(strstr(sweep,"u0") != NULL){
											strcpy(ASR_COMMAND,"已关闭扫风");	
												old_command[9] = '0';
												old_command[12] = '0';
										} else if(strstr(sweep,"u1") != NULL){
										strcpy(ASR_COMMAND,"已开启扫风");	
											old_command[9] = '1';
											old_command[12] = '1';

										} else {
											return(INVALID_COMMAND);
										}

							 			 printf("new_command:%s ########\n",old_command);
							 			 if(send_Complete_code(name,old_command) != 0)
								 			 return(COMPLETE_SCODE_ERR);										

							  } else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL ){
								 		 if(strstr(sweep,"u0") != NULL){
										 	strcpy(ASR_COMMAND,"已关闭扫风");	
										  	old_command[7] = '0';
										 	 old_command[10] = '0';
								 	 	} else if(strstr(sweep,"u1") != NULL){
								 	 	strcpy(ASR_COMMAND,"已开启扫风");	
									  		old_command[7] = '1';
									  		old_command[10] = '1';
								  
								 		 } else {
									  		return(INVALID_COMMAND);
								  		}
								  
								   		printf("new_command:%s ########\n",old_command);
								   		if(send_Complete_code(name,old_command) != 0)
									   		return(COMPLETE_SCODE_ERR); 

							  } else {
										return(INVALID_OLD_COMMAND);
							  }

					  }					

				} 
				  //没有设置参数
				  else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") == 0){
					  //如果历史命令不存在
					  strcpy(ASR_COMMAND,"已打开空调");	
					  if (access(n_path,F_OK) != 0){
							  printf("new_command:%s ########\n","r_s0_26_u0_l0_p0");
							  if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								  return(COMPLETE_SCODE_ERR);
					  } else{

					  		//获取*.record 里面上一次命令 old_command
					 		fd = fopen(n_path,"r+");
					 		if(fd == NULL){
							  	printf("%s: open command.record error!!!!.\n",__func__);
							 	return(OPEN_FILE_ERR);
					  		}
					  		fread(old_command,50,1,fd);
					  		fclose(fd);
					  		//打印历史命令

							if(old_command[0] != 'a' && old_command[0] != 'd' && old_command[0] != 'w' && old_command[0] != 'r' && old_command[0] != 'h'){
								 printf("new_command:%s ########\n","r_s0_26_u0_l0_p0");
							 	 if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								  	return(COMPLETE_SCODE_ERR);
							} else {
								printf("new_command:%s ########\n",old_command);
								if(send_Complete_code(name,old_command) != 0)
									return(COMPLETE_SCODE_ERR);
							}


					  }


				} else {
					//其他命令无效
					return(INVALID_COMMAND);
				}
			
		} else if(strcmp(power,"CLOSE") == 0){
			strcpy(ASR_COMMAND,"已关闭空调");	
			if(send_Complete_code(name,"off") != 0)
					return(COMPLETE_SCODE_ERR);
		} else {
			//其他命令无效
					return(INVALID_COMMAND);	
		}
		
		/***********************************************
				    上面是码库v3版本的解析	
		************************************************/

	} 

	return 0;
}



//匹配模式
int match_handler_func(char *token)
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
		
		if(get_Complete_code(list) != 0)
			return(COMPLETE_GCODE_ERR);
	} else {
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
		
		//second  获取激活码 保存在本地
		if(get_Activation_code() != 0)
			return(ACTIVATION_CODE_ERR);
	
	
		//获取匹配码库
		if(get_Match_code(bid,t,"4") != 0)
			return(MATCH_GCODE_ERR);
		//发送测试码
		if(send_Match_code(atoi(t),atoi(number),src) !=0)
			return(MATCH_SCODE_ERR);
		
	}


	return 0;
}


//app 使用码库
int use_handler_func(char *token)
{
	//APP使用 t:电器类型 name:码库名字 appCommand:获取命令
	char t[10] = {0};		
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(t,token);
	
	char app_name[50] = {0};
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
	if(strcmp(t,"7") == 0 || strcmp(t,"1") == 0 || strcmp(t,"2") == 0 || strcmp(t,"6") == 0){
			printf("APPUSE:  app_name :%s  appCommand:%s \n",app_name,appCommand);
			if(send_Complete_code(app_name,appCommand) != 0)
				return(COMPLETE_SCODE_ERR);		
	}else if(strcmp(t,"99") == 0){
			if(atoi(appCommand) >= 0 && atoi(appCommand) <= 16){
				printf("SEND LEARN DATA:  app_name :%s  appCommand:%s \n",app_name,appCommand);
				if(SEND_IR_LEARN(app_name,atoi(appCommand)) != 0)
					return(SEND_IR_LEARN_ERR);
		}
	} else if(strcmp(t,"51") == 0){
		if(strcmp(appCommand,"0") == 0){
			//关夜灯
			bngo_led_close("PB15");
			return 0;
		} else if(strcmp(appCommand,"1") == 0){
			bngo_led_open("PB15");
		} else if(strcmp(appCommand,"2") == 0){
			bngo_led_open("PB15");
		} else if(strcmp(appCommand,"3") == 0){
			bngo_led_open("PB15");
		} else if(strcmp(appCommand,"4") == 0){
			bngo_led_open("PB15");
		} else if(strcmp(appCommand,"5") == 0){
			bngo_led_open("PB15");
		} else {
			return(INVALID_COMMAND);
		}

		FILE *fd;
		/*************记录历史命令*******************/
		fd = fopen("/usr/fs/usr/yuan/bngoLed.record","w+");
		if(fd == NULL){
			printf("%s: open record error!!!!.\n",__func__);
			return(INVALID_COMMAND);
		}
		fwrite(appCommand,strlen(appCommand),1,fd);
		fclose(fd);
		
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
	
	printf("t:%s  name:%s  key:%s  ####\n",l_t,l_name,l_key);
	
	if(strcmp(l_t,"OK") == 0){
		FILE *learnFd;
		//生成文件路径
		char l_path[50] = {0};
		sprintf(l_path,"%s%s%s","/usr/fs/usr/yuan/",l_name,".txt");
		//如果文件不存在
		if (access(l_path,F_OK) != 0){
				//判断键值是否有效
				if(k < 0 || k > 16)
					return(INVALID_COMMAND);
	
				//判断学习配置文件是否为空
				if (access("/usr/fs/usr/yuan/learn.txt",F_OK) != 0)
					return(LEARN_PATH_EMPTY);
				
				//读取测试成功的学习数据		
				learnFd = fopen("/usr/fs/usr/yuan/learn.txt","r+");
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
				if (access("/usr/fs/usr/yuan/learn.txt",F_OK) != 0)
					return(LEARN_PATH_EMPTY);
			
				//读取测试成功的学习数据
			
				learnFd = fopen("/usr/fs/usr/yuan/learn.txt","r+");
				if(learnFd == NULL){
					printf("open secret_key.txt error!!!!.\n");
					return(OPEN_FILE_ERR);
				}
				fread(&l_data,sizeof(l_data),1,learnFd);
				fclose(learnFd);
			
			
				//将学习到的数据写入
				learnFd = fopen(l_path,"r+");
				if(learnFd == NULL){
					printf("%s: open Complete_code.txt error!!!!.\n",__func__);
					return(OPEN_FILE_ERR);
				}
			
				fseek(learnFd,k*sizeof(l_data),SEEK_SET);
				fwrite(&l_data,sizeof(l_data),1,learnFd);	
				fclose(learnFd);
		}
		
	
	} else if(atoi(l_key) >=0 && atoi(l_key) < 17) {
		//进入学习模式，同时将学习到的数据保存到本地 红灯亮
		bngo_led_open("PA10");
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
	
	char name[60] = "格力333";
#if 0
	if(strcmp(t,"51") != 0){
			//获取语音控制码库路径
			char t_path[50] = {0};
			sprintf(t_path,"%s%s%s","/usr/fs/usr/yuan/",t,".txt");
			
			//判断语音控制码库不存在，退出,需要先匹配码库
			if (access(t_path,F_OK) != 0)
				return(CODE_NOT_EXIST);
			
			//获取语音控制码库名称
			//char name[60] = {0};
			
			FILE *fd;
			fd = fopen(t_path,"r+");
			if(fd == NULL){
				printf("%s: open Complete_code.txt error!!!!.\n",__func__);
				return(OPEN_FILE_ERR);
			}
			fread(str_v,10,1,fd);
			fread(name,60,1,fd);
			fclose(fd);
		}else {
			strcpy(name,"bngoLed");
		}
#endif		
	//生成上一次红外历史命令路径
	char n_path[50] = {0};
	sprintf(n_path,"%s%s%s","/usr/fs/usr/yuan/",name,".record");

	/* air_handler_func();
	 *	v: 码库的版本  name: 码库名称  n_path: 码库历史命令路径，token: 接受到的指令 
	 */
	int ret;
	if(strcmp(t,"7") == 0){ 
		ret = air_handler_func("3",name,n_path,token,ASR_COMMAND);			
	} else {
		return INVALID_COMMAND;
	}

	/*
	if(strcmp(t,"1") == 0){
		ret = box_handler_func(str_v,name,n_path,token);
	}else if(strcmp(t,"2") == 0){ 
		ret = tv_handler_func(str_v,name,n_path,token);		
	}else if(strcmp(t,"7") == 0){ 
		ret = air_handler_func(str_v,name,n_path,token);			
	}else if(strcmp(t,"6") == 0){ 
		ret = fan_handler_func(str_v,name,n_path,token);			
	}else if(strcmp(t,"51") == 0){ 
		ret = led_handler_func(n_path,token);			
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


	char file[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(file,token);

	//生成上一次红外历史命令路径
	char d_path[50] = {0};
	char d_record[50] = {0};
	sprintf(d_path,"%s%s%s","/usr/fs/usr/yuan/",file,".txt");	
	sprintf(d_record,"%s%s%s","/usr/fs/usr/yuan/",file,".record");

	if(access(d_path,F_OK) == 0){
		remove(d_path);
	}
	if(access(d_record,F_OK) == 0){
		remove(d_record);
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


	char file[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(file,token);

	//文件名路径
	char d_path[50] = {0};
	sprintf(d_path,"%s%s%s","/usr/fs/usr/yuan/",file,".txt");	

	rename("/usr/fs/usr/yuan/bngoIR.txt", d_path);

	//判断rid:遥控id是否存在
	if (access("/usr/fs/usr/yuan/rid.txt",F_OK) != 0)
		return -1;
	
	//读取rid, c_path:唯一的码库名字
	char rid[128] ={0};
	char h_path[60] ={0};
	char str_v[10] = {0};
	FILE *fd;
	fd = fopen("/usr/fs/usr/yuan/rid.txt","r+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fread(rid,128,1,fd);
	fread(str_v,10,1,fd);
	fread(h_path,60,1,fd);
	fclose(fd);

	if(strcmp(file,h_path) != 0)
		return INVALID_COMMAND;

	/**************语音控制配置文件*****************/
	char asr_path[60] ={0};
	sprintf(asr_path,"%s%s%s","/usr/fs/usr/yuan/",t,".txt");

	if (access(asr_path,F_OK) != 0){
		fd = fopen(asr_path,"w+");
		if(fd == NULL){
			printf("open asr path.txt error!!!!.\n");
			return OPEN_FILE_ERR;
		
		}
		fwrite(str_v,10,1,fd);
		fwrite(h_path,strlen(h_path)+1,1,fd);
		fclose(fd);		
	}

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

	char str_v[10] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(str_v,token);

	if(strcmp(str_v,"0") == 0)
		return(INVALID_COMMAND);

	char file[50] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(file,token);

	//默认遥控器路径
	char d_path[50] = {0};
	sprintf(d_path,"%s%s%s","/usr/fs/usr/yuan/",t,".txt");	

	//判断rid:遥控id是否存在
	if (access(d_path,F_OK) != 0)
		return SETIR_FILE_ERR;

	FILE *fd;
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


