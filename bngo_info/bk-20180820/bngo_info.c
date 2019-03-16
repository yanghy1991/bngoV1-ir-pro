#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/wireless.h>
#include <stdlib.h>
#include <ctype.h>

#include <time.h>
#include <fcntl.h>

#include "bngo_info.h"
#include "bngo_aes.h"
#include "ini_interface.h"

#include "cJSON.h"


/* The name of the interface */
#ifndef IW_NAME
#define ifname "wlan0"
#endif
#define PROC_NET_WIRELESS	"/proc/net/wireless"


#include "bngo_info.h"

//get wifi_mac
void get_mac(char *Str)
{
	char *dest = Str;
	while(*Str != '\0')
	{
		if(*Str != ':' && *Str != 0x0a)
		{
			*dest++ = *Str;
		}
		
		++Str;
	}
	*dest = '\0';	
}

//获取设备id
int get_bngoID(char *bngoId){
	//获取设备id
	char buffer[256];
	FILE *devFd;
	
	if (access("/usr/data/BINGO_MSG.bngo",F_OK) == 0){
		memset(buffer,0,256);
		devFd = fopen("/usr/data/BINGO_MSG.bngo","r+");
		if(devFd == NULL){
			printf("read deviceID.txt file error!!\n");
			return -1;
		}
		fread(buffer,256,1,devFd);
		fclose(devFd);
		
		//解析
		char *result_json = NULL;
		result_json = strstr(buffer,"{");
		
		cJSON * root = NULL;
		cJSON * bngoid = NULL;
		root = cJSON_Parse(result_json);
		if (!root) 
		{
			//printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			printf("%s : parse json error.\n",__func__);  
			return -1;
		}

		bngoid = cJSON_GetObjectItem(root, "BINGO_ID");
		if(bngoid == NULL)
		{
			printf("%s : get err_msg failed! error:[%s]\n",__func__,cJSON_Print(bngoid));
			cJSON_Delete(root);
		} else {
			memcpy(bngoId,bngoid->valuestring,strlen(bngoid->valuestring));
			cJSON_Delete(root);
			
			
			if(strlen(bngoId) > 10){	
				return 0;					
			} else {
				printf("bngoID获取失败 bngoid:%s len:%d \n",bngoId,strlen(bngoId));
			}			
			
		}
	} 
	
	int w_fd;
	char b_buf[25] = {0};
	char deviceID[30] = "SZH1-BINGOV1-";
	
	w_fd = open("/sys/class/net/wlan0/address",O_RDONLY);

	if(w_fd < 0){
		printf("open mac file error!!\n");
		return -1;
	}
	
	read(w_fd,b_buf,25);
	get_mac(b_buf);
	close(w_fd);
	strcat(deviceID,b_buf);
	
	//strcpy(bngoId,deviceID);
	bngoID_AES(deviceID,bngoId);		
	

	///////	
	return 0;
}


//获取设备id
int get_bngoID_mqttpub(char *bngoId){
	//获取设备id
	char buffer[30];
	FILE *devFd;
	
	if (access("/usr/data/szhy_bngoid.txt",F_OK) == 0){
		memset(buffer,0,50);
		devFd = fopen("/usr/data/szhy_bngoid.txt","r+");
		if(devFd == NULL){
			printf("read deviceID.txt file error!!\n");
			return -1;
		}
		fread(buffer,30,1,devFd);
		fclose(devFd);
		
		memcpy(bngoId,buffer,30);
		
		if(strlen(buffer) > 20)
			return 0;

	} 
	
	int w_fd;
	char b_buf[25] = {0};
	char deviceID[30] = "SZH1-BINGOV1-";
	
	w_fd = open("/sys/class/net/wlan0/address",O_RDONLY);

	if(w_fd < 0){
		printf("open mac file error!!\n");
		return -1;
	}
	
	read(w_fd,b_buf,25);
	get_mac(b_buf);
	close(w_fd);
	strcat(deviceID,b_buf);
	memcpy(bngoId,deviceID,30);
	
	w_fd = open("/usr/data/szhy_bngoid.txt",O_WRONLY | O_CREAT);
	
	if(w_fd < 0){
		printf("open szhy_bngoid.txt error!!\n");
		return -1;
	}
	write(w_fd,deviceID,30);
	close(w_fd);
	
	///////	
	return 0;
}

