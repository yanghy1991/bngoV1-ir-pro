#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include "bngo_led.h"
#include "cacertinmem.h"
#include "bngo_ir_learn.h"


//红外发送脉宽设置
struct pwm_date {
	int pwm_time;
	int pwm_set;
};

//红外学习接受的数据
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
 *  红外学习模式
 */ 
 int READ_IR_LEARN( struct ir_learn *l_date)
 {
	 //struct ir_learn l_date;
	 //l_date.count = 0;
	 //memset(l_date.times,0,512);
	 
	 int fd;
	 int flag = 1;
	 fd = open("/dev/ir_learn",O_RDWR);
	 if(fd<0){
		 printf("open /dev/ir_learn error\n");
		 return 11;
	 }
 
	 read(fd,l_date,sizeof(struct ir_learn));
	 close(fd);
	 
	 //printf("count:%d .\n",l_date.count);
	 //没有学习到数据
	 if(l_date->count == 0)
	 	return 13;
	 else if(l_date->count == 2)
	 	return 19;

	
	 //发送红外数据
	 /*
	 int i;
	 for(i = 0;i<l_date->count;i++){
		 //printf("%lld ",p_date.times[i]); 
		 //printf("%d ",l_date.times[i]); 
		 IR_SEND(l_date->times[i],flag);
		 flag ^= 1;
	 }	 
	*/

	 return 0;
 }
#if 0
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
		 return 11;
	 }
 
	 read(fd,&l_date,sizeof(l_date));
	 
	 //printf("count:%d .\n",l_date.count);
	 //没有学习到数据
	 if(l_date.count == 0)
	 	return 13;
	 int i;
	 for(i = 0;i<l_date.count;i++){
		 //printf("%lld ",p_date.times[i]); 
		 //printf("%d ",l_date.times[i]); 
		 IR_SEND(l_date.times[i],flag);
		 flag ^= 1;
	 }	 
	 close(fd);

	//将学习数据保存
	
	FILE *l_fd;
	l_fd = fopen("/usr/data/learn.txt","w+");
	if(l_fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return 11;
	}
	fwrite(&l_date,sizeof(l_date),1,l_fd);
	fclose(l_fd);
 	
	 return 0;
 }
#endif

//将学习到的数据化为一键匹配数据
int MATCH_IR_LEARN(char *match_learn_src)
{
	 struct ir_learn l_date;
	 l_date.count = 0;
	 memset(l_date.times,0,sizeof(unsigned int)*512);
	 int fd;
	 int flag = 1;
	 fd = open("/dev/ir_learn",O_RDONLY);
	 if(fd<0){
		 printf("open /dev/ir_learn error\n");
		 return 11;
	 }
 
	 read(fd,&l_date,sizeof(l_date));
	 close(fd);
	 
	 //printf("count:%d .\n",l_date.count);
	 //没有学习到数据
	 if(l_date.count == 0)
	 	return 13;
	 else if(l_date.count == 2)
	 	return 19;
	 unsigned char timeSrc[10] = {0};
	 unsigned int b = 0;
	 int i;	 
	
	 //遥看需要的拼接头
	 strcat(match_learn_src,"1,38000,");
	 for(i = 0;i<l_date.count;i++){ 
		 //printf("%d ",l_date.times[i]);
		 //IR_SEND(l_date.times[i],flag);
		 
		 b = (l_date.times[i]/26.3)+1;
		 sprintf(timeSrc,"%d",b);
		 strcat(match_learn_src,timeSrc);
		 if(i != l_date.count-1)
		 	strcat(match_learn_src,",");
		 flag ^= 1;
	 }	 
 

#if 0	
	//将学习数据保存
	FILE *l_fd;
	l_fd = fopen("/usr/data/learn.txt","w+");
	if(l_fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return 11;
	}
	fwrite(&l_date,sizeof(l_date),1,l_fd);
	fclose(l_fd);
#endif 
	return 0;
}

