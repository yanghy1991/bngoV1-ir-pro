#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>



#include "cacertinmem.h"
#include "bngo_ir.h"
#include "bngo_ir_learn.h"

//����ѧϰ���ܵ�����
struct learn_data {
	unsigned int count;
	unsigned int times[512];
};


int bngo_ir(char *device)
{
	int status;
	pid_t fpid;
	fpid = fork();
	//�����ӽ��̣�����Ϊʵʱ����
	if(fpid == 0){
	//����Ϊʵʱ����
	int i;
	
	struct sched_param param; 
	int maxpri; 
	maxpri = sched_get_priority_max(SCHED_FIFO);
	if(maxpri == -1) 
	{ 
		perror("sched_get_priority_max() failed"); 
		exit(1); 
	} 
		param.sched_priority = maxpri; 
	if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) //�������ȼ�
	{ 
		perror("sched_setscheduler() failed"); 
		exit(1); 
	} 

	//first ��ȡ�豸id
	if(get_secret_key() != 0)
		exit(SECRET_KEY_ERR);
	
	/*******����bngo���ܵ���json����********/
	char *token;	
	token = strtok(device,"-");
	//������Դ����ΪSERVER MATCH USE LEARN
	char dataSource[10] = {0};
	strcpy(dataSource,token);
	
	if(strcmp(dataSource,"SERVER") == 0){
		//��ȡ��������
		char t[10] = {0};
		token = strtok( NULL,"-");
		if(token == NULL)
			exit(INVALID_COMMAND);
		strcpy(t,token);
		
		//��ȡ�����������·��
		char t_path[50] = {0};
		sprintf(t_path,"%s%s%s","/usr/fs/usr/yuan/",t,".txt");
		
		//�ж�����������ⲻ���ڣ��˳�,��Ҫ��ƥ�����
		if (access(t_path,F_OK) != 0)
			exit(CODE_NOT_EXIST);

		//��ȡ���������������
		char name[50] = {0};
		FILE *fd;
		fd = fopen(t_path,"r+");
		if(fd == NULL){
			printf("%s: open Complete_code.txt error!!!!.\n",__func__);
			exit(OPEN_FILE_ERR);
		}
		fread(name,50,1,fd);
		fclose(fd);

		//������һ�κ�����ʷ����·��
		char n_path[50] = {0};
		sprintf(n_path,"%s%s%s","/usr/fs/usr/yuan/",name,".record");

		/* air_handler_func();
		 *	v: ���İ汾  name: �������  n_path: �����ʷ����·����token: ���ܵ���ָ�� 
		 */
		if(strcmp(t,"7") == 0){
			exit(air_handler_func("3",name,n_path,token));			
		}
		///////////////////////////////////////////////////////////
		#if 0
		//power: open����close
		char power[10] = {0};
		token = strtok( NULL,"-");
		strcpy(power,token);
		if(strcmp(power,"OPEN") == 0){
			//#############�����յ�����#################
			if(strcmp(t,"7") == 0){
				//������������������: SERVER-7-OPEN-a-NO-NO-NO
				char model[5] = {0};
				char wind[5] = {0};
				char temperature[5] = {0};
				char sweep[5] = {0};
				//*.record �е���ʷ����
				char old_model[10] = {0};
				char old_wind[10] = {0};
				char old_temperature[10] = {0};
				char old_lrsweep[10] = {0};
				char old_upsweep[10] = {0};
				char old_sleep[10] = {0};
				//������
				char new_command[50] = {0};
				//��ȡ��ʷ�����ʷ�������
				char old_command[50] = {0};
				//��ʷ����ָ�
				char *old_token = NULL;	

				i = 0;
	  		 	while( token != NULL )
	  		 	{
	  		 		token = strtok( NULL,"-");
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
					exit(INVALID_COMMAND);

				printf("model:%s wind:%s temperature:%s sweep:%s \n",model,wind,temperature,sweep);
				//��ȡ��һ��: ģʽ
				if(strcmp(model,"NO") != 0){
					//��ȡ��һ����������
					if (access(n_path,F_OK) != 0){
						if(strcmp(model,"a") == 0){
							if(send_Complete_code(name,"a_s0__u0_l0_p0") != 0)
								exit(COMPLETE_SCODE_ERR);	
						} else if(strcmp(model,"d") == 0){
							if(send_Complete_code(name,"d_s0__u0_l0_p0") != 0)
								exit(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"w") == 0){
							if(send_Complete_code(name,"w_s0_26_u0_l0_p0") != 0)
								exit(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"h") == 0){
							if(send_Complete_code(name,"h_s0_26_u0_l0_p0") != 0)
								exit(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"r") == 0){
							if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								exit(COMPLETE_SCODE_ERR);
						} else {
								exit(INVALID_COMMAND);
						}
					
					} else {
						//��ȡ*.record ������һ������ old_command
						fd = fopen(n_path,"r+");
						if(fd == NULL){
							printf("%s: open command.record error!!!!.\n",__func__);
							exit(OPEN_FILE_ERR);
						}
						fread(old_command,50,1,fd);
						fclose(fd);
						//��ӡ��ʷ����
						printf("old_command:%s  #####\n",old_command);

						//�ж��Ƿ��Զ�ģʽ
						if(strcmp(model,"a") == 0){
							if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
								old_command[0] = 'a';
								if(send_Complete_code(name,old_command) != 0)
									exit(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL){
								old_token = strtok(old_command,"_");
								if(old_token == NULL)
									exit(INVALID_OLD_COMMAND);
									
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
									exit(INVALID_OLD_COMMAND);
								
								sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s","a","_",old_wind,"_","_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
								printf("new_command:%s ########\n",new_command);
								if(send_Complete_code(name,new_command) != 0)
									exit(COMPLETE_SCODE_ERR);
							}else {
								exit(INVALID_OLD_COMMAND);

							}
											
						} else if(strcmp(model,"d") == 0){
							if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
									//��ʪģʽ������ֻ��s1
									//��ȡ����ɨ��
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//��ȡ����ɨ��
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}		

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s","d","_","s1","_","_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,"d_s1__u0_l0_p0") != 0)
										exit(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL){
								old_token = strtok(old_command,"_");
								if(old_token == NULL)
									exit(INVALID_OLD_COMMAND);
									
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
									exit(INVALID_OLD_COMMAND);
								
								sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s","d","_","s1","_","_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
								printf("new_command:%s ########\n",new_command);
								if(send_Complete_code(name,new_command) != 0)
									exit(COMPLETE_SCODE_ERR);
							}	else {
								exit(INVALID_OLD_COMMAND);
							}						
						} else if(strcmp(model,"w") == 0){
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'w';
								char *strWind = strstr(old_command,"s");
								if(strWind[1] == '0')
									strWind[1] = '1';
								if(send_Complete_code(name,old_command) != 0)
									exit(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){

									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//��ȡ����
									if(strstr(old_command,"s0") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(old_command,"s1") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(old_command,"s2") != NULL){
										strcpy(old_wind,"s2");
									}else if(strstr(old_command,"s3") != NULL){
										strcpy(old_wind,"s3");
									}

									//��ȡ����ɨ��
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//��ȡ����ɨ��
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","w","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,new_command) != 0)
										exit(COMPLETE_SCODE_ERR);
												
							}	else {
									exit(INVALID_OLD_COMMAND);
							}						
						} else if(strcmp(model,"h") == 0){
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'h';
								if(send_Complete_code(name,old_command) != 0)
									exit(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
								
									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//��ȡ����
									if(strstr(old_command,"s0") != NULL){
										strcpy(old_wind,"s0");
									}else if(strstr(old_command,"s1") != NULL){
										strcpy(old_wind,"s1");
									}else if(strstr(old_command,"s2") != NULL){
										strcpy(old_wind,"s2");
									}else if(strstr(old_command,"s3") != NULL){
										strcpy(old_wind,"s3");
									}

									//��ȡ����ɨ��
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//��ȡ����ɨ��
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","h","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,new_command) != 0)
										exit(COMPLETE_SCODE_ERR);
								
								
							}	else {
								exit(INVALID_OLD_COMMAND);
							}							
						} else if(strcmp(model,"r") == 0){
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'r';
								if(send_Complete_code(name,old_command) != 0)
									exit(COMPLETE_SCODE_ERR);
							} else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){
									strcpy(old_sleep,"p0");
									strcpy(old_temperature,"26");
									//��ȡ����
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

									//��ȡ����ɨ��
									if(strstr(old_command,"u0") != NULL){
										strcpy(old_upsweep,"u0");
									}else if(strstr(old_command,"u1") != NULL){
										strcpy(old_upsweep,"u1");
									}

									//��ȡ����ɨ��
									if(strstr(old_command,"l0") != NULL){
										strcpy(old_lrsweep,"l0");
									}else if(strstr(old_command,"l1") != NULL){
										strcpy(old_lrsweep,"l1");
									}

									sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
									printf("new_command:%s ########\n",new_command);
									if(send_Complete_code(name,new_command) != 0)
										exit(COMPLETE_SCODE_ERR);
								} else {
									exit(INVALID_OLD_COMMAND);								
								}															
						}else{
							exit(INVALID_COMMAND);
						}

					}
				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") != 0){
					  	//��ȡ�ڶ���: ����
				  		//��ʷ������� *.record ���ͳ�ʼֵ
						if (access(n_path,F_OK) != 0){
					  		  if(strstr(wind,"s0") != NULL){
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_","26","_","u0","_","l0","_","p0");
							  } else if(strstr(wind,"s1") != NULL){
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s1","_","26","_","u0","_","l0","_","p0");
							  } else if(strstr(wind,"s2") != NULL){
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s2","_","26","_","u0","_","l0","_","p0");
							  } else if(strstr(wind,"s3") != NULL){
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s3","_","26","_","u0","_","l0","_","p0");								  
							  } else {
								  exit(INVALID_COMMAND);
							  }
							 
							  printf("new_command:%s ########\n",new_command);
							  if(send_Complete_code(name,new_command) != 0)
								  exit(COMPLETE_SCODE_ERR);
						} else {
								//��ȡ*.record ������һ������ old_command
								fd = fopen(n_path,"r+");
								if(fd == NULL){
										printf("%s: open command.record error!!!!.\n",__func__);
										exit(OPEN_FILE_ERR);
								}
								fread(old_command,50,1,fd);
								fclose(fd);
								//��ӡ��ʷ����
								printf("old_command:%s	#####\n",old_command);

								//��ȡ����
								char *strWind2 = strstr(old_command,"s");
								if(strWind2 == NULL)
									exit(INVALID_COMMAND);
								if(strstr(old_command,"d") != NULL){
										strWind2[1] = '1';
								} else if(strstr(old_command,"w") != NULL){
										if(wind[1] == '0')
											strWind2[1] = '1';
										else 
											strWind2[1] = wind[1];
								} else{
									strWind2[1] = wind[1];

								}

								printf("new_command:%s ########\n",old_command);
								if(send_Complete_code(name,old_command) != 0)
										exit(COMPLETE_SCODE_ERR);

								
						}

				} else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") != 0){
					  //��ȡ������: �¶�
				  int temp = atoi(temperature);
				  if(temp > 15 && temp < 31){
					  //�����ʷ�������
					  if (access(n_path,F_OK) != 0){
							  sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_",temperature,"_","u0","_","l0","_","p0");
							  printf("new_command:%s ########\n",new_command);
							  
							  if(send_Complete_code(name,new_command) != 0)
								  exit(COMPLETE_SCODE_ERR);
					  } else {
							  //��ȡ*.record ������һ������ old_command
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  exit(OPEN_FILE_ERR);
							  }
							  fread(old_command,50,1,fd);
							  fclose(fd);
							  //��ӡ��ʷ����
							  printf("old_command:%s  #####\n",old_command);
					  
							 
					  		  //��ȡ��ʷ�����е�ģʽ
					  		  if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
									old_command[5] = temperature[0];
									old_command[6] = temperature[1];
									
									printf("new_command:%s ########\n",old_command);
									if(send_Complete_code(name,old_command) != 0)
										exit(COMPLETE_SCODE_ERR);		
									
							  } else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL){

							 		strcpy(old_sleep,"p0");
							  		strcpy(old_temperature,temperature);
							  		//��ȡ����
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

							  
							  		//��ȡ����ɨ��
							  		if(strstr(old_command,"u0") != NULL){
								  		strcpy(old_upsweep,"u0");
							  		}else if(strstr(old_command,"u1") != NULL){
								  		strcpy(old_upsweep,"u1");
							  		}
							  
							  		//��ȡ����ɨ��
							  		if(strstr(old_command,"l0") != NULL){
								 		 strcpy(old_lrsweep,"l0");
							  		}else if(strstr(old_command,"l1") != NULL){
								  		strcpy(old_lrsweep,"l1");
							 		}
							  
							 		 sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_",old_wind,"_",old_temperature,"_",old_upsweep,"_",old_lrsweep,"_",old_sleep);
							 		 printf("new_command:%s ########\n",new_command);
							 		 if(send_Complete_code(name,new_command) != 0)
								  		exit(COMPLETE_SCODE_ERR);
							  }
							  
					  }
				  	}else {
						exit(INVALID_COMMAND);
					}


				}

				  //��ȡ���ļ�: ɨ��
				  else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") != 0){
					  //�����ʷ�������
					  if (access(n_path,F_OK) != 0){
					  		  if(strstr(sweep,"u0") != NULL){
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_",temperature,"_","u0","_","l0","_","p0");

							  } else if(strstr(sweep,"u1") != NULL){
								   sprintf(new_command,"%s%s%s%s%s%s%s%s%s%s%s","r","_","s0","_",temperature,"_","u1","_","l1","_","p0");
							  } else {
								  exit(INVALID_COMMAND);
							  }
							 
							  printf("new_command:%s ########\n",new_command);
							  if(send_Complete_code(name,new_command) != 0)
								  exit(COMPLETE_SCODE_ERR);
					  } else{
							  //��ȡ*.record ������һ������ old_command
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  exit(OPEN_FILE_ERR);
							  }
							  fread(old_command,50,1,fd);
							  fclose(fd);
							  //��ӡ��ʷ����
							  printf("old_command:%s  #####\n",old_command);	

							  if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
							  			if(strstr(sweep,"u0") != NULL){
												old_command[9] = '0';
												old_command[12] = '0';
										} else if(strstr(sweep,"u1") != NULL){
											old_command[9] = '1';
											old_command[12] = '1';

										} else {
											exit(INVALID_COMMAND);
										}

							 			 printf("new_command:%s ########\n",old_command);
							 			 if(send_Complete_code(name,old_command) != 0)
								 			 exit(COMPLETE_SCODE_ERR);										

							  } else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL ){
								 		 if(strstr(sweep,"u0") != NULL){
										  	old_command[7] = '0';
										 	 old_command[10] = '0';
								 	 	} else if(strstr(sweep,"u1") != NULL){
									  		old_command[7] = '1';
									  		old_command[10] = '1';
								  
								 		 } else {
									  		exit(INVALID_COMMAND);
								  		}
								  
								   		printf("new_command:%s ########\n",old_command);
								   		if(send_Complete_code(name,old_command) != 0)
									   		exit(COMPLETE_SCODE_ERR); 

							  } else {
										exit(INVALID_OLD_COMMAND);
							  }

					  }					

				} 
				  //û�����ò���
				  else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") == 0){
					  //�����ʷ�������
					  if (access(n_path,F_OK) != 0){
							  printf("new_command:%s ########\n","r_s0_26_u0_l0_p0");
							  if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								  exit(COMPLETE_SCODE_ERR);
					  } else{

					  		//��ȡ*.record ������һ������ old_command
					 		fd = fopen(n_path,"r+");
					 		if(fd == NULL){
							  	printf("%s: open command.record error!!!!.\n",__func__);
							 	exit(OPEN_FILE_ERR);
					  		}
					  		fread(old_command,50,1,fd);
					  		fclose(fd);
					  		//��ӡ��ʷ����
					
							printf("new_command:%s ########\n",old_command);
							if(send_Complete_code(name,old_command) != 0)
								exit(COMPLETE_SCODE_ERR);

					  }


				} else {
					//����������Ч
					exit(INVALID_COMMAND);
				}
				
			} else {

			}

		
			
		} else if(strcmp(power,"CLOSE") == 0){
			if(send_Complete_code(name,"off") != 0)
					exit(COMPLETE_SCODE_ERR);
		} else {
			//����������Ч
					exit(INVALID_COMMAND);	
		}
	#endif
	///////////////////////////////////////////////////////////////////////////////
		
		
	} else if(strcmp(dataSource,"MATCH") == 0){
		printf("###############  MATCH  ###########\n");
		//����ƥ��ģʽ ��ȡ��������
		char t[10] = {0};
		token = strtok( NULL,"-");
		strcpy(t,token);
		if(strcmp(t,"OK") == 0){
			//��ȡ�������
			char list[10] = {0};
			token = strtok( NULL,"-");
			strcpy(list,token);
			
			if(get_Complete_code(list) != 0)
				exit(COMPLETE_GCODE_ERR);
		} else {
			char bid[10] = {0};
			char number[10] = {0};
			char src[10] = {0};

			i = 0;
			while( token != NULL )
	  		{
	  		 	token = strtok( NULL,"-");
	   			if(i ==0)
					strcpy(bid,token);
				else if(i ==1)
					strcpy(number,token);
				else if(i ==2)
					strcpy(src,token);
		  		++i;
	  		}
			if(i != 4)
				exit(INVALID_COMMAND); //������Ч

			//printf("t:%d bid:%s number:%d  src:%s ########\n",atoi(t),bid,atoi(number),src);
			
			//second  ��ȡ������ �����ڱ���
			if(get_Activation_code() != 0)
				exit(ACTIVATION_CODE_ERR);


			//��ȡƥ�����
			if(get_Match_code(bid,t,"4") != 0)
				exit(MATCH_GCODE_ERR);
			//���Ͳ�����
			if(send_Match_code(atoi(t),atoi(number),src) !=0)
				exit(MATCH_SCODE_ERR);
			
		}
	
	} else if(strcmp(dataSource,"USE") == 0){
		printf("###############  USE  ###########\n");
		//APPʹ�� t:�������� name:������� appCommand:��ȡ����
		char t[10] = {0};		
		token = strtok( NULL,"-");
		if(token == NULL)
			exit(INVALID_COMMAND);
		strcpy(t,token);

		char app_name[50] = {0};
		token = strtok( NULL,"-");
		if(token == NULL)
			exit(INVALID_COMMAND);
		strcpy(app_name,token);
		
		char appCommand[50] = {0};
		token = strtok( NULL,"-");
		if(token == NULL)
			exit(INVALID_COMMAND);
		strcpy(appCommand,token);

		if(strstr(appCommand,"w") != NULL || strstr(appCommand,"h") != NULL || strstr(appCommand,"r") != NULL || strstr(appCommand,"a") != NULL || strstr(appCommand,"d") != NULL || strstr(appCommand,"off") != NULL){
				//�����������
				printf("APPUSE:  app_name :%s  appCommand:%s \n",app_name,appCommand);
				if(send_Complete_code(app_name,appCommand) != 0)
					exit(COMPLETE_SCODE_ERR);

		} else if(atoi(appCommand) >= 0 && atoi(appCommand) <= 16){
				//����ѧϰ����
				if(SEND_IR_LEARN(app_name,atoi(appCommand)) != 0)
					exit(SEND_IR_LEARN_ERR);
				
		} else {
				exit(INVALID_COMMAND);
		}
		

	} else if(strcmp(dataSource,"LEARN") == 0){
		printf("############## LEARN #############\n");

		//��ȡ��������
		char l_t[10] = {0};
		//��ȡѧϰģʽ�ļ���
		char l_name[20] = {0};
		//��ȡ��ֵ
		char l_key[10] = {0};
		//��ȡ�Ѿ����Թ���ѧϰ����
		struct learn_data l_data;
		memset(&l_data,0,sizeof(struct learn_data));
		
		token = strtok( NULL,"-");
		if(token == NULL)
			exit(INVALID_COMMAND);
		strcpy(l_t,token);	
		//��ȡ�ļ���
		token = strtok( NULL,"-");
		if(token == NULL)
			exit(INVALID_COMMAND);
		strcpy(l_name,token);
		//��ȡ��ֵ	
		token = strtok( NULL,"-");
		if(token == NULL)
			exit(INVALID_COMMAND);
		strcpy(l_key,token);
		int k =atoi(l_key);

		printf("t:%s  name:%s  key:%s  ####\n",l_t,l_name,l_key);
		
		if(strcmp(l_t,"OK") == 0){
			FILE *learnFd;
			//�����ļ�·��
			char l_path[50] = {0};
			sprintf(l_path,"%s%s%s","/usr/fs/usr/yuan/",l_name,".txt");
			//����ļ�������
			if (access(l_path,F_OK) != 0){
					//�жϼ�ֵ�Ƿ���Ч
					if(k < 0 || k > 16)
						exit(INVALID_COMMAND);

					//�ж�ѧϰ�����ļ��Ƿ�Ϊ��
					if (access("/usr/fs/usr/yuan/learn.txt",F_OK) != 0)
						exit(LEARN_PATH_EMPTY);
					
					//��ȡ���Գɹ���ѧϰ����		
					learnFd = fopen("/usr/fs/usr/yuan/learn.txt","r+");
					if(learnFd == NULL){
						printf("open secret_key.txt error!!!!.\n");
						exit(OPEN_FILE_ERR);
					}
					fread(&l_data,sizeof(l_data),1,learnFd);
					fclose(learnFd);

					
					//��ѧϰ��������д��
					learnFd = fopen(l_path,"w+");
					if(learnFd == NULL){
						printf("%s: open Complete_code.txt error!!!!.\n",__func__);
						exit(OPEN_FILE_ERR);
					}
					
					fseek(learnFd,k*sizeof(l_data),SEEK_SET);
					fwrite(&l_data,sizeof(l_data),1,learnFd);
					fclose(learnFd);				
			} else {
					//�жϼ�ֵ�Ƿ���Ч
					if(k < 0 || k > 16)
						exit(INVALID_COMMAND);
				
					//�ж�ѧϰ�����ļ��Ƿ�Ϊ��
					if (access("/usr/fs/usr/yuan/learn.txt",F_OK) != 0)
						exit(LEARN_PATH_EMPTY);
				
					//��ȡ���Գɹ���ѧϰ����
				
					learnFd = fopen("/usr/fs/usr/yuan/learn.txt","r+");
					if(learnFd == NULL){
						printf("open secret_key.txt error!!!!.\n");
						exit(OPEN_FILE_ERR);
					}
					fread(&l_data,sizeof(l_data),1,learnFd);
					fclose(learnFd);
				
				
					//��ѧϰ��������д��
					learnFd = fopen(l_path,"r+");
					if(learnFd == NULL){
						printf("%s: open Complete_code.txt error!!!!.\n",__func__);
						exit(OPEN_FILE_ERR);
					}
				
					fseek(learnFd,k*sizeof(l_data),SEEK_SET);
					fwrite(&l_data,sizeof(l_data),1,learnFd);	
					fclose(learnFd);
			}
			

		} else if(atoi(l_key) >=0 && atoi(l_key) < 17) {
			//����ѧϰģʽ��ͬʱ��ѧϰ�������ݱ��浽����
			IR_LEARN_SET(1);
				//return LEARN_DATA_EMPTY;
			usleep(100);
				
			if(READ_IR_LEARN() != 0)
				exit(READ_IR_LEARN_ERR);

		} else {
			exit(INVALID_COMMAND);
		}		
		
	} else{
		exit(INVALID_COMMAND); //������Ч
	}
	//�ӽ�����������
	exit(0);
	}else if(fpid > 0){

		//�ȴ��ӽ��̽���
		waitpid(fpid,&status,0);

		//printf("status:%d \n",WEXITSTATUS(status));
		return WEXITSTATUS(status);

	}


#if 0
	//first
	if(get_secret_key() != 0)
		return SECRET_KEY_ERR;

	//second
	//if(get_Activation_code() != 0)
		//return ACTIVATION_CODE_ERR;

	//three ��������
	//if(get_Electrical_list() != 0)
		//return ELECTRICAL_LIST_ERR;

	//four ��ȡƷ��
	//if(get_Brand_list("15") != 0)
		//return BRAND_LIST_ERR;

	//five
	if(get_Match_code("104","7","4") != 0)
		return MATCH_GCODE_ERR;
		
	if(send_Match_code(7,0,"on") !=0)
		return MATCH_SCODE_ERR;

	if(get_Complete_code("7") != 0)
		return COMPLETE_GCODE_ERR;



	printf("device:%s \n",device);
	
	//if(send_Complete_code("\u683c\u529b0","r_s0_26_u0_l0_p0") != 0)
	if(send_Complete_code("\u683c\u529b0","off") != 0)
		exit(COMPLETE_SCODE_ERR);
#endif	
	//return 0;
}