//存在已过去闹钟 返回0  不存在 返回-1
int check_alarmID(char *line_str,char *alarmId,char *alarmType)
{	
	/*******解析bngo接受到的json数据********/	
	char *token;	
	int i = 0;
	char alarmID[64] = {0};
	char type[64] = {0};

	for(token = strtok(line_str, " "); token != NULL; token = strtok(NULL, " ")) {    
        //printf(token);
		if(i == 6)
			strcpy(alarmID,token);	
		else if(i == 9)
			strcpy(type,token);
        i++;    
    }
	//判断是否缺少参数
	if(i != 11)
		return -1;

	//printf("day:%s mon:%s alarmId:%s topic:%s Timestamps:%s\n",alarmDay,alarmMon,alarmId,topic,Timestamps);

	//如果闹钟id 、闹钟类型 相同 删除
	if(strcmp(alarmID,alarmId) != 0 || strcmp(alarmType,type) != 0){
		if(strcmp(alarmType,"apalarm") == 0){
			if(strcmp(alarmID,alarmId) != 0 || strcmp("bgalarm",type) != 0)
				return -1;
		} else {
			return -1;
		}	
	}

	
	return 0;
}

//存在bgalarm apalarm 返回0 否则返回 -1
int check_alarmType(char *line_str)
{	
	/*******解析bngo接受到的json数据********/	
	char *token;	
	int i = 0;
	char alarmType[10] = {0};
	char alarmID[64] = {0};

	for(token = strtok(line_str, " "); token != NULL; token = strtok(NULL, " ")) {    
        //printf(token);
		if(i == 6)
			strcpy(alarmID,token);	
		else if(i == 9)
			strcpy(alarmType,token);	
        i++;    
    }
	//判断是否缺少参数
	if(i != 11)
		return -1;

	//printf("day:%s mon:%s alarmId:%s topic:%s Timestamps:%s\n",alarmDay,alarmMon,alarmId,topic,Timestamps);
	//如果闹钟id 相同 删除
	if(strcmp(alarmType,"apalarm") != 0 && strcmp(alarmType,"bgalarm") != 0)
		return -1;
	
	//删除服务器数据
	//if(szhy_get("del_alarm",alarmID) != 0)
		//return -1;

	return 0;
}

//根据闹钟ID 删除对应的闹钟
int remove_Alarm(char *alarmId,char *alarmType)
{
    char* file_from = "/var/spool/cron/crontabs/root";
    char* file_to = "/var/spool/cron/crontabs/root.new";

	if(access(file_from,F_OK) != 0)
		return 1;
	
    FILE *fpf, *fpt;
    char line_str[100];
	char root_str[100];
    fpf = fopen(file_from, "r+");
    fpt = fopen(file_to, "w+");
    if(fpf == NULL || fpt == NULL)
    {
        printf("open file error!\n");
        return 2;
    }
	//获取一行
    while((fgets(line_str,100,fpf)) != NULL)
    {	
        //if(strstr(line_str,alarmId) == NULL)
            //fprintf(fpt, "%s", line_str);
		strcpy(root_str,line_str);
		if(check_alarmID(root_str,alarmId,alarmType) != 0)
			 fprintf(fpt, "%s", line_str);
    }
    fclose(fpf);
    fclose(fpt);

	rename(file_to,file_from);
    return 0;	
}

//删除所有闹钟 apalarm bgalarm
int remove_ALLalarm()
{
    char* file_from = "/var/spool/cron/crontabs/root";
    char* file_to = "/var/spool/cron/crontabs/root.new";

	if(access(file_from,F_OK) != 0)
		return 1;
	
    FILE *fpf, *fpt;
    char line_str[100];
	char root_str[100];
    fpf = fopen(file_from, "r+");
    fpt = fopen(file_to, "w+");
    if(fpf == NULL || fpt == NULL)
    {
        printf("open file error!\n");
        return 2;
    }
	//获取一行
    while((fgets(line_str,100,fpf)) != NULL)
    {	
		strcpy(root_str,line_str);
		if(check_alarmType(root_str) != 0)
			 fprintf(fpt, "%s", line_str);
    }
    fclose(fpf);
    fclose(fpt);

	rename(file_to,file_from);
    return 0;	
}