//读取本地学习数据，然后发送
int SEND_IR_LEARN(char *filename,int key)
{
	 struct ir_learn l_date;
	 l_date.count = 0;
	 memset(l_date.times,0,512);

	 char learn_path[50] = {0};
	 sprintf(learn_path,"%s%s%s","/usr/data/",filename,".txt");
     //学习文件不存在，返回
	 if (access(learn_path,F_OK) != 0)
	 	return -2;


	 //将读取学习数据
	 FILE *l_fd;
	 l_fd = fopen(learn_path,"r+");
	 if(l_fd == NULL){
		 printf("open learn_path.txt error!!!!.\n");
		 return -1;
	 }
	 fseek(l_fd,key*sizeof(l_date),SEEK_SET);
	 fread(&l_date,sizeof(l_date),1,l_fd);
	 fclose(l_fd);

	 //printf("count:%d .\n",l_date.count);
	 //没有学习到数据
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
	 fd = open("/dev/ir_learn",O_WRONLY);
	 if(fd<0){
		 printf("open /dev/ir_learn error\n");
		 return 11;
	 }
 
	 write(fd,&byte,sizeof(byte));	 
	 close(fd);
	 
	 return 0;
}


//获取学习码库头部信息
int get_learn_info(learn_head_t *info,char *file_name)
{
	FILE *Fd;
	//file_path 为文件绝对路径
	char file_path[512] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file_name,".txt");

	//读出学习info的信息
	Fd = fopen(file_path,"r+");
	if(Fd == NULL){
		printf("[%s] [%d] open file_path:%s error!!!!.\n",__func__,__LINE__,file_path);
		return(OPEN_FILE_ERR);
	}
	fread(info,sizeof(learn_head_t),1,Fd);
	fclose(Fd);

	return 0;
}

