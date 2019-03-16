#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_get.h"

//如果是正式服务器 注释该行
#define __DEBUG__

#ifdef __DEBUG__
	#define DEBUG(format, ...) printf("[%s : %s : %d] ",__FILE__,__func__,__LINE__); printf(format, ##__VA_ARGS__);
#else
	#define DEBUG(format, ...)
#endif

#define SERVER_IP "http://47.107.25.1:80"


//从服务器获取时间
int main(int argc,char *argv[])
{
	char rsp[5120] = {0};
	#if 1
	if(http_get("https://example.com/",rsp) != 0)
		return -1;
	
	DEBUG("rsp:%s \n",rsp);
	#endif
	
	/*
	if(http_get("http://47.107.25.1:80/irelf/v1/device/register?bngoid=123&city=123",rsp) != 0)
		return -1;
	*/
	
	DEBUG("rsp:%s \n",rsp);
	
	return 0;
}