//获取时间戳
int get_Timestamps(char *startDate)
{

	struct tm tmTemp;

	//char srcTime[64] = "2018-05-23 16:04";
	char srcTime[64] = {0};
	strcpy(srcTime,startDate);
	strptime(srcTime,"%Y-%m-%d %H:%M",&tmTemp);
	//printf("year:%d mon:%d day:%d hour:%d min:%d sec:%d\n",tmTemp.tm_year,tmTemp.tm_mon,tmTemp.tm_mday,tmTemp.tm_hour,tmTemp.tm_min,tmTemp.tm_sec);
	tmTemp.tm_sec = 0;
	
	time_t t  = mktime(&tmTemp);
	
	int i = t;
	//*Timestamps = i;
	
	//printf("t:%ld i:%d \n",t,i);

	return i;
}

int set_bngo_info(int cmd,bngo_info_t bngo_info_msg)
{
	char* bngo_info_path = "/usr/data/bngo_info.txt";
	bngo_info_t bngo_info_new;
	memset(&bngo_info_new,0,sizeof(bngo_info_new));

	FILE *fd;

	//////判断bngo info 信息是否存在///////////////
	if (access(bngo_info_path,F_OK) != 0){
		/*************写入 bngo 信息*******************/
		fd = fopen(bngo_info_path,"w+");
		if(fd == NULL){
			printf("%s: open bngo_info.txt!!!!.\n",__func__);
			return -1;
		}
		fwrite(&bngo_info_msg,sizeof(bngo_info_msg),1,fd);
		fflush(fd); 
		fclose(fd);
		
	} else {

		/*************读出 bngo 信息*******************/
		fd = fopen(bngo_info_path,"r+");
		if(fd == NULL){
			printf("%s: open bngo_info.txt!!!!.\n",__func__);
			return -1;
		}
		fread(&bngo_info_new,sizeof(bngo_info_new),1,fd);
		fclose(fd);


		switch(cmd){
			case 0:
				memcpy(bngo_info_new.led_val,bngo_info_msg.led_val,sizeof(bngo_info_msg.led_val));
				bngo_info_new.led_ison = bngo_info_msg.led_ison;
				break;
			case 1:
				bngo_info_new.led_alarm_flag = bngo_info_msg.led_alarm_flag;
				break;
			case 2:
				bngo_info_new.sleep_flag = bngo_info_msg.sleep_flag;
				break;
			case 3:
				bngo_info_new.time_flag = bngo_info_msg.time_flag;
				break;
			case 4:
				bngo_info_new.night_flag = bngo_info_msg.night_flag;
				break;
			case 5:
				bngo_info_new.sleepMusic_flag = bngo_info_msg.sleepMusic_flag;
				break;
			case 11:
				//同时修改led_ison 和延时熄灯标志
				//memcpy(bngo_info_new.led_val,bngo_info_msg.led_val,sizeof(bngo_info_msg.led_val));
				bngo_info_new.led_alarm_flag = bngo_info_msg.led_alarm_flag;
				bngo_info_new.led_ison = bngo_info_msg.led_ison;
				
				break;
				
			default:
				printf("bngo info 结构体没有该参数\n");
				break;
		}

		/*************写入 bngo 信息*******************/
		fd = fopen(bngo_info_path,"w+");
		if(fd == NULL){
			printf("%s: open bngo_info.txt!!!!.\n",__func__);
			return -1;
		}
		fwrite(&bngo_info_new,sizeof(bngo_info_new),1,fd);
		fflush(fd); 
		fclose(fd);
	}

	return 0;
}


int get_bngo_info(bngo_info_t *bngo_info_msg)
{
	FILE *fd;
	
	char* bngo_info_path = "/usr/data/bngo_info.txt";
	memset(bngo_info_msg,0,sizeof(bngo_info_t));

	
	//////判断bngo info 信息是否存在///////////////
	if (access(bngo_info_path,F_OK) != 0){
		memcpy(bngo_info_msg->led_val,"0",strlen("0")+1);
		bngo_info_msg->time_flag = 1;
	} else {
		/*************读出 bngo 信息*******************/
		fd = fopen(bngo_info_path,"r+");
		if(fd == NULL){
			printf("%s: open bngo_info.txt!!!!.\n",__func__);
			return -1;
		}
		fread(bngo_info_msg,sizeof(bngo_info_t),1,fd);
		fclose(fd);
	}
	return 0;
}

