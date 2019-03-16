#include "all_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cacertinmem.h"
#include "bngo_ir.h"
#include "bngo_ir_learn.h"

int air_handler_func(char *v,char *name,char  *n_path,char *token)
{
	int i;
	FILE *fd;
	
	if(strcmp(v,"3") == 0){
		//power: open����close
		char power[10] = {0};
		token = strtok( NULL,"-");
		strcpy(power,token);
		if(strcmp(power,"OPEN") == 0){
			//#############�����յ�����#################
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
					return(INVALID_COMMAND);

				printf("model:%s wind:%s temperature:%s sweep:%s \n",model,wind,temperature,sweep);
				//��ȡ��һ��: ģʽ
				if(strcmp(model,"NO") != 0){
					//��ȡ��һ����������
					if (access(n_path,F_OK) != 0){
						if(strcmp(model,"a") == 0){
							if(send_Complete_code(name,"a_s0__u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);	
						} else if(strcmp(model,"d") == 0){
							if(send_Complete_code(name,"d_s0__u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"w") == 0){
							if(send_Complete_code(name,"w_s0_26_u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"h") == 0){
							if(send_Complete_code(name,"h_s0_26_u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else if(strcmp(model,"r") == 0){
							if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								return(COMPLETE_SCODE_ERR);
						} else {
								return(INVALID_COMMAND);
						}
					
					} else {
						//��ȡ*.record ������һ������ old_command
						fd = fopen(n_path,"r+");
						if(fd == NULL){
							printf("%s: open command.record error!!!!.\n",__func__);
							return(OPEN_FILE_ERR);
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
										return(COMPLETE_SCODE_ERR);
												
							}	else {
									return(INVALID_OLD_COMMAND);
							}						
						} else if(strcmp(model,"h") == 0){
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'h';
								if(send_Complete_code(name,old_command) != 0)
									return(COMPLETE_SCODE_ERR);
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
										return(COMPLETE_SCODE_ERR);
								
								
							}	else {
								return(INVALID_OLD_COMMAND);
							}							
						} else if(strcmp(model,"r") == 0){
							if(strstr(old_command,"w") != NULL || strstr(old_command,"h") != NULL || strstr(old_command,"r") != NULL ){
								old_command[0] = 'r';
								if(send_Complete_code(name,old_command) != 0)
									return(COMPLETE_SCODE_ERR);
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
										return(COMPLETE_SCODE_ERR);
								} else {
									return(INVALID_OLD_COMMAND);								
								}															
						}else{
							return(INVALID_COMMAND);
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
								  return(INVALID_COMMAND);
							  }
							 
							  printf("new_command:%s ########\n",new_command);
							  if(send_Complete_code(name,new_command) != 0)
								  return(COMPLETE_SCODE_ERR);
						} else {
								//��ȡ*.record ������һ������ old_command
								fd = fopen(n_path,"r+");
								if(fd == NULL){
										printf("%s: open command.record error!!!!.\n",__func__);
										return(OPEN_FILE_ERR);
								}
								fread(old_command,50,1,fd);
								fclose(fd);
								//��ӡ��ʷ����
								printf("old_command:%s	#####\n",old_command);

								//��ȡ����
								char *strWind2 = strstr(old_command,"s");
								if(strWind2 == NULL)
									return(INVALID_COMMAND);
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
										return(COMPLETE_SCODE_ERR);

								
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
								  return(COMPLETE_SCODE_ERR);
					  } else {
							  //��ȡ*.record ������һ������ old_command
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  return(OPEN_FILE_ERR);
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
										return(COMPLETE_SCODE_ERR);		
									
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
								  		return(COMPLETE_SCODE_ERR);
							  }
							  
					  }
				  	}else {
						return(INVALID_COMMAND);
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
								  return(INVALID_COMMAND);
							  }
							 
							  printf("new_command:%s ########\n",new_command);
							  if(send_Complete_code(name,new_command) != 0)
								  return(COMPLETE_SCODE_ERR);
					  } else{
							  //��ȡ*.record ������һ������ old_command
							  fd = fopen(n_path,"r+");
							  if(fd == NULL){
									  printf("%s: open command.record error!!!!.\n",__func__);
									  return(OPEN_FILE_ERR);
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
											return(INVALID_COMMAND);
										}

							 			 printf("new_command:%s ########\n",old_command);
							 			 if(send_Complete_code(name,old_command) != 0)
								 			 return(COMPLETE_SCODE_ERR);										

							  } else if(strstr(old_command,"a") != NULL || strstr(old_command,"d") != NULL ){
								 		 if(strstr(sweep,"u0") != NULL){
										  	old_command[7] = '0';
										 	 old_command[10] = '0';
								 	 	} else if(strstr(sweep,"u1") != NULL){
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
				  //û�����ò���
				  else if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(temperature,"NO") == 0 && strcmp(sweep,"NO") == 0){
					  //�����ʷ�������
					  if (access(n_path,F_OK) != 0){
							  printf("new_command:%s ########\n","r_s0_26_u0_l0_p0");
							  if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
								  return(COMPLETE_SCODE_ERR);
					  } else{

					  		//��ȡ*.record ������һ������ old_command
					 		fd = fopen(n_path,"r+");
					 		if(fd == NULL){
							  	printf("%s: open command.record error!!!!.\n",__func__);
							 	return(OPEN_FILE_ERR);
					  		}
					  		fread(old_command,50,1,fd);
					  		fclose(fd);
					  		//��ӡ��ʷ����
					
							printf("new_command:%s ########\n",old_command);
							if(send_Complete_code(name,old_command) != 0)
								return(COMPLETE_SCODE_ERR);

					  }


				} else {
					//����������Ч
					return(INVALID_COMMAND);
				}


		
			
		} else if(strcmp(power,"CLOSE") == 0){
			if(send_Complete_code(name,"off") != 0)
					return(COMPLETE_SCODE_ERR);
		} else {
			//����������Ч
					return(INVALID_COMMAND);	
		}

	} else if(strcmp(v,"1") == 0){

	}

	return 0;
}