int get_learn_offset(int *offset,char *appCommand)
{


	/*
		1.机顶盒: 	 power\poweroff\0\1\2\3\4\5\6\7\8\9\ch-\ch+\vol-\vol+\up\down\left\right\exit\menu\mute\ok\signal\back\boot
		2.电视: 	 power\poweroff\0\1\2\3\4\5\6\7\8\9\ch-\ch+\vol-\vol+\up\down\left\right\exit\menu\mute\ok\signal\back\boot
		6.风扇: 	 power\poweroff\mode\oscillation\timer\fanspeed\
		7.空调: 	 on\off\aa\ad\aw\ar16..ar30\ah16..ah30
		10.电视盒子: power\poweroff\0\1\2\3\4\5\6\7\8\9\ch-\ch+\vol-\vol+\up\down\left\right\exit\menu\mute\ok\signal\back\boot
	*/
	
	int k = 0;
	//开
	if(strcmp(appCommand,"power") == 0 || strcmp(appCommand,"on") == 0){
		k = sizeof(learn_head_t) + 0*sizeof(struct ir_learn);
	//关
	} else if(strcmp(appCommand,"poweroff") == 0 || strcmp(appCommand,"off") == 0){
		k = sizeof(learn_head_t) + 1*sizeof(struct ir_learn);
	//模式 数字键:0 空调自动 
	}else if(strcmp(appCommand,"mode") == 0 || strcmp(appCommand,"0") == 0 || strcmp(appCommand,"aa") == 0){
		k = sizeof(learn_head_t) + 2*sizeof(struct ir_learn);
	//摇头 数字键:1 空调除湿 
	}else if(strcmp(appCommand,"oscillation") == 0 || strcmp(appCommand,"1") == 0 || strcmp(appCommand,"ad") == 0){
		k = sizeof(learn_head_t) + 3*sizeof(struct ir_learn);
	//定时 数字键:2 空调送风 
	}else if(strcmp(appCommand,"timer") == 0 || strcmp(appCommand,"2") == 0 || strcmp(appCommand,"aw") == 0){
		k = sizeof(learn_head_t) + 4*sizeof(struct ir_learn);			
	//风速 数字键:3 空调制冷16度
	}else if(strcmp(appCommand,"fanspeed") == 0 || strcmp(appCommand,"3") == 0 || strcmp(appCommand,"ar16") == 0){
		k = sizeof(learn_head_t) + 5*sizeof(struct ir_learn);	
	//
	}else if(strcmp(appCommand,"4") == 0 || strcmp(appCommand,"ar17") == 0){
		k = sizeof(learn_head_t) + 6*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"5") == 0 || strcmp(appCommand,"ar18") == 0){
		k = sizeof(learn_head_t) + 7*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"6") == 0 || strcmp(appCommand,"ar19") == 0){
		k = sizeof(learn_head_t) + 8*sizeof(struct ir_learn);
	
	}else if(strcmp(appCommand,"7") == 0 || strcmp(appCommand,"ar20") == 0){
		k = sizeof(learn_head_t) + 9*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"8") == 0 || strcmp(appCommand,"ar21") == 0){
		k = sizeof(learn_head_t) + 10*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"9") == 0 || strcmp(appCommand,"ar22") == 0){
		k = sizeof(learn_head_t) + 11*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ch-") == 0 || strcmp(appCommand,"ar23") == 0){
		k = sizeof(learn_head_t) + 12*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ch+") == 0 || strcmp(appCommand,"ar24") == 0){
		k = sizeof(learn_head_t) + 13*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"vol-") == 0 || strcmp(appCommand,"ar25") == 0){
		k = sizeof(learn_head_t) + 14*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"vol+") == 0 || strcmp(appCommand,"ar26") == 0){
		k = sizeof(learn_head_t) + 15*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"up") == 0 || strcmp(appCommand,"ar27") == 0){
		k = sizeof(learn_head_t) + 16*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"down") == 0 || strcmp(appCommand,"ar28") == 0){
		k = sizeof(learn_head_t) + 17*sizeof(struct ir_learn);
	
	}else if(strcmp(appCommand,"left") == 0 || strcmp(appCommand,"ar29") == 0){
		k = sizeof(learn_head_t) + 18*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"right") == 0 || strcmp(appCommand,"ar30") == 0){
		k = sizeof(learn_head_t) + 19*sizeof(struct ir_learn);
		
	/////////////////////////////////////////////////////////////////////////////
	}else if(strcmp(appCommand,"exit") == 0 || strcmp(appCommand,"ah16") == 0){
		k = sizeof(learn_head_t) + 20*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"menu") == 0 || strcmp(appCommand,"ah17") == 0){
		k = sizeof(learn_head_t) + 21*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"mute") == 0 || strcmp(appCommand,"ah18") == 0){
		k = sizeof(learn_head_t) + 22*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ok") == 0 || strcmp(appCommand,"ah19") == 0){
		k = sizeof(learn_head_t) + 23*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"signal") == 0 || strcmp(appCommand,"ah20") == 0){
		k = sizeof(learn_head_t) + 24*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"back") == 0 || strcmp(appCommand,"ah21") == 0){
		k = sizeof(learn_head_t) + 25*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"boot") == 0 || strcmp(appCommand,"ah22") == 0){
		k = sizeof(learn_head_t) + 26*sizeof(struct ir_learn);
	
	}else if(strcmp(appCommand,"ah23") == 0){
		k = sizeof(learn_head_t) + 27*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ah24") == 0){
		k = sizeof(learn_head_t) + 28*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ah25") == 0){
		k = sizeof(learn_head_t) + 29*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ah26") == 0){
		k = sizeof(learn_head_t) + 30*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ah27") == 0){
		k = sizeof(learn_head_t) + 31*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ah28") == 0){
		k = sizeof(learn_head_t) + 32*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ah29") == 0){
		k = sizeof(learn_head_t) + 33*sizeof(struct ir_learn);
	//
	}else if(strcmp(appCommand,"ah30") == 0){
		k = sizeof(learn_head_t) + 34*sizeof(struct ir_learn);
	} else {
			return INVALID_COMMAND;
	}
	
	*offset = k;
	
	return 0;
}

//所有电器自学习函数
int Ele_learn_func(char *t,char *bid,char *file_name,char *appCommand,char *ASR_COMMAND)
{

	int ret = 0;

	//写 偏移量 
	int offset = 0;
	//根据命令 获取偏移量
	ret = get_learn_offset(&offset,appCommand);
	if(ret != 0)
		return ret;

	//红外数据
	struct ir_learn learn_date;
	memset(&learn_date,0,sizeof(struct ir_learn));

	//头部信息
	learn_head_t learn_info;
	memset(&learn_info,0,sizeof(learn_head_t));

	strcpy(learn_info.t,t);
	strcpy(learn_info.bid,bid);


	//进入学习模式，同时将学习到的数据保存到本地 红灯亮
	IR_LEARN_SET(1);
		//return LEARN_DATA_EMPTY;
	usleep(100);

	ret = READ_IR_LEARN(&learn_date);
		
	if(ret != 0)
		return ret;

	printf("log: [%s] [%d] [count:%d] [offset:%d] [learn_head_t:%d] ###\n",__func__,__LINE__,learn_date.count,offset,sizeof(learn_head_t));
	
	//file_path 为文件绝对路径
	char file_path[512] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file_name,".txt");

	FILE *learnFd;

	//文件不存在 w+打开
	if (access(file_path,F_OK) != 0){
		//将学习到的数据写入
		learnFd = fopen(file_path,"w+");
		if(learnFd == NULL){
			printf("%s %d: open Complete_code.txt error!!!!.\n",__func__,__LINE__);
			return(OPEN_FILE_ERR);
		}
		fwrite(&learn_info,sizeof(learn_head_t),1,learnFd);	

	} else {
		//文件存在 r+ 打开 将学习到的数据写入
		learnFd = fopen(file_path,"r+");
		if(learnFd == NULL){
			printf("%s: open Complete_code.txt error!!!!.\n",__func__,__LINE__);
			return(OPEN_FILE_ERR);
		}
	}

	fseek(learnFd,offset,SEEK_SET);
	fwrite(&learn_date,sizeof(learn_date),1,learnFd);
	fflush(learnFd);
	fclose(learnFd);

	return 0;
}