//获取音乐助眠参数
int get_sleepMusic_info(sds_sleepmusic_file *bngo_sleepMusic_msg)
{
	FILE *fd;
	
	char* bngo_sleepMusic_path = "/usr/data/sleep_music.txt";
	memset(bngo_sleepMusic_msg,0,sizeof(sds_sleepmusic_file));

	
	//////判断bngo info 信息是否存在///////////////
	if (access(bngo_sleepMusic_path,F_OK) != 0){
		//音频选择1
		strcpy(bngo_sleepMusic_msg->sm_source,"1");
		//音频播放时长
		strcpy(bngo_sleepMusic_msg->sm_times,"15");
		//开启音量渐变
		strcpy(bngo_sleepMusic_msg->sm_volume_flag,"1");
		//开始音量
		strcpy(bngo_sleepMusic_msg->sm_volume_start,"80");
		//开启夜灯渐变
		strcpy(bngo_sleepMusic_msg->sm_led_flag,"1");
		//开始夜灯亮度
		strcpy(bngo_sleepMusic_msg->sm_led_start,"30");
	} else {
		/*************读出 bngo 信息*******************/
		fd = fopen(bngo_sleepMusic_path,"r+");
		if(fd == NULL){
			printf("%s: open bngo_info.txt!!!!.\n",__func__);
			return -1;
		}
		fread(bngo_sleepMusic_msg,sizeof(sds_sleepmusic_file),1,fd);
		fclose(fd);
	}
	return 0;
}



//获取设备名称
int get_device_name(char *name)
{
	if (access("/usr/data/DEVICENAME",F_OK) != 0){
		strcpy(name,"BNGO");
		return 0;
	}
	
	FILE *fd;
	/*************记录历史命令*******************/
	fd = fopen("/usr/data/DEVICENAME","r+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		return -1;
	}
	fread(name,128,1,fd);
	fclose(fd);

	//printf("[%s]: ok ########## \n",__func__);
	return 0;
}

//获取夜间模式
int get_night_volume(int *volume,char *led_val)
{
	if (access("/usr/data/NIGHTMODE_ON.txt",F_OK) != 0){
		*volume = 20;
		strcpy(led_val,"5");
		return 0;
	}
	
	FILE *fd;
	/*************记录历史命令*******************/
	fd = fopen("/usr/data/NIGHTMODE_ON.txt","r+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		return -1;
	}
	fread(volume,sizeof(int),1,fd);
	fread(led_val,10,1,fd);
	fclose(fd);
	
	if(led_val[0] < '0' || led_val[0] > '9'){
		strcpy(led_val,"5");
	}

	return 0;
}

