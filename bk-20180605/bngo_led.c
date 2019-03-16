#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bngo_info.h"



#define SYSFS_GPIO_EXPORT        "/sys/class/gpio/export"
#define SYSFS_GPIO_DIR_OUT       "out"
#define SYSFS_GPIO_PIN_VAL_H     "1"
#define SYSFS_GPIO_PIN_VAL_L     "0"


//?????? LED1 :PA9  红 wifi异常
#define SYSFS_GPIO_PIN_PA9 			"9"
#define SYSFS_GPIO_PIN_PA9_DIR      "/sys/class/gpio/gpio9/direction"
#define SYSFS_GPIO_PIN_PA9_VAL      "/sys/class/gpio/gpio9/value"

//?????? LED2 :PA7 绿 wifi正常工作
#define SYSFS_GPIO_PIN_PA7 			"7"
#define SYSFS_GPIO_PIN_PA7_DIR      "/sys/class/gpio/gpio7/direction"
#define SYSFS_GPIO_PIN_PA7_VAL      "/sys/class/gpio/gpio7/value"

//?????: LED3 :PA10  红 禁麦指示灯
#define SYSFS_GPIO_PIN_PA10 		"10"
#define SYSFS_GPIO_PIN_PA10_DIR      "/sys/class/gpio/gpio10/direction"
#define SYSFS_GPIO_PIN_PA10_VAL      "/sys/class/gpio/gpio10/value"

int echo_led_export(char *pin,char *pinDir)
{
 	if(access(pinDir,F_OK) == 0)
		return 0;
	
	/* O_WRONLY O_RDONLY  O_RDWR
	 * echo 91 > export
	 */
	int fd;
	fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	if(fd < 0){
		printf("echo_led_export:open export error\n");
		return -1;	
	}

	write(fd, pin,sizeof(pin));
	close(fd);

	/*
	 *  /sys/class/gpio/gpio91# echo out > direction
	 */
	fd = open(pinDir, O_WRONLY);
	if(fd < 0){
		printf("echo_led_export:busy pin direction open error\n");
		return -2;	
	}	

	write(fd, SYSFS_GPIO_DIR_OUT, sizeof(SYSFS_GPIO_DIR_OUT));
	close(fd);
	
    return 0;
}