//所有电器发送函数
int Ele_send_func(char *t,char *file_name,char *appCommand)
{
	int ret = 0;
	//获取 偏移量
	int offset = 0;
	//根据命令 获取偏移量
	ret = get_learn_offset(&offset,appCommand);
	if(ret != 0)
		return ret;

	//file_path 为文件绝对路径
	char file_path[512] = {0};
	sprintf(file_path,"%s%s%s","/usr/data/",file_name,".txt");

	//读取红外数据结构体
	struct ir_learn l_date;
	memset(&l_date,0,sizeof(struct ir_learn));
	
	int flag = 1;
	
	FILE *l_fd;
	l_fd = fopen(file_path,"r+");
	if(l_fd == NULL){
		printf("open secret_fan.txt error!!!!.\n");
		return 10;
	}
	fseek(l_fd,offset,SEEK_SET);
	fread(&l_date,sizeof(l_date),1,l_fd);
	fclose(l_fd);
	
	
	//printf("log: [%s] [%d] [count:%d] [offset:%d] [learn_head_t:%d] ###\n",__func__,__LINE__,l_date.count,offset,sizeof(learn_head_t));

	//没有学习到数据 或者 数据异常
	if(l_date.count == 0 || l_date.count > 512)
	   return LEARN_SED_ERR;
	int i;
	for(i = 0;i<l_date.count;i++){
		//printf("%lld ",p_date.times[i]); 
		//printf("%d ",l_date.times[i]); 
		IR_SEND(l_date.times[i],flag);
		flag ^= 1;
	}	

	//如果是空调，保存历史命令
	if(strcmp(t,"7") == 0 && strcmp(appCommand,"on") !=0 && strcmp(appCommand,"off") !=0){
		char file_path_record[512] = {0};
		sprintf(file_path_record,"%s%s%s","/usr/data/",file_name,".record");

		l_fd = fopen(file_path_record,"w+");
		if(l_fd == NULL){
			printf("%s: open record error!!!!.\n",__func__);
			return OPEN_FILE_ERR;
		}
		fwrite(appCommand,50,1,l_fd);
		fflush(l_fd);
		fclose(l_fd);		
	}


	return 0;
}