int get_bngo_city(char *city)
{
	//获取 BINGO_MSG.txt 中的 location
	FILE *fd;
	char buffer[256] = {0};
	fd = fopen("/usr/data/BINGO_MSG.bngo","r+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		return -1;
	}
	fread(buffer,256,1,fd);
	fclose(fd);

	char *json_src = NULL;
	json_src = strstr(buffer,"{");
	if(json_src == NULL){
		return -1;
	}

	cJSON * root = NULL;
	root = cJSON_Parse(json_src);
	if (!root) 
	{
		printf("%s : parse json error.\n",__func__);  
		return -1;
	}
	
	cJSON * msg = NULL;
	msg = cJSON_GetObjectItem(root, "location");
	if(NULL == msg)
	{
		printf("%s : get location failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	//printf("err_msg : %s\n", cJSON_Print(err_msg));
		
	memcpy(city,msg->valuestring,strlen(msg->valuestring)+1);

	if(root){
		cJSON_Delete(root);
	}

	return 0;
}

//修改设备名称
int set_bngo_name(char *name,char *ASR_COMMAND)
{
	if(strcmp(name,"") == 0){
		memcpy(ASR_COMMAND,"修改失败，设备名称不能为空",strlen("设备名称不能为空")+1);
		return -1;
	}
	
	//修改设备名称
	FILE *fd;
	/*************记录历史命令*******************/
	fd = fopen("/usr/data/DEVICENAME","w+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
	}
	fwrite(name,strlen(name),1,fd);
	fclose(fd);


	//修改 BINGO_MSG.txt 中的 NAME
	char buffer[256] = {0};
	fd = fopen("/usr/data/BINGO_MSG.bngo","r+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
	}
	
	fread(buffer,256,1,fd);
	fclose(fd);

	char *json_src = NULL;
	json_src = strstr(buffer,"{");
	if(json_src == NULL){
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
	}

	cJSON * root = NULL;
	//cJSON * c_name = NULL;
	root = cJSON_Parse(json_src);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
    }

	cJSON_ReplaceItemInObject(root, "NAME", cJSON_CreateString(name));
	
	char *s = cJSON_PrintUnformatted(root);
	
	fd = fopen("/usr/data/BINGO_MSG.bngo","w+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		free(s);
		cJSON_Delete(root);
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
	}
	fwrite(s,strlen(s),1,fd);
	fclose(fd);


	//修改 tcp_info.txt 中的 NAME
	char buffer1[256] = {0};
	fd = fopen("/usr/data/tcp_info.txt","r+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		free(s);
		cJSON_Delete(root);
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
	}
	fread(buffer1,256,1,fd);
	fclose(fd);

	json_src = strstr(buffer1,"{");
	if(json_src == NULL){
		free(s);
		cJSON_Delete(root);
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
	}

	cJSON * root_tcp = NULL;
	//cJSON * c_name = NULL;
	root_tcp = cJSON_Parse(json_src);
	if (!root_tcp) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__); 
		free(s);		
		cJSON_Delete(root);
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
    }

	cJSON_ReplaceItemInObject(root_tcp, "NAME", cJSON_CreateString(name));
	
	char *s_tcp = cJSON_PrintUnformatted(root_tcp);
	
	fd = fopen("/usr/data/tcp_info.txt","w+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		free(s);
		free(s_tcp);
		cJSON_Delete(root);
		cJSON_Delete(root_tcp);
		memcpy(ASR_COMMAND,"设备名称修改失败",strlen("设备名称修改失败")+1);
		return -1;
	}
	fwrite(s_tcp,strlen(s_tcp),1,fd);
	fclose(fd);

	memcpy(ASR_COMMAND,"设备名称修改成功",strlen("设备名称修改成功")+1);


	if(s){
		free(s);
	}
	
	if(s_tcp){
		free(s_tcp);
	}
	
	if(root_tcp){
		cJSON_Delete(root_tcp);
	}
	
	if(root){
		cJSON_Delete(root);
	}

	return 0;
}


