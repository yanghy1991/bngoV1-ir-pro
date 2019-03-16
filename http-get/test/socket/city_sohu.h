/***************************************************************************
 * 
 * 从搜狐获取当前城市名的汉字并且转成GB18030码进行存储
 * 
 **************************************************************************/
#ifndef  __CITY_SOHU_H_
#define  __CITY_SOHU_H_
#endif

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include<dirent.h>

#include <locale.h>
#include <stdbool.h>

//http访问默认端口
#define DEFAULT_PORT 80

//从pv.sohu.com/cityjson?ie=utf-8获取当前城市名
#define SOHU_LINE_BUFSIZE 1024
#define SOHU_COM_BUFSIZE 1024
#define SOHU_RESP_BUFSIZE 1024
#define SOHU_SMALL_BUFSIZE 16
/**
*功能:发起http请求，获取响应数据
*参数:
*url:http请求地址
*city:用来存储city
*
*/
int sohu_get_city(char *city);
