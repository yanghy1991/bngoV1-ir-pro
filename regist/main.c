#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#include "szhy_get.h"
#include "cJSON.h"

int main(int argc,char *argv[])
{
	char strResponse[5120] = {0};
	if(get_time_server(strResponse) == 0){
		char *json_str = NULL;
		json_str = strstr(strResponse,"{");
		
		if(json_str == NULL)
			return -1;
		
		cJSON * root = NULL;
		cJSON * jdata = NULL;
		cJSON * code = NULL;
		
		root = cJSON_Parse(json_str);
		if (!root) 
		{
			//printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			printf("%s : parse json error.\n",__func__);  
			return -1;
		}
		else
		{
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
			
			jdata = cJSON_GetObjectItem(root, "data");
			if(jdata == NULL)
			{
				printf("%s : get data failed! error\n",__func__);
				cJSON_Delete(root);
				return -1;//获取err_msg失败
			}
			//memcpy(ret_msg,msg->valuestring,strlen(msg->valuestring));
			
			char cmdStr[512] = {0};
			strcat(cmdStr,"date -s \"");
			strcat(cmdStr,jdata->valuestring);
			strcat(cmdStr,"\"");
			
			//printf("date:%s \n",cmdStr);
			
			if(system(cmdStr) != 0){
				printf("%s : system error.\n",__func__);  
				cJSON_Delete(root);
				return -1;						
			}
			
			
			if(access("/var/spool/cron/crontabs/root",F_OK) != 0){
				//一小时更新一次时间
				system("echo \"0 6 * * * /usr/fs/usr/sbin/bngoSetTime\" >> /var/spool/cron/crontabs/root");	
			}
			
			system("sync");
			
		}
		cJSON_Delete(root);
		
	} else {
		printf("[%s] set time error ######\n",__func__);
		return -1;		
	}
		
	/*
	
	if(http_get("http://47.107.25.1:80/irelf/v1/app/getDate?",strResponse) == 0){
		
		char *json_str = NULL;
		json_str = strstr(strResponse,"{");
		
		if(json_str == NULL)
			return -1;
		
		cJSON * root = NULL;
		cJSON * jdata = NULL;
		cJSON * code = NULL;
		
		root = cJSON_Parse(json_str);
		if (!root) 
		{
			//printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			printf("%s : parse json error.\n",__func__);  
			return -1;
		}
		else
		{
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
			
			jdata = cJSON_GetObjectItem(root, "data");
			if(jdata == NULL)
			{
				printf("%s : get data failed! error\n",__func__);
				cJSON_Delete(root);
				return -1;//获取err_msg失败
			}
			//memcpy(ret_msg,msg->valuestring,strlen(msg->valuestring));
			
			char cmdStr[512] = {0};
			strcat(cmdStr,"date -s \"");
			strcat(cmdStr,jdata->valuestring);
			strcat(cmdStr,"\"");
			
			//printf("date:%s \n",cmdStr);
			
			if(system(cmdStr) != 0){
				printf("%s : system error.\n",__func__);  
				cJSON_Delete(root);
				return -1;						
			}
			
			
			if(access("/var/spool/cron/crontabs/root",F_OK) != 0){
				//一小时更新一次时间
				system("echo \"0 6 * * * /usr/fs/usr/sbin/bngoSetTime\" >> /var/spool/cron/crontabs/root");	
			}
			
			system("sync");
			
		}
		cJSON_Delete(root);
		
	} else {
		printf("[%s] set time error ######\n",__func__);
		return -1;	
	}
	*/
	//printf("strResponse:%s \n",strResponse);
	
	printf("bngo set time success #####\n");
	return 0;
}
