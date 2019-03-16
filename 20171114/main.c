#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#include "bngo_ir.h"

int main(int argc,char *argv[])
{
	int ret;
	//pid_t fpid;
	//fpid = fork();
	//if(fpid == 0){
		struct sched_param param; 
		int maxpri; 
		maxpri = sched_get_priority_max(SCHED_FIFO);
		if(maxpri == -1) 
		{ 
			perror("sched_get_priority_max() failed"); 
			exit(1); 
		} 
			param.sched_priority = maxpri; 
		if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) //设置优先级
		{ 
			perror("sched_setscheduler() failed"); 
			exit(1); 
		} 
		
		ret = bngo_ir("MATCH_7_104_0_on");
		printf("ret :%d \n",ret);
		
	//} 
	//int status;
	//waitpid(fpid,&status,0);
	//printf("status:%d #######\n",status);
	
	
	return 0;
}