int bngo_led_open(char *pin)
{
	int fd;
	if(strcmp(pin,"PA10") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA10,SYSFS_GPIO_PIN_PA10_DIR);
		fd = open(SYSFS_GPIO_PIN_PA10_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA7") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA7,SYSFS_GPIO_PIN_PA7_DIR);
		fd = open(SYSFS_GPIO_PIN_PA7_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA9") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA9,SYSFS_GPIO_PIN_PA9_DIR);
		fd = open(SYSFS_GPIO_PIN_PA9_VAL, O_WRONLY);
	} else {
		   return -1;
	}

	if(fd < 0) {
		printf("bngo_led_open: open export error.\n");
		return -1;
	}
	
	write(fd, SYSFS_GPIO_PIN_VAL_L,sizeof(SYSFS_GPIO_PIN_VAL_L));
	fsync(fd);
	close(fd);	
	return 0;
}

int bngo_led_close(char *pin)
{
	int fd;
	if(strcmp(pin,"PA10") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA10,SYSFS_GPIO_PIN_PA10_DIR);
		fd = open(SYSFS_GPIO_PIN_PA10_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA7") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA7,SYSFS_GPIO_PIN_PA7_DIR);
		fd = open(SYSFS_GPIO_PIN_PA7_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA9") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA9,SYSFS_GPIO_PIN_PA9_DIR);
		fd = open(SYSFS_GPIO_PIN_PA9_VAL, O_WRONLY);
	} else {
		   return -1;
	}

	if(fd < 0) {
		printf("bngo_led_close: open export error.\n");
		return -1;
	}
	write(fd, SYSFS_GPIO_PIN_VAL_H,sizeof(SYSFS_GPIO_PIN_VAL_H));
	fsync(fd);
	close(fd);

	return 0;
}

//夜灯
int bngo_led_write(unsigned int led_level)
{
	int fd;
	//printf("led_level:%d   ...\n",led_level);
	fd = open("/dev/led_bngo",O_RDWR);
	if(fd<0){
		printf("open /dev/led_bngo error\n");
		return -1;
	}
	write(fd,&led_level,sizeof(led_level));
	close(fd);

	return 0;
}

#if 0
//根据闹钟ID 删除对应的闹钟
int remove_Alarm(char *alarmId){
    char* file_from = "/var/spool/cron/crontabs/root";
    char* file_to = "/var/spool/cron/crontabs/root.new";

	if(access(file_from,F_OK) != 0)
		return 1;
	
    FILE *fpf, *fpt;
    char line_str[100];
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
        if(strstr(line_str,alarmId) == NULL)
            fprintf(fpt, "%s", line_str);
    }
    printf("operation done!\n");
    fclose(fpf);
    fclose(fpt);

	rename(file_to,file_from);

    return 0;	
}
#endif

//bngo按键调节夜灯亮度
int key_use_led()
{
	//设置延时熄灯标志
	bngo_info_t bngo_info_msg;
	memset(&bngo_info_msg,0,sizeof(bngo_info_msg));
	int bngo_info_cmd = -1;

	//获取夜灯历史状态
	get_bngo_info(&bngo_info_msg);

	int val = atoi(bngo_info_msg.led_val);
	if(val < 0 || val > 35)
		return -1;

	if(bngo_info_msg.led_ison == 0){
		val = 0;
	}

	switch(val){
		case 0:
		case 1:
			memcpy(bngo_info_msg.led_val,"2",strlen("2")+1);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;
			bngo_led_write(2);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			memcpy(bngo_info_msg.led_val,"12",strlen("12")+1);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;
			bngo_led_write(12);
			break;
		case 12:	
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:		
		case 18:	
		case 19:		
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:	
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
			memcpy(bngo_info_msg.led_val,"35",strlen("35")+1);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;
			bngo_led_write(35);
			break;
		case 35:
			//关夜灯
			bngo_led_write(0);
			//删除延时熄灯标志
			remove_Alarm("61578898");
			//strcpy(bngo_info_msg.led_val,"0");
			//bngo_info_cmd = 0;
			bngo_info_cmd = 11;
			bngo_info_msg.led_alarm_flag = 0;
			bngo_info_msg.led_ison = 0;
			break;
		default:
			printf("not led val .... \n");
			return -1;
			break;
	}
	
	if(bngo_info_cmd != -1)
		set_bngo_info(bngo_info_cmd,bngo_info_msg);
	
	return 0;
}

//APP按键调节夜灯亮度
int app_use_led(char *appCommand)
{
	//设置延时熄灯标志
	bngo_info_t bngo_info_msg;
	memset(&bngo_info_msg,0,sizeof(bngo_info_msg));
	int bngo_info_cmd = -1;
	if(appCommand[0] < '0' || appCommand[0] > '9')
		return -1;

	int val = atoi(appCommand);
	if(val < 0 || val > 35)
		return -1;
	
	switch(val){
		case 0:
			//关夜灯
			bngo_led_write(val);
			remove_Alarm("61578898");
		
			bngo_info_msg.led_alarm_flag = 0;
			bngo_info_msg.led_ison = 0; //off
			//memcpy(bngo_info_msg.led_val,"0",strlen("0"));
			bngo_info_cmd = 11;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:	
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:;
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
			//memcpy(bngo_info_msg.led_val,appCommand,strlen(appCommand));
			strcpy(bngo_info_msg.led_val,appCommand);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;
			bngo_led_write(val);
			break;
		default:
			printf("not led val .... \n");
			return -1;
			break;
	}
	
	if(bngo_info_cmd != -1)
		set_bngo_info(bngo_info_cmd,bngo_info_msg); 

	return 0;
}

//语音调节夜灯亮度
int server_use_led(char *new_command,char *ASR_COMMAND)
{
	bngo_info_t bngo_info_msg;
	memset(&bngo_info_msg,0,sizeof(bngo_info_msg));
	int bngo_info_cmd = -1;

	//获取夜灯历史状态
	get_bngo_info(&bngo_info_msg);

	if(strcmp(new_command,"CLOSE") == 0){
		//关夜灯
		bngo_led_write(0);
		remove_Alarm("CLOSELED");

		//延时熄灯标志
		bngo_info_msg.led_alarm_flag = 0;
		//夜灯开关标志
		bngo_info_msg.led_ison = 0; //off
		//memcpy(bngo_info_msg.led_val,"0",strlen("0"));
		bngo_info_cmd = 11;
		strcpy(ASR_COMMAND,"已关灯");
		
	} else if(strcmp(new_command,"OPEN") == 0){
		//打开夜灯
		if(strcmp(bngo_info_msg.led_val,"0") == 0){
			memcpy(bngo_info_msg.led_val,"18",sizeof("18")+1);
		}
		
		bngo_led_write(atoi(bngo_info_msg.led_val));
		bngo_info_cmd = 0;
		bngo_info_msg.led_ison = 1;  //on

		strcpy(ASR_COMMAND,"已开灯");
		
	} else if(strcmp(new_command,"UP") == 0){
		//亮一点
		if(bngo_info_msg.led_ison == 0){
			strcpy(ASR_COMMAND,"请打开夜灯，再调节亮度");
			return 0;
		}
		
		unsigned int val = atoi(bngo_info_msg.led_val);
		if(val < 0 || val > 35)
			return -1;

		if(val < 2){
			memcpy(bngo_info_msg.led_val,"2",strlen("2")+1);
			bngo_led_write(2);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;  //on
			strcpy(ASR_COMMAND,"亮一点");
		} else if(val > 1 && val < 12){
			memcpy(bngo_info_msg.led_val,"12",strlen("12")+1);
			bngo_led_write(12);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;  //on
			strcpy(ASR_COMMAND,"亮一点");
		} else if(val > 11 && val < 35){
			memcpy(bngo_info_msg.led_val,"35",strlen("35")+1);
			bngo_led_write(35);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;  //on
			strcpy(ASR_COMMAND,"亮一点");
		} else if(val == 35){
			strcpy(ASR_COMMAND,"已是最高亮度");
		}
		
		/*
		if(bngo_info_msg.led_val[0] < '5'){
			bngo_info_msg.led_val[0] += 1;
						
			bngo_led_write(atoi(bngo_info_msg.led_val));
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;  //on
			strcpy(ASR_COMMAND,"亮一点");
		} else {
			strcpy(ASR_COMMAND,"已是最高亮度");
		}
		*/
		
	} else if(strcmp(new_command,"DOWN") == 0){
		//暗一点
		if(bngo_info_msg.led_ison == 0){
			strcpy(ASR_COMMAND,"请打开夜灯，再调节亮度");
			return 0;
		}

		unsigned int val = atoi(bngo_info_msg.led_val);
		if(val < 0 || val > 35)
			return -1;

		if(val == 0){
			strcpy(ASR_COMMAND,"请打开夜灯，再调节亮度");
		} else if(val > 0 && val < 3){
			strcpy(ASR_COMMAND,"已是最低亮度");
		} else if(val > 2 && val < 13){
			memcpy(bngo_info_msg.led_val,"2",strlen("2")+1);
			
			bngo_led_write(2);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;  //on
			strcpy(ASR_COMMAND,"暗一点");
		} else if(val > 12 && val < 36){		
			memcpy(bngo_info_msg.led_val,"12",strlen("12")+1);
			bngo_led_write(12);
			bngo_info_cmd = 0;
			bngo_info_msg.led_ison = 1;  //on
			strcpy(ASR_COMMAND,"暗一点");
		} 

	} else if(strcmp(new_command,"MAX") == 0){
		//亮一点
		memcpy(bngo_info_msg.led_val,"35",sizeof("35")+1);
		bngo_info_cmd = 0;
		bngo_info_msg.led_ison = 1;  //on
		
		bngo_led_write(atoi(bngo_info_msg.led_val));
		strcpy(ASR_COMMAND,"已是最高亮度");
	} else if(strcmp(new_command,"MIN") == 0){
		//暗一点
		memcpy(bngo_info_msg.led_val,"2",sizeof("2")+1);
		
		bngo_led_write(atoi(bngo_info_msg.led_val));
		bngo_info_cmd = 0;
		bngo_info_msg.led_ison = 1;  //on
		strcpy(ASR_COMMAND,"已是最低亮度");

	} else {
		return -1;
	}

	if(bngo_info_cmd != -1)
		set_bngo_info(bngo_info_cmd,bngo_info_msg);	
	
	return 0;
}


