#include "city_sohu.h"

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int address_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//不是
    ptmp = (char*)strstr(response,"HTTP/1.1");  
    if(!ptmp){  
        printf("%s : http/1.1 not find.\n",__func__);  
        return -1;    
    }
	//HTTP访问出错
    if(atoi(ptmp + 9)!=200){  
        printf("%s : http response data error.\n",__func__);
        return -1;  
    }

	//没有json
	ptmp = (char*)strstr(response,"text/json");
	if(!ptmp)
	{
		printf("%s : no json return.\n",__func__);
		return -1; 
	}
	
	return 0;
} 

/**
*功能:发起http请求，获取响应数据
*参数:
*url:http请求地址
*city:用来存储city
*
*/
int sohu_get_city(char *city)
{
    int socket_fd;//socket打开文件符
    struct sockaddr_in socaddr;

	//创建URL request信息
	char request[SOHU_COM_BUFSIZE];
    memset(request,0,SOHU_COM_BUFSIZE);
    strcat(request,"GET ");
    strcat(request,"/irelf/v1/app/getDate?");
    strcat(request," HTTP/1.1\r\n");//至此为http请求行的信息
    strcat(request,"HOST: ");
    strcat(request,"47.107.25.1");
    strcat(request,"\r\n");
    strcat(request,"Cache-Control: no-cache\r\n\r\n");
	
    //创建socket连接
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == socket_fd)
    {
        printf("%s : create socket failed of client!\n",__func__);
        return -1;//socket创建失败
    }

	
	struct timeval time_out;
	time_out.tv_sec = 15;
	time_out.tv_usec = 0;		//设置超时时间5秒
	setsockopt(socket_fd,SOL_SOCKET,SO_RCVTIMEO,&time_out,sizeof(time_out));	//设置接收数据超时
	setsockopt(socket_fd,SOL_SOCKET,SO_SNDTIMEO,&time_out,sizeof(time_out));	//设置发送数据超时
 
    //设置IP地址结构
    bzero(&socaddr,sizeof(struct sockaddr_in));
    socaddr.sin_family = AF_INET;
    socaddr.sin_addr.s_addr = inet_addr("47.107.25.1");
    socaddr.sin_port = htons(DEFAULT_PORT);
		
    //连接服务器
	int connect_flag;
	connect_flag = connect(socket_fd,(struct sockaddr*)&socaddr,(socklen_t)sizeof(socaddr));
    if(-1 == connect_flag)
	{
        printf("%s : connect failed of client!\n",__func__);
        return -1;//socket连接失败
    }
 
    //向服务器发送url请求的request
    int send_flag;
	send_flag = send(socket_fd,request,strlen(request),0);
    if(-1 == send_flag)
    {
        printf("%s : send failed of client!\n",__func__);
        return -1;//向服务器发送失败
    }
 
    //客户端接收服务器的返回信息
	static char response[SOHU_RESP_BUFSIZE];
    memset(response,0,SOHU_RESP_BUFSIZE);
    int nCount;
	nCount = recv(socket_fd,response,SOHU_RESP_BUFSIZE,0);
    if(-1 == nCount)
    {
        printf("%s : recieve failed!\n",__func__);
        return -1;//服务器信息接收失败
    }
    else
    {
		printf("\n%s \n",response);
		/*
		int response_flg = 0;
		response_flg = address_get_http_result(response);
		
		if(response_flg != 0)
		{
			printf("%s : http response failed !\n",__func__);
			return -1;//服务器返回失败
		}
		else
		{
			printf("\n%s \n",response);
		}
		*/
    }
	
    close(socket_fd);
	return 0;
}











