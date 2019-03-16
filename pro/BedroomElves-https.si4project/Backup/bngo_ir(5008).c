#define _GNU_SOURCE

#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <pthread.h>
#include <signal.h>


#include "cacertinmem.h"



#if 1
////////////////////  fork  ////////////////////////////////
int bngo_ir(char *device,char *P_COMMAND,char *data)
{
	int file_pipes[2] = {-1,-1};	//定义两个文件描述符
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
			int ret;

			//关闭读端
			close(file_pipes[0]);
			
			result_to_mozart_t mozart_msg;
			memset(&mozart_msg,0,sizeof(result_to_mozart_t));
			
			struct sched_param param; 
			int maxpri; 
			maxpri = sched_get_priority_max(SCHED_FIFO);
			if(maxpri == -1) 
			{ 
				perror("sched_get_priority_max() failed"); 
				strcpy(mozart_msg.ASR_COMMAND,"网络异常");
				
				write(file_pipes[1],&mozart_msg, sizeof(result_to_mozart_t));
				//关闭写端
				close(file_pipes[1]);
				exit(1); 
			} 
				//printf("maxpri:%d ########\n",maxpri);
				
				param.sched_priority = maxpri; 
			if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) //设置优先级
			{ 
				perror("sched_setscheduler() failed"); 
				strcpy(mozart_msg.ASR_COMMAND,"网络异常");
				write(file_pipes[1],&mozart_msg, sizeof(result_to_mozart_t));
				//关闭写端
				close(file_pipes[1]);
				exit(1); 
			} 

			//first 获取设备id
			if(get_secret_key() != 0){
				//write(file_pipes[1], "获取设备ID失败", strlen("获取设备ID失败")+1);

				strcpy(mozart_msg.ASR_COMMAND,"获取设备ID失败");
				write(file_pipes[1],&mozart_msg, sizeof(result_to_mozart_t));
				//关闭写端
				close(file_pipes[1]);
				exit(SECRET_KEY_ERR);
			}
				
			//second  获取激活码 保存在本地
			if(get_Activation_code() != 0){
				strcpy(mozart_msg.ASR_COMMAND,"获取红外激活码失败");
				write(file_pipes[1],&mozart_msg, sizeof(result_to_mozart_t));

				//关闭写端
				close(file_pipes[1]);
				exit(ACTIVATION_CODE_ERR);
			}

			
			/*******解析bngo接受到的json数据********/	
			char *token;	
			token = strtok(device,"=");
			if(token == NULL){
				ret = INVALID_COMMAND;
				goto ir_error;
			}
			//数据来源，分为SERVER MATCH USE LEARN
			char dataSource[10] = {0};
			strcpy(dataSource,token);
			
			if(strcmp(dataSource,"SERVER") == 0){
				//printf("###############  SERVER  ###########\n");
				ret = server_handler_func(token,mozart_msg.ASR_COMMAND);
				
			} else if(strcmp(dataSource,"SERVERBIND") == 0){
				//语音同时打开两个电器
				//printf("###############  SERVERBIND  ###########\n");
				ret = serverbind_handler_func(token,mozart_msg.ASR_COMMAND);
			} else if(strcmp(dataSource,"APPBIND") == 0){
				//printf("###############  APPBIND  ###########\n");
				//app设置绑定关系
				ret = appbind_handler_func(token,mozart_msg.ASR_COMMAND);

			} else if(strcmp(dataSource,"MATCH") == 0){
				//printf("###############  MATCH  ###########\n");
				ret = match_handler_func(token,&mozart_msg);
			} else if(strcmp(dataSource,"AKEYMATCH") == 0){
				//printf("############### A KEY MATCH  ###########\n");
				ret = akeymatch_handler_func(token,mozart_msg.ASR_COMMAND);

			} else if(strcmp(dataSource,"AKMSEND") == 0){
				//printf("###############  AKMSEND  ###########\n");
				ret = akmsend_handler_func(token,&mozart_msg);
				//if(ret == 0)
					//strcpy(mozart_msg.ASR_COMMAND,"一键匹配数据发送成功");
			} else if(strcmp(dataSource,"USE") == 0){
				//printf("###############  USE  ###########\n");
				ret = use_handler_func(token);
				if(ret == 0)
					strcpy(mozart_msg.ASR_COMMAND,"设备控制成功");
				
			} else if(strcmp(dataSource,"LEARN") == 0){
				//printf("############## LEARN #############\n");
				ret = learn_handler_func(token,mozart_msg.ASR_COMMAND);
				
			} else if(strcmp(dataSource,"LENADD") == 0){
				//添加学习过的红外码库数据
				//printf("############## LENADD #############\n");
				ret = lenadd_handler_func(token,mozart_msg.ASR_COMMAND);
				
			} else if(strcmp(dataSource,"LENMOD") == 0){
				//修改学习过的红外码库数据
				//printf("############## LENMOD #############\n");
				ret = lenmod_handler_func(token,mozart_msg.ASR_COMMAND);
			} else if(strcmp(dataSource,"LENSEND") == 0){
				//LENSEND = T = NAME = CMD
				//printf("############## LENSEND #############\n");
				ret = lensend_handler_func(token,mozart_msg.ASR_COMMAND);
				
			} else if(strcmp(dataSource,"DELETE") == 0){
				//printf("############## DELETE #############\n");
				ret = delete_handler_func(token);
				if(ret == 0)
					strcpy(mozart_msg.ASR_COMMAND,"码库删除成功");
			} else if(strcmp(dataSource,"ADD") == 0){
				//printf("############## ADD #############\n");
				ret = add_handler_func(token);
				if(ret == 0)
					strcpy(mozart_msg.ASR_COMMAND,"匹配成功请使用");
				else 
					ret = ADD_CODE_ERR;
			} else if(strcmp(dataSource,"SETIR") == 0){
				//printf("############## SETIR #############\n");
				ret = setir_handler_func(token);
				if(ret == 0)
					strcpy(mozart_msg.ASR_COMMAND,"默认遥控器设置成功");
			} else{
				ret = INVALID_COMMAND;
			}
			
			//把文件刷到磁盘
			system("sync");

