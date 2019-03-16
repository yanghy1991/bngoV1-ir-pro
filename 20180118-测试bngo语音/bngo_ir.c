#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include "cacertinmem.h"



int bngo_ir(char *device,char *P_COMMAND)
{
	int file_pipes[2];	//定义两个文件描述符
	if (pipe(file_pipes) < 0){
		strcpy(P_COMMAND,"网络异常");
		return -1;
	}


	int status;
	pid_t fpid;
	fpid = fork();
	//创建子进程，设置为实时进程
	if(fpid == 0){
			//设置为实时进程
			int i;
			int ret;
			char ASR_COMMAND[512] = {0};
			
			struct sched_param param; 
			int maxpri; 
			maxpri = sched_get_priority_max(SCHED_FIFO);
			if(maxpri == -1) 
			{ 
				perror("sched_get_priority_max() failed");
				write(file_pipes[1],"网络异常", strlen("网络异常"));
				exit(1); 
			} 
				param.sched_priority = maxpri; 
			if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) //设置优先级
			{ 
				perror("sched_setscheduler() failed"); 
				write(file_pipes[1],"网络异常", strlen("网络异常"));
				exit(1); 
			} 
			
			//first 获取设备id
			//if(get_secret_key() != 0)
				//exit(SECRET_KEY_ERR);
			
			/*******解析bngo接受到的json数据********/
			
			char *token;	
			token = strtok(device,"=");
			if(token == NULL)
				return(INVALID_COMMAND);
			//数据来源，分为SERVER MATCH USE LEARN
			char dataSource[10] = {0};
			strcpy(dataSource,token);
			
			if(strcmp(dataSource,"SERVER") == 0){
				printf("###############  SERVER  ###########\n");
				ret = server_handler_func(token,ASR_COMMAND);
			} else if(strcmp(dataSource,"MATCH") == 0){
				printf("###############  MATCH  ###########\n");
				ret = match_handler_func(token);	
			} else if(strcmp(dataSource,"USE") == 0){
				printf("###############  USE  ###########\n");
				ret = use_handler_func(token);		
			} else if(strcmp(dataSource,"LEARN") == 0){
				printf("############## LEARN #############\n");
				ret = learn_handler_func(token);
			} else if(strcmp(dataSource,"DELETE") == 0){
				printf("############## DELETE #############\n");
				ret = delete_handler_func(token);		
			} else if(strcmp(dataSource,"ADD") == 0){
				printf("############## ADD #############\n");
				ret = add_handler_func(token);
			} else if(strcmp(dataSource,"SETIR") == 0){
				printf("############## SETIR #############\n");
				ret = setir_handler_func(token);
	
			} else{
				//exit(INVALID_COMMAND); //命令无效
				ret = INVALID_COMMAND;
			}
			if(ret == INVALID_COMMAND)
			{
				strcpy(ASR_COMMAND,"小乐暂时不支持该领域指令");
			}
			write(file_pipes[1], ASR_COMMAND, strlen(ASR_COMMAND)+1);
			
			if(ret != 0)
					exit(ret);
			
			//子进程正常返回
			exit(0);
	
	}else if(fpid > 0){

		read(file_pipes[0], P_COMMAND, 512);
		//等待子进程结束
		waitpid(fpid,&status,0);

		close(file_pipes[0]);
		close(file_pipes[1]);
		//printf("status:%d \n",WEXITSTATUS(status));
		return WEXITSTATUS(status);

	} else {
		strcpy(P_COMMAND,"网络异常");
		return -1;
	}
	
	//return 0;
}
