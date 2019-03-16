#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#include "http_get.h"
#include "cJSON.h"

#define SERVER_IP "http://47.107.25.1:80"

//json 解析
//{"errId":0,"errMsg":null,"input":null,"results":null,"code":2000,"message":"success","data":"删除成功"}

int szhy_content_json(char *json_str)
{
	cJSON * root = NULL;
	cJSON * code = NULL;
	cJSON * jerror = NULL;
	
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	else
	{
		jerror = cJSON_GetObjectItem(root, "data");
		if(jerror == NULL)
		{
			cJSON_Delete(root);
			printf("%s : get ret_code failed!\n",__func__);
			return -1;//获取err_msg失败
		}
		
		
		code = cJSON_GetObjectItem(root, "code");
		if(NULL == code)
		{
			cJSON_Delete(root);
			printf("%s : get ret_code failed!\n",__func__);
			return -1;//获取err_no失败
		}
		//printf("err_no : %d\n", err_no->valueint);
		
		if(code->valueint != 2000){
			cJSON_Delete(root);
			return -1;
		}
			
	}

	cJSON_Delete(root);
	return 0;
}

int szhy_get(char *cmd,char *data)
{
	char strResponse[5120] = {0};
	char src_cmd[5120] = {0};
	char *result_json = NULL;
	
	if(strcmp(cmd,"del_alarm") == 0){
		strcat(src_cmd,SERVER_IP);
		strcat(src_cmd,"/irelf/v1/alarm/delAlarmclock?alarmclockid=");
		strcat(src_cmd,data);

		if(http_get(src_cmd,strResponse) != 0){
			printf("[%s] [%s] 删除闹钟失败 ######\n",__func__,strResponse);
			return -1;	
		} 
		
		result_json = strstr(strResponse,"{");	
		if(szhy_content_json(result_json) != 0){
			printf("[%s] [%s] 删除闹钟失败 ######\n",__func__,strResponse);
			return -1;	
		}
		
		//printf("删除闹钟成功 [%s]\n",strResponse);
		
		
	} else if(strcmp(cmd,"del_cozy") == 0){
		strcat(src_cmd,SERVER_IP);
		strcat(src_cmd,"/irelf/v1/timedevice/delete?Deviceclockid=");
		strcat(src_cmd,data);
		
		if(http_get(src_cmd,strResponse) != 0){
			printf("[%s] [%s] 删除COZY失败 ######\n",__func__,strResponse);
			return -1;	
		} 
		
		result_json = strstr(strResponse,"{");	
		if(szhy_content_json(result_json) != 0){
			printf("[%s] [%s] 删除COZY失败 ######\n",__func__,strResponse);
			return -1;	
		}
		
		//printf("删除COZY成功 [%s]\n",strResponse);
			
		
	} else if(strcmp(cmd,"del_code") == 0){
		strcat(src_cmd,SERVER_IP);
		strcat(src_cmd,"/irelf/v1/irDevice/delUserDevice?aircodeerid=");
		strcat(src_cmd,data);
		
		if(http_get(src_cmd,strResponse) != 0){
			printf("[%s] [%s] 删除码库失败 ######\n",__func__,strResponse);
			return -1;	
		} 
		
		result_json = strstr(strResponse,"{");	
		if(szhy_content_json(result_json) != 0){
			printf("[%s] [%s] 删除码库失败 ######\n",__func__,strResponse);
			return -1;	
		}
	
	} else if(strcmp(cmd,"bngoReset") == 0){
		char *token = NULL;
		char tokenSrc[256] = {0};
		token = strtok(data,"=");
		if(token == NULL){
			return -1;
		}
		
		strcpy(tokenSrc,token);
		if(strcmp(tokenSrc,"bngoReset") != 0)
			return -1;
		
		//获取手机号码
		token = strtok(NULL,"=");
		if(token == NULL){
			return -1;
		}
		strcpy(tokenSrc,token);
		
		strcat(src_cmd,SERVER_IP);
		strcat(src_cmd,"/irelf/v1/appUser/resetBngo?phonenumber=");
		strcat(src_cmd,tokenSrc);
		strcat(src_cmd,"&bngoid=");
		
		//获取bngoid
		token = strtok(NULL,"=");
		if(token == NULL){
			return -1;
		}
		strcpy(tokenSrc,token);
		
		strcat(src_cmd,tokenSrc);
		//printf("### [%s] [%d] [URL:%s] bngoReset ###\n",__func__,__LINE__,src_cmd);

		
		if(http_get(src_cmd,strResponse) != 0){
			printf("[%s] [%s] 恢复出厂设置失败 ######\n",__func__,strResponse);
			return -1;	
		} 
		
		result_json = strstr(strResponse,"{");	
		if(szhy_content_json(result_json) != 0){
			printf("[%s] [%s] 恢复出厂设置失败 ######\n",__func__,strResponse);
			return -1;	
		}	
	}
	return 0;
}

int get_content_server(char* asr_str,char *rsp)
{
	char cmd_urlencode[512] = {0};

	//base64 加密city
	URLEncode(asr_str, strlen(asr_str), cmd_urlencode, 512);
	//base64_encode((unsigned char *)asr_str,cmd_base64_encode, strlen(asr_str));
	
	char bngoId[100] = {0};
	get_bngoID(bngoId);
	//printf("bngoId:%s \n",bngoId);
	
	char message[10240];					
	memset(message,0,10240);							
									
	char url[1024];							
	memset(url,0,1024);	
	
	strcat(url,SERVER_IP);
	strcat(url,"/irelf/v1/nlp/command?");

	//strcat(url,"47.107.25.1:80/irelf/v1/nlp/command?");
	strcat(url,"bngoid=");
	strcat(url,bngoId);
	
	strcat(url,"&cmd=");							
	strcat(url,cmd_urlencode);
	
	printf("\n### [%s] [%d] url:%s ###\n",__func__,__LINE__,url);
	
	if(http_get(url,rsp) != 0){
		printf("[%s] [%s] 连接合一服务器失败 ######\n",__func__,rsp);
		return -1;	
	} 
	
	return 0;
}

//从服务器获取时间
int get_time_server(char *rsp)
{
	char src_cmd[5120] = {0};
	
	strcat(src_cmd,SERVER_IP);
	strcat(src_cmd,"/irelf/v1/app/getDate?");
	
	if(http_get(src_cmd,rsp) != 0)
		return -1;
	
	return 0;
}


