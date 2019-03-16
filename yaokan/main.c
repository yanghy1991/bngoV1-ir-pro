#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "yaokan_ir.h"

int main(int argc,char *argv[])
{
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
	
	#if 0
	unsigned char secret_key[25] = {0};
	//memset(secret_key,'\0',24);
	int i;
	i =  Registration_equipment(secret_key);
	if(i != 0){
		printf("equipment error!\n");
	}
	printf("%s\n",secret_key);
	#endif 
	
	unsigned char code[512] = "K4rfzhTJS3rnJB8rvyWPDOz8VVj47le3FOXXILEB4KE=";
	unsigned char command[1024] = "01Z/+qis0E26yfD+iCiN/WnI963JNqWgqZO6GYaDbQzPDP2yJJLxgrAhiPJcso8gnTVzGBIjBl9AL/2gRgjrB/quTAnGyV3Hr9OYdAY3zj4sbv9Tc9Ozr8MAqp5aNZB9B02kR229+RxcQSF72wSMurXf8QxZm470ECJDPgr83EleM4i3GMDtEl03esMlUOQGcQ";
	//unsigned char command[1024] = "01gWVXLRMNKzdMaAO2Ir6krodUIExjsAWOpRCA5XrIEUQzLw5FP9Q/QdxfphIaePD5";
	//unsigned char command[512] = "01gqt3Xrrd2P1PYXPHf07eFa2j2PAm04B02sJjFJn6w\/HAmxsrRZY3Y0e88mP\/jZmqS0\/rLVC+cH0oCuAeJD863WvXlEYLqWtKDyLHVt6w2RU=";
	#if 1
	int ret;
	ret=IR_send_data(code,command);
	if(ret==0)
		printf("sucees\n");
	else
		printf("fail\n");
	#endif
	
	return 0;
}