int makeWifiJson(char *p,const char *ssid,unsigned char dbm)
{
	cJSON *root = NULL;
	
	root = cJSON_CreateObject(); // 创建根
	if (!root)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
        return -1;  
    } 

	//char led_val[10] = {0};
	//int led_status = 0;
	char name[128] = {0};
	if(get_device_name(name) != 0){
		cJSON_Delete(root); 
		return -1;
	}
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);
	
	//获取夜间模式音量
	int night_val = 0;
	char led_val[10] = {0};
	if(get_night_volume(&night_val,led_val) != 0){
		cJSON_Delete(root); 
		return -1;
	}
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);	
	
	// 方法 使用cJSON_AddItemToObject，推荐使用 
	cJSON_AddItemToObject(root, "SSID", cJSON_CreateString(ssid));
	cJSON_AddItemToObject(root, "DBM", cJSON_CreateNumber(dbm)); 
	cJSON_AddItemToObject(root, "NAME", cJSON_CreateString(name));



	/////////获取bngo info 结构体//////////////////////////
	//char* bngo_info_path = "/usr/data/bngo_info.txt";
	bngo_info_t bngo_info_new;
	memset(&bngo_info_new,0,sizeof(bngo_info_new));

	if(get_bngo_info(&bngo_info_new) != 0){
		cJSON_Delete(root); 
		return -1;
	}
	
	/////////获取sds_sleepmusic_file 结构体//////////////////////////
	sds_sleepmusic_file sds_sleepmusic_msg;
	memset(&sds_sleepmusic_msg,0,sizeof(sds_sleepmusic_file));

	if(get_sleepMusic_info(&sds_sleepmusic_msg) != 0){
		cJSON_Delete(root); 
		return -1;
	}
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);	
	
	if(bngo_info_new.led_ison == 0)
		cJSON_AddItemToObject(root, "LED_VAL", cJSON_CreateString("0"));
	else
		cJSON_AddItemToObject(root, "LED_VAL", cJSON_CreateString(bngo_info_new.led_val));
	
	cJSON_AddItemToObject(root, "LED_ALARM", cJSON_CreateBool(bngo_info_new.led_alarm_flag)); 
	cJSON_AddItemToObject(root, "SLEEP_FLAG", cJSON_CreateBool(bngo_info_new.sleep_flag)); 
	cJSON_AddItemToObject(root, "TIME_FLAG", cJSON_CreateBool(bngo_info_new.time_flag)); 
	cJSON_AddItemToObject(root, "NIGHT_FLAG", cJSON_CreateBool(bngo_info_new.night_flag));
	cJSON_AddItemToObject(root, "SLEEPMUSIC_FLAG", cJSON_CreateBool(bngo_info_new.sleepMusic_flag));
	
	//返回音乐助眠参数
	cJSON_AddItemToObject(root, "SM_SOURCE", cJSON_CreateString(sds_sleepmusic_msg.sm_source));
	cJSON_AddItemToObject(root, "SM_TIMES", cJSON_CreateString(sds_sleepmusic_msg.sm_times));
	cJSON_AddItemToObject(root, "SMVOLUME_FLAG", cJSON_CreateString(sds_sleepmusic_msg.sm_volume_flag));
	cJSON_AddItemToObject(root, "SMVOLUME_START", cJSON_CreateString(sds_sleepmusic_msg.sm_volume_start));
	cJSON_AddItemToObject(root, "SMLED_FLAG", cJSON_CreateString(sds_sleepmusic_msg.sm_led_flag));
	cJSON_AddItemToObject(root, "SMLED_START", cJSON_CreateString(sds_sleepmusic_msg.sm_led_start));
	
	char vol_buf[8] = {0};
	if (mozart_ini_getkey("/usr/data/system.ini", "volume", "music", vol_buf)) {
		printf("failed to parse /usr/data/system.ini, set music volume to 20.\n");
		strcpy(vol_buf,"80");
	}
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);	
	
	cJSON_AddItemToObject(root, "MUSIC_VOLUME", cJSON_CreateString(vol_buf));
	memset(vol_buf,0,sizeof(vol_buf));
	if (mozart_ini_getkey("/usr/data/system.ini", "volume", "bt_call", vol_buf)) {
		printf("failed to parse /usr/data/system.ini, set bt_call volume to 20.\n");
		strcpy(vol_buf,"80");
	}
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);	
	cJSON_AddItemToObject(root, "ALARM_VOLUME", cJSON_CreateString(vol_buf));
	
	cJSON_AddItemToObject(root, "NIGHT_VOLUME", cJSON_CreateNumber(night_val)); 
	cJSON_AddItemToObject(root, "NIGHT_LED", cJSON_CreateString(led_val));
	
	
	
	//printf("\n######led_val:%s led_ison:%d led_alarm_flag:%d sleep_flag:%d time_flag:%d night_flag:%d #########\n",bngo_info_new.led_val,bngo_info_new.led_ison,bngo_info_new.led_alarm_flag,bngo_info_new.sleep_flag,bngo_info_new.time_flag,bngo_info_new.night_flag);
	//////////////////////////////////////////////////


	//////////////////////返回bngo info///////////////////////////////////
	char *s = cJSON_PrintUnformatted(root);

	memcpy(p,s,strlen(s)+1);
	//printf("p:%s\n",p);

	if(s)
		free(s);
	
	if(root)
		cJSON_Delete(root);	
	return 0;
}


