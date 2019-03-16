#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#include "cacertinmem.h"
#include "bngo_ir.h"

int bngo_ir(char *device)
{
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

	
	/*******����bngo���ܵ���json����********/
	char *token;	
	token = strtok(device,"_");
	//������Դ����ΪSERVER MATCH USE LEARN
	char dataSource[10] = {0};
	strcpy(dataSource,token);
	
	if(strcmp(dataSource,"SERVER") == 0){
		//��ȡ��������
		char t[10] = {0};
		token = strtok( NULL,"_");
		strcpy(t,token);
		//��ȡ���������������
		char t_path[50] = {0};
		sprintf(t_path,"%s%s%s","/usr/fs/usr/yuan/",t,".txt");
		
		//�ж�����������ⲻ���ڣ��˳�,��Ҫ��ƥ�����
		if (access(t_path,F_OK) != 0)
			return CODE_NOT_EXIST;

		//��ȡ���������������
		char name[50] = {0};
		FILE *fd;
		fd = fopen(t_path,"r+");
		if(fd == NULL){
			printf("%s: open Complete_code.txt error!!!!.\n",__func__);
			return OPEN_FILE_ERR;
		}
		fread(name,50,1,fd);
		fclose(fd);

		//������һ�κ�����ʷ����·��
		char n_path[50] = {0};
		sprintf(n_path,"%s%s%s","/usr/fs/usr/yuan/",name,".record");
		
		//power: open����close
		char power[10] = {0};
		token = strtok( NULL,"_");
		strcpy(power,token);
		if(strcmp(power,"OPEN") == 0){
			char model[5] = {0};
			char wind[5] = {0};
			char temperature[5] = {0};
			char sweep[5] = {0};

			i = 1;
	  		 while( token != NULL )
	  		 {
	  		 	token = strtok( NULL,"_");
	   			if(i ==1)
					strcpy(model,token);
				else if(i ==2)
					strcpy(wind,token);
				else if(i ==3)
					strcpy(temperature,token);
				else if(i ==4)
					strcpy(sweep,token);
		  		++i;
	  		 }

			//��ȡ��һ��: ģʽ
			if(strcmp(model,"NO") != 0){
				//��ȡ��һ����������
				if (access(n_path,F_OK) != 0){
					if(strcmp(model,"a") == 0){
						if(send_Complete_code(name,"a_s0__u0_l0_p0") != 0)
							return COMPLETE_SCODE_ERR;	
					} else if(strcmp(model,"d") == 0){
						if(send_Complete_code(name,"d_s0__u0_l0_p0") != 0)
							return COMPLETE_SCODE_ERR;
					} else if(strcmp(model,"w") == 0){
						if(send_Complete_code(name,"w_s0_26_u0_l0_p0") != 0)
							return COMPLETE_SCODE_ERR;
					} else if(strcmp(model,"h") == 0){
						if(send_Complete_code(name,"h_s0_26_u0_l0_p0") != 0)
							return COMPLETE_SCODE_ERR;
					} else if(strcmp(model,"r") == 0){
						if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
							return COMPLETE_SCODE_ERR;
					}
					
				} else {
					//��ȡ��ʷ����
					char old_command[50] = {0};
					fd = fopen(n_path,"r+");
					if(fd == NULL){
						printf("%s: open command.record error!!!!.\n",__func__);
						return OPEN_FILE_ERR;
					}
					fread(old_command,50,1,fd);
					fclose(fd);

					if(send_Complete_code(name,old_command) != 0)
						return COMPLETE_SCODE_ERR;	
				}
			} 
			 
		/*
		if(strcmp(model,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(wind,"NO") == 0 && strcmp(wind,"NO") == 0){
			//��ȡ��һ����������
			if (access(n_path,F_OK) != 0){
				if(send_Complete_code(name,"r_s0_26_u0_l0_p0") != 0)
					return COMPLETE_SCODE_ERR;	
			} else {
				//��ȡ��ʷ����
				char command[50] = {0};
				fd = fopen(n_path,"r+");
				if(fd == NULL){
					printf("%s: open command.record error!!!!.\n",__func__);
					return OPEN_FILE_ERR;
				}
				fread(command,50,1,fd);
				fclose(fd);

				if(send_Complete_code(name,command) != 0)
					return COMPLETE_SCODE_ERR;	
			}
		} 
		*/

		
			
		} else if(strcmp(power,"CLOSE") == 0){
			if(send_Complete_code(name,"off") != 0)
					return COMPLETE_SCODE_ERR;
		}

		
		
	} else if(strcmp(dataSource,"MATCH") == 0){
		printf("###############  MATCH  ###########\n");
		//����ƥ��ģʽ ��ȡ��������
		char t[10] = {0};
		token = strtok( NULL,"_");
		strcpy(t,token);
		if(strcmp(t,"OK") == 0){
			//��ȡ�������
			if(get_Complete_code("7") != 0)
				return COMPLETE_GCODE_ERR;
		} else {
			char bid[10] = {0};
			char number[10] = {0};
			char src[10] = {0};

			i = 0;
			while( token != NULL )
	  		{
	  		 	token = strtok( NULL,"_");
	   			if(i ==0)
					strcpy(bid,token);
				else if(i ==1)
					strcpy(number,token);
				else if(i ==2)
					strcpy(src,token);
		  		++i;
	  		}
			printf("i:%d   t:%s bid:%s number:%s  src:%s ########\n",i,t,bid,number,src);
			if(i != 4)
				return INVALID_COMMAND; //������Ч

			//printf("t:%d bid:%s number:%d  src:%s ########\n",atoi(t),bid,atoi(number),src);
			//��ȡƥ�����
			if(get_Match_code(bid,t,"4") != 0)
				return MATCH_GCODE_ERR;
			//���Ͳ�����
			if(send_Match_code(atoi(t),atoi(number),src) !=0)
				return MATCH_SCODE_ERR;
			
		}
	
	} else if(strcmp(dataSource,"USE") == 0){

	} else if(strcmp(dataSource,"LEARN") == 0){
 
	} else{
		return INVALID_COMMAND; //������Ч
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
		return COMPLETE_SCODE_ERR;
#endif	
	
	return 0;
}
