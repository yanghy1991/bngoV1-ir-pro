#include "bngo_ir_learn.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <ctype.h>


//���ⷢ����������
struct pwm_date {
	int pwm_time;
	int pwm_set;
};

//����ѧϰ���ܵ�����
struct ir_learn {
	unsigned int count;
	unsigned int times[512];
};


int IR_SEND(uint32 time,int set)
{	
	int fd;
	fd = open("/dev/ir_send",O_RDWR);
	if(fd<0){
		printf("open /dev/ir_send error\n");
		return -1;
	}
	
	struct pwm_date p_date;
	p_date.pwm_time = time;
	p_date.pwm_set = set;	
	write(fd,&p_date,sizeof(p_date));

	lseek(fd,0,SEEK_SET);
	fsync(fd);

	return close(fd);
}


/*
 *  ����ѧϰģʽ
 */ 
int READ_IR_LEARN()
 {
	 struct ir_learn l_date;
	 l_date.count = 0;
	 memset(l_date.times,0,512);
	 int fd;
	 int flag = 1;
	 fd = open("/dev/ir_learn",O_RDWR);
	 if(fd<0){
		 printf("open /dev/ir_learn error\n");
		 return -1;
	 }
 
	 read(fd,&l_date,sizeof(l_date));
	 
	 printf("count:%d .\n",l_date.count);
	 //û��ѧϰ������
	 if(l_date.count == 0)
	 	return -1;
	 int i;
	 for(i = 0;i<l_date.count;i++){
		 //printf("%lld ",p_date.times[i]); 
		 //printf("%d ",l_date.times[i]); 
		 IR_SEND(l_date.times[i],flag);
		 flag ^= 1;
	 }	 
	 close(fd);

	//��ѧϰ���ݱ���
	
	FILE *l_fd;
	l_fd = fopen("/usr/fs/usr/yuan/learn.txt","w+");
	if(l_fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fwrite(&l_date,sizeof(l_date),1,l_fd);
	fclose(l_fd);
 	
	 return 0;
 }

//��ȡ����ѧϰ���ݣ�Ȼ����
int SEND_IR_LEARN(char *filename,int key)
{
	 struct ir_learn l_date;
	 l_date.count = 0;
	 memset(l_date.times,0,512);

	 char learn_path[50] = {0};
	 sprintf(learn_path,"%s%s%s","/usr/fs/usr/yuan/",filename,".txt");
     //ѧϰ�ļ������ڣ�����
	 if (access(learn_path,F_OK) != 0)
	 	return -2;


	 //����ȡѧϰ����
	 FILE *l_fd;
	 l_fd = fopen(learn_path,"r+");
	 if(l_fd == NULL){
		 printf("open learn_path.txt error!!!!.\n");
		 return -1;
	 }
	 fseek(l_fd,key*sizeof(l_date),SEEK_SET);
	 fread(&l_date,sizeof(l_date),1,l_fd);
	 fclose(l_fd);

	 printf("count:%d .\n",l_date.count);
	 //û��ѧϰ������
	 if(l_date.count == 0)
	 	return -1;
	 int i;
	 int flag = 1;
	 for(i = 0;i<l_date.count;i++){
		 //printf("%lld ",p_date.times[i]); 
		 //printf("%d ",l_date.times[i]); 
		 IR_SEND(l_date.times[i],flag);
		 flag ^= 1;
	 }	 

	 return 0;
}


int IR_LEARN_SET(char byte)
{
	 int fd;
	 fd = open("/dev/ir_learn",O_RDWR);
	 if(fd<0){
		 printf("open /dev/ir_learn error\n");
		 return -1;
	 }
 
	 write(fd,&byte,sizeof(byte));	 
	 close(fd);
 
	 return 0;
}