ir_error:
			if(ret == SECRET_KEY_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"网络忙,请稍后再试");
			} else if(ret == ACTIVATION_CODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"网络忙,请稍后再试");
			} else if(ret == ELECTRICAL_LIST_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"网络忙,请稍后再试");
			} else if(ret == BRAND_LIST_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"网络忙,请稍后再试");
			} else if(ret == MATCH_GCODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"网络忙,请稍后再试");
			} else if(ret == MATCH_SCODE_ERR){
				if(mozart_msg.ASR_COMMAND[0] == '\0' )
					strcpy(mozart_msg.ASR_COMMAND,"红外码库数据发送失败，请重新尝试");
				
			} else if(ret == COMPLETE_GCODE_ERR){
				if(mozart_msg.ASR_COMMAND[0] == '\0' )
					strcpy(mozart_msg.ASR_COMMAND,"红外码库获取失败，请重新尝试");
					
			} else if(ret == COMPLETE_SCODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"网络忙,请稍后再试");
			} else if(ret == INVALID_COMMAND){
				strcpy(mozart_msg.ASR_COMMAND,"您说的指令小乐还在学习中");
			} else if(ret == CODE_NOT_EXIST){
				strcpy(mozart_msg.ASR_COMMAND,"本地码库不存在");
			} else if(ret == OPEN_FILE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"打开配置文件错误");
			} else if(ret == INVALID_OLD_COMMAND){
				strcpy(mozart_msg.ASR_COMMAND,"红外命令文件校验错误");
			} else if(ret == READ_IR_LEARN_ERR){
				//strcpy(mozart_msg.ASR_COMMAND,"匹配超时，请选择是否进入码库匹配");
				strcpy(mozart_msg.ASR_COMMAND,"匹配超时");
			} else if(ret == LEARN_PATH_EMPTY){
				strcpy(mozart_msg.ASR_COMMAND,"本地码库不存在");
			} else if(ret == SEND_IR_LEARN_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"发送红外学习数据失败");
			} else if(ret == CODE_V_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"码库版本校验失败,请重新匹配");
			} else if(ret == SETIR_FILE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"设置默认遥控器失败");
			} else if(ret == AKEYMATCH_GCODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"匹配失败，请选择是否进入码库匹配");
			} else if(ret == IR_LEARN_EXIT){
				strcpy(mozart_msg.ASR_COMMAND,"退出红外学习模式");
				ret = 0;
			} else if(ret == SET_ASR_IR){
				strcpy(mozart_msg.ASR_COMMAND,"请先设置语音默认遥控器，再重新操作");
			} else if(ret == ADD_CODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"码库添加失败，请重新匹配");
			} else if(ret == DEL_CODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"码库删除失败");
			} else if(ret == LEARN_SED_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"请先学习该按键");
			} else if(ret == COMPLETE_SCODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"发送失败，请重新尝试");
			} else if(ret == JSON_PARESE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"码库信息出错，请重新添加");
			} else if(ret == JSON_KEY_NOT_EXIST){
				strcpy(mozart_msg.ASR_COMMAND,"该版本码库暂不支持此按键");
			} else if(ret == JSON_CODE_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"本地码库信息有误，请重新添加");
			} else if(ret == BIND_NOT_EXIST){
				strcpy(mozart_msg.ASR_COMMAND,"请先添加绑定关系，再重新操作");
			} else if(ret == NOCH_ERR){
				strcpy(mozart_msg.ASR_COMMAND,"请先添加频道列表，再重新操作");
			}
			
			int wcount = write(file_pipes[1],&mozart_msg, sizeof(result_to_mozart_t));
			if(wcount < 0){
				printf("### [%s] [%d] [count:%d] [file_pipes[1]:%d] write error ### \n",__func__,__LINE__,wcount,file_pipes[1]);
			}
			//关闭写端
			close(file_pipes[1]);
			
			if(ret != 0){
				exit(ret);
			}

			//子进程正常返回
			exit(0);
	
	}else if(fpid > 0){
		////关闭写端
		close(file_pipes[1]);
		
		result_to_mozart_t mozart_msg_p;
		memset(&mozart_msg_p,0,sizeof(result_to_mozart_t));

		//printf("### [%s] [%d] ### \n",__func__,__LINE__);
		//使用select监听 /////////////////////////////////////
		 fd_set readfd; //读文件描述符集合

		struct timeval timeout;
		int s_ret = -1;
		int timeout_cnt = -1;
		int rcount = 0;

		while(1){
			timeout.tv_sec = 1;
        	timeout.tv_usec = 0;

			 //文件描述符集合清0
        	FD_ZERO(&readfd);

        	//将套接字描述符加入到文件描述符集合
        	FD_SET(file_pipes[0], &readfd);

       	 	//select侦听是否有数据到来
        	s_ret = select(file_pipes[0] + 1, &readfd, NULL, NULL, &timeout); //侦听是否可读
	       	 switch (s_ret)
	        {
		        case -1: //发生错误
		            perror("select error:");
					usleep(500000);
					 if(timeout_cnt > 34){
						close(file_pipes[0]);
						strcpy(P_COMMAND,"系统异常，请重新尝试");
						return -1;
					}
		            timeout_cnt++;
		            break;
		        case 0: //超时
		            //printf("[%s] [%d] select timeout\n",__func__,__LINE__);
		            if(timeout_cnt > 34){
						close(file_pipes[0]);
						strcpy(P_COMMAND,"系统异常，请重新尝试");
						return -1;
					}
		            timeout_cnt++;
		            break;
		        default:
					rcount = read(file_pipes[0], &mozart_msg_p, sizeof(result_to_mozart_t));
					if(rcount < 0){
						printf("### [%s] [%d] [count:%d] [file_pipes[0]:%d] read error ### \n",__func__,__LINE__,rcount,file_pipes[0]);
					}

					//关闭读端
					close(file_pipes[0]);
					strcpy(P_COMMAND,mozart_msg_p.ASR_COMMAND);
					sprintf(data,"%d",mozart_msg_p.key);

					//等待子进程结束
					waitpid(fpid,&status,0);
					
					//printf("status:%d \n",WEXITSTATUS(status));
					return WEXITSTATUS(status);
					break;
	       	 }
		}
		
		 /////////////////////////////////////////////////////
		/*
		int rcount = read(file_pipes[0], &mozart_msg_p, sizeof(result_to_mozart_t));
		if(rcount < 0){
			printf("### [%s] [%d] [count:%d] [file_pipes[0]:%d] read error ### \n",__func__,__LINE__,rcount,file_pipes[0]);
		}
		
		//关闭读端
		close(file_pipes[0]);
		
		strcpy(P_COMMAND,mozart_msg_p.ASR_COMMAND);
		sprintf(data,"%d",mozart_msg_p.key);

		//等待子进程结束
		waitpid(fpid,&status,0);
		
		//printf("status:%d \n",WEXITSTATUS(status));

		printf("### [%s] [%d] ### \n",__func__,__LINE__);
		return WEXITSTATUS(status);
		*/

	} else {
		strcpy(P_COMMAND,"网络忙,请稍后再试");
		return -1;
	}
	


	//return 0;
}
#endif