int get_wifi_signal(unsigned char *dbm)
{
	struct iw_statistics stats;
	memset(&stats,0,sizeof(stats));
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);
	//获取signal
	FILE *	f = fopen(PROC_NET_WIRELESS, "r");      
	char	buf[256] = {0};      
	char *	bp = NULL;      
	int	t = 0;      
	if(f==NULL)			
		return -1;      /* Loop on all devices */      
	while(fgets(buf,255,f))	{	   
		bp=buf;	  
		while(*bp&&isspace(*bp))	    
			bp++;	  /* Is it the good device ? */	 
		
		//printf("### [%s] [%d] [bp:%s] ### \n",__func__,__LINE__,bp);
		
		if(strncmp(bp,ifname,strlen(ifname))==0 && bp[strlen(ifname)]==':')	    {
			//printf("### [%s] [%d] ### \n",__func__,__LINE__);
			/* Skip ethX: */	      
			bp=strchr(bp,':');	      
			bp++;	     
			/* -- status -- */	      
			bp = strtok(bp, " ");
			if(bp == NULL){
				fclose(f);
				return -1;
			}
			sscanf(bp, "%X", &t);	      
			stats.status = (unsigned short) t;	
			
			//printf("### [%s] [%d] [stats.status:%X] ### \n",__func__,__LINE__,t);
			/* -- link quality -- */	      
			bp = strtok(NULL, " ");	
			if(bp == NULL){
				fclose(f);
				return -1;
			}
			
			if(strchr(bp,'.') != NULL)		
				stats.qual.updated |= 1;
			
			sscanf(bp, "%d", &t);	      
			stats.qual.qual = (unsigned char) t;
			//printf("### [%s] [%d] [qual:%d] ### \n",__func__,__LINE__,t);
			
			/* -- signal level -- */	      
			bp = strtok(NULL, " ");	 
			if(bp == NULL){
				fclose(f);
				return -1;
			}
			
			if(strchr(bp,'.') != NULL)		
				stats.qual.updated |= 2;	      
			sscanf(bp, "%d", &t);	      
			stats.qual.level = (unsigned char) t;
			
			//printf("### [%s] [%d] [level:%d] ### \n",__func__,__LINE__,t);
			
			#if 0
			/* -- noise level -- */	      
			bp = strtok(NULL, " ");	
			if(bp == NULL){
				fclose(f);
				return -1;
			}	
			
			if(strchr(bp,'.') != NULL)		
				stats.qual.updated += 4;	      
			sscanf(bp, "%d", &t);	      
			stats.qual.noise = (unsigned char) t;	

			printf("### [%s] [%d] [noise:%d] ### \n",__func__,__LINE__,t);	
			
			/* -- discarded packets -- */	      
			bp = strtok(NULL, " ");	      
			sscanf(bp, "%d", &stats.discard.nwid);	      
			bp = strtok(NULL, " ");	      
			sscanf(bp, "%d", &stats.discard.code);	      
			bp = strtok(NULL, " ");	      
			sscanf(bp, "%d", &stats.discard.misc);
			#endif
			
			fclose(f);	      
			/* No conversion needed */		
				//creak json
			*dbm = stats.qual.level;
			//printf("### [%s] [%d] [dbm:%d] ### \n",__func__,__LINE__,*dbm);
			return 0;	    
		}	
	} 
	
	fclose(f);	  
	return -1;
}


int get_wifi_ssid(char *ssid)
{
	int sockfd;
	
	struct iwreq req;	
	memset(&req, 0, sizeof(struct iwreq));	
	sprintf(req.ifr_name, ifname);

	/* Any old socket will do, and a datagram socket is pretty cheap */
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		perror("Could not create simple datagram socket");
		return -1;
	}


	//if not write these codes , the program maybe wrong.
	char buffer[100] = {0};
	req.u.essid.pointer = buffer;
	req.u.essid.length = 100;

	if(ioctl(sockfd, SIOCGIWESSID, &req) == -1) 
	{
		perror("Error performing SIOCGIWESSID");
		close(sockfd);
		return -1;
	}

	strcpy(ssid,buffer);
	
	close(sockfd);

	return 0;
}


int send_bngo_info(char *ASR_COMMAND)
{
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);
	
	char buffer[100] = {0};
	if(get_wifi_ssid(buffer) != 0)
		return -1;
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);
	
	unsigned char dbm = 0;
	if(get_wifi_signal(&dbm) != 0)
		return -1;
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);
	
	if(makeWifiJson(ASR_COMMAND,buffer,dbm) != 0){
		printf("[%s] get msg error \n",__func__);
		return -1;
	}
	
	//printf("### [%s] [%d] ### \n",__func__,__LINE__);
	/*
	printf("Signal level%s is %d%s.\n",
	       (stats.qual.updated & IW_QUAL_DBM ? " (in dBm)" :""),
	       (signed char)stats.qual.level,
	       (stats.qual.updated & IW_QUAL_LEVEL_UPDATED ? " (updated)" :""));
	*/
	return 0;
}