////机顶盒 自学习语音控制接口
int box_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND)
{
	int ret = 0;

	//获取电视命令
	char box_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(box_command,token);

	
	if(box_command[0] >= '0' && box_command[0] <= '9'){
		if(atoi(box_command) > 999)
			return INVALID_COMMAND;
		
		strcat(ASR_COMMAND,"已调到");
		strcat(ASR_COMMAND,box_command);
		strcat(ASR_COMMAND,"台");
		char num[5] = {0};
		num[0] = box_command[0]; 
		//发送第一位数字
		ret = Ele_send_func(t,file_name,num);
		if(ret != 0)
			return ret;
		usleep(400000);
		//发送第二位数字
		if(box_command[1] >= '0' && box_command[1] <= '9'){
			memset(num,0,5);
			num[0] = box_command[1]; 
			ret = Ele_send_func(t,file_name,num);
			if(ret != 0)
				return ret;	
			usleep(400000);
			//发送第三位数字
			if(box_command[2] >= '0' && box_command[2] <= '9'){
				memset(num,0,5);
				num[0] = box_command[2]; 
				ret = Ele_send_func(t,file_name,num);
				if(ret != 0)
					return ret;	
			}
		}
		
	}else if(strcmp(box_command,"ch-") == 0){
		strcpy(ASR_COMMAND,"已调到上一个频道");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"ch+") == 0){
		strcpy(ASR_COMMAND,"已调到下一个频道");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"vol-") == 0){

		int i;
		if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"电视音量已减小");
			i = 10;
			while(i--){
				ret = Ele_send_func(t,file_name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}

		} else if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"机顶盒音量已减小");
			i = 3;
			while(i--){
				ret = Ele_send_func(t,file_name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}

		} else {
			strcpy(ASR_COMMAND,"音量已减小");
			ret = Ele_send_func(t,file_name,box_command);
		}

	}else if(strcmp(box_command,"vol+") == 0){

		int i;
		if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"电视音量已增大");
			i = 10;
			while(i--){
				ret = Ele_send_func(t,file_name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}
			
		} else if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"机顶盒音量已增大");
			i = 3;
			while(i--){
				ret = Ele_send_func(t,file_name,box_command);
				if(ret != 0)
					break;
				usleep(100000);
			}

		} else {
			strcpy(ASR_COMMAND,"音量已增大");
			ret = Ele_send_func(t,file_name,box_command);
		}
		
	}else if(strcmp(box_command,"up") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"down") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"left") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"right") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"exit") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"menu") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"mute") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"ok") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"power") == 0){
		if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"已打开机顶盒");
		} else if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"已打开电视");
		} else if(strcmp(t,"5") == 0){
			strcpy(ASR_COMMAND,"已打开投影仪");
		} else if(strcmp(t,"8") == 0){
			strcpy(ASR_COMMAND,"已打开红外灯");
		} else if(strcmp(t,"13") == 0){
			strcpy(ASR_COMMAND,"已打开音响");
		} else if(strcmp(t,"12") == 0){
			strcpy(ASR_COMMAND,"已打开扫地机");
		} else if(strcmp(t,"15") == 0){
			strcpy(ASR_COMMAND,"已打开空气净化器");
		} else {
			return INVALID_COMMAND;
		}
		
		ret = Ele_send_func(t,file_name,box_command);
		
	}else if(strcmp(box_command,"poweroff") == 0){
		if(strcmp(t,"1") == 0 || strcmp(t,"10") == 0){
			strcpy(ASR_COMMAND,"已关闭机顶盒");
			ret = Ele_send_func(t,file_name,"power");
		} else if(strcmp(t,"2") == 0){
			strcpy(ASR_COMMAND,"已关闭电视");
			ret = Ele_send_func(t,file_name,"power");
		} else if(strcmp(t,"5") == 0){
			strcpy(ASR_COMMAND,"已关闭投影仪");
			ret = Ele_send_func(t,file_name,box_command);
		} else if(strcmp(t,"8") == 0){
			strcpy(ASR_COMMAND,"已关闭红外灯");
			ret = Ele_send_func(t,file_name,box_command);
		} else if(strcmp(t,"13") == 0){
			strcpy(ASR_COMMAND,"已关闭音响");
			ret = Ele_send_func(t,file_name,box_command);
		} else if(strcmp(t,"12") == 0){
			strcpy(ASR_COMMAND,"已关闭扫地机");
			ret = Ele_send_func(t,file_name,box_command);
		} else if(strcmp(t,"15") == 0){
			strcpy(ASR_COMMAND,"已关闭空气净化器");
			ret = Ele_send_func(t,file_name,box_command);
		} else {
			return INVALID_COMMAND;
		}

	}else if(strcmp(box_command,"signal") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"back") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");	
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"boot") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
		ret = Ele_send_func(t,file_name,box_command);
	}else if(strcmp(box_command,"noch") == 0){
		return NOCH_ERR;
	} else {
			return INVALID_COMMAND;
	}

	return ret;
}

//风扇 修改和自学习接口
int fan_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND)
{
	int ret = 0;

	//获取电视命令
	char fan_command[20] = {0};
	token = strtok( NULL,"=");
	if(token == NULL)
		return(INVALID_COMMAND);
	strcpy(fan_command,token);

	//模式
	if(strcmp(fan_command,"mode") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
	//摇头
	}else if(strcmp(fan_command,"oscillation") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
	//开关
	}else if(strcmp(fan_command,"power") == 0){
		strcpy(ASR_COMMAND,"已打开风扇");
	//开关
	}else if(strcmp(fan_command,"poweroff") == 0){
		strcpy(ASR_COMMAND,"已关闭风扇");
		strcpy(fan_command,"power");
	//定时
	}else if(strcmp(fan_command,"timer") == 0){
		strcpy(ASR_COMMAND,"设备控制成功");
	//风速
	}else if(strcmp(fan_command,"fanspeed") == 0){
		strcpy(ASR_COMMAND,"已调节风速");
	} else {
			return INVALID_COMMAND;
	}
	
	ret = Ele_send_func(t,file_name,fan_command);
	
	return ret;
}

//空调 自学习语音控制接口
int air_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND)
{
	//生成上一次红外历史命令路径
	char n_path[200] = {0};
	sprintf(n_path,"%s%s%s","/usr/data/",file_name,".record");
	char old_command[50] = {0};

	FILE *fd;

	///////////获取历史命令/////////////////////////////////////
	fd = fopen(n_path,"r+");
	if(fd == NULL){
		printf("%s: open command.record error!!!!.\n",__func__);
		return(OPEN_FILE_ERR);
	}
	fread(old_command,sizeof(old_command),1,fd);
	fclose(fd);

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

			int i = 0;
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
				if (old_command[0] == '\0'){
					if(strcmp(model,"a") == 0){
						strcpy(ASR_COMMAND,"已设到自动模式");
						return(Ele_send_func(t,file_name,"aa"));	
					} else if(strcmp(model,"d") == 0){
						strcpy(ASR_COMMAND,"已设到抽湿模式");
						return(Ele_send_func(t,file_name,"ad"));
					} else if(strcmp(model,"w") == 0){
						strcpy(ASR_COMMAND,"已设到送风模式");
						return(Ele_send_func(t,file_name,"aw"));
					} else if(strcmp(model,"h") == 0){
						strcpy(ASR_COMMAND,"已设到制热模式");
						return(Ele_send_func(t,file_name,"ah26"));
					} else if(strcmp(model,"r") == 0){
						strcpy(ASR_COMMAND,"已设到制冷模式");
						return(Ele_send_func(t,file_name,"ar26"));
					} else {
							return(INVALID_COMMAND);
					}
				
				} else {

					//printf("old_command:%s  #####\n",old_command);
	
					//判断是否自动模式
					if(strcmp(model,"a") == 0){
						strcpy(ASR_COMMAND,"已设到自动模式");
						strcpy(new_command,"aa");
						return(Ele_send_func(t,file_name,new_command));
										
					} else if(strcmp(model,"d") == 0){
						strcpy(ASR_COMMAND,"已设到抽湿模式");
						strcpy(new_command,"ad");
						return(Ele_send_func(t,file_name,new_command));	
						
					} else if(strcmp(model,"w") == 0){
						strcpy(ASR_COMMAND,"已设到送风模式");
						strcpy(new_command,"aw");
						return(Ele_send_func(t,file_name,new_command));
						
					} else if(strcmp(model,"h") == 0){
						strcpy(ASR_COMMAND,"已设到制热模式");
						if(old_command[1] == 'h' || old_command[1] == 'r'){
							old_command[1] = 'h';
							return(Ele_send_func(t,file_name,old_command));
						} else if(old_command[1] == 'a' || old_command[1] == 'd' || old_command[1] == 'w'){
							strcpy(new_command,"ah26");
							return(Ele_send_func(t,file_name,new_command));
	
						}	else {
							return(INVALID_OLD_COMMAND);
						}							
					} else if(strcmp(model,"r") == 0){
						strcpy(ASR_COMMAND,"已设到制冷模式");
						if(old_command[1] == 'h' || old_command[1] == 'r'){
							old_command[1] = 'r';

							return(Ele_send_func(t,file_name,old_command));
						} else if(old_command[1] == 'a' || old_command[1] == 'd' || old_command[1] == 'w'){
							strcpy(new_command,"ar26");

							return(Ele_send_func(t,file_name,new_command));
	
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
						if (old_command[0] == '\0'){
								sprintf(new_command,"%s%s","ar",temperature);

								return(Ele_send_func(t,file_name,new_command));
						} else {

								//获取历史命令中的模式
								if(old_command[1] == 'h' || old_command[1] == 'r'){
									  old_command[2] = temperature[0];
									  old_command[3] = temperature[1];
									  
									  return(Ele_send_func(t,file_name,old_command));	  
									  
								} else if(old_command[1] == 'a' || old_command[1] == 'd' || old_command[1] == 'w'){
									  sprintf(new_command,"%s%s","ar",temperature);

									  return(Ele_send_func(t,file_name,new_command));
								} else {
									  return(INVALID_OLD_COMMAND);
								}
								
						 }
						
					  }else if(temp > 50 && temp < 60){
							//温度升高 1度
							//如果历史命令不存在
							if (old_command[0] == '\0'){
									strcpy(ASR_COMMAND,"已设到26度");
									sprintf(new_command,"%s","ar26");
	
									return(Ele_send_func(t,file_name,new_command));
							} else {
							
									//获取历史命令中的模式
									if(old_command[1] == 'h' || old_command[1] == 'r'){
										  int old_temperature = atoi(old_command+2);
										  if((old_temperature + (temp%10))<31){
											 old_temperature += (temp%10);
											 memset(temperature,0,10);
											 sprintf(temperature,"%d",old_temperature);
											 old_command[2] = temperature[0];
											 old_command[3] = temperature[1];
										  } else if((old_temperature + (temp%10)) > 30){
											   old_temperature = 30;
											   memset(temperature,0,10);
											   sprintf(temperature,"%d",old_temperature);
											   old_command[2] = temperature[0];
											   old_command[3] = temperature[1];
	
										  } else {
											  return(INVALID_OLD_COMMAND);
										  }
	
										  strcpy(ASR_COMMAND,"已设到");
										  strcat(ASR_COMMAND,temperature);
										  strcat(ASR_COMMAND,"度"); 
										  
										  return(Ele_send_func(t,file_name,old_command));	  
										  
									} else if(old_command[1] == 'a' || old_command[1] == 'd' || old_command[1] == 'w'){
										  strcpy(new_command,"ar26");
										  strcpy(ASR_COMMAND,"已设到26度");
	
										  return(Ele_send_func(t,file_name,new_command));
									} else {
										  return(INVALID_OLD_COMMAND);
									}
									
							 }												
					  }else if(temp > 40 && temp < 50){
							//温度降低
							//如果历史命令不存在
							if (old_command[0] == '\0'){
									sprintf(new_command,"%s","ar26");
									strcpy(ASR_COMMAND,"已设到26度");	
	
									return(Ele_send_func(t,file_name,new_command));
							} else {
							
									//获取历史命令中的模式
									if(old_command[1] == 'h' || old_command[1] == 'r'){
										  int old_temperature = atoi(old_command+2);
										  if((old_temperature - (temp%10))> 15){
											 old_temperature -= (temp%10);
											 memset(temperature,0,10);
											 sprintf(temperature,"%d",old_temperature);
											 old_command[2] = temperature[0];
											 old_command[3] = temperature[1];
										  } else if((old_temperature - (temp%10)) < 16){
											  old_temperature = 16;
											  memset(temperature,0,10);
											  sprintf(temperature,"%d",old_temperature);
											  old_command[2] = temperature[0];
											  old_command[3] = temperature[1];
	
										  } else {
											  return(INVALID_OLD_COMMAND);
										  }
	
										  strcpy(ASR_COMMAND,"已设到");
										  strcat(ASR_COMMAND,temperature);
										  strcat(ASR_COMMAND,"度"); 

										  return(Ele_send_func(t,file_name,old_command));	  
										  
									} else if(old_command[1] == 'a' || old_command[1] == 'd' || old_command[1] == 'w'){
										  strcpy(new_command,"ar26");
										  strcpy(ASR_COMMAND,"已设到26度"); 
	
										  return(Ele_send_func(t,file_name,new_command));
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
				  return(Ele_send_func(t,file_name,"on"));
				  /*
				  if (old_command[0] == '\0'){
						  strcpy(new_command,"aa");
						  return(Ele_send_func(t,file_name,new_command));
				  } else {
						return(Ele_send_func(t,file_name,old_command));
	
				  }
				  */
			} else {
					//其他命令无效
					return(INVALID_COMMAND);
			}
			
	} else if(strcmp(power,"CLOSE") == 0){
		strcpy(ASR_COMMAND,"已关闭空调");
		return(Ele_send_func(t,file_name,"off"));
	} else {
		return(INVALID_COMMAND);
	}

	return 0;
}

