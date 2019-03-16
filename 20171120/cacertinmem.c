/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * CA cert in memory with OpenSSL to get a HTTPS page.
 * </DESC>
 */
//#include <openssl/ssl.h>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "cacertinmem.h"
#include "https_interface.h"
#include "https_result.h" 

int shift = 0;

size_t writefunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
  //printf("buffer is %s\n",(char *)ptr);
 // fwrite(ptr, size, nmemb, stream);
	int page_size = size*nmemb;
  	memcpy(stream+shift,ptr,size*nmemb);
  	shift += page_size;
  	//printf("nmemb:%d .################################################# \n",size*nmemb);
	return (nmemb*size);
}
/*  注册、匹配使用的接口
 *
 */
int https_post(const char *strPost,char *strResponse,char *strClient)
{
  shift = 0;
  char s_client[HEARD_SIZE] = {0};
  sprintf(s_client,"%s%s","client:",strClient);

  CURL *ch;
  CURLcode rv;
  //const char *data = "c=t&m=none&appid=15095155717939&f=007566E38531ACD9C6432195";
  //const char *data = "c=r&m=none&appid=15095155717939&f=007566E38531ACD9C6432195";
  
  struct curl_slist *headers=NULL; /* init to NULL is important */
  headers = curl_slist_append(headers, "Accept-Language: zh");
  //headers = curl_slist_append(headers, "client:15100472_dab41");
  headers = curl_slist_append(headers, s_client);

  rv=curl_global_init(CURL_GLOBAL_ALL);
  ch=curl_easy_init();
  rv=curl_easy_setopt(ch, CURLOPT_VERBOSE, 0L);
  rv=curl_easy_setopt(ch, CURLOPT_HEADER, 0L);
  rv=curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
  rv=curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1L);
  
  curl_easy_setopt(ch, CURLOPT_ACCEPT_ENCODING, "");   //自动解压gzip
  
  rv=curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, *writefunction);
  rv=curl_easy_setopt(ch, CURLOPT_WRITEDATA, strResponse);

  //打印请求信息
  //curl_easy_setopt(ch, CURLOPT_VERBOSE, 1); 
  //char str[POSTSTR_SIZE] = {0};
  //char *rid;
  
  //rid = curl_easy_escape(ch, "o2idu+GdgrVIiXj/oTU6rg==", strlen("o2idu+GdgrVIiXj/oTU6rg=="));
  //sprintf(str,"%s%s%s%s","c=d&m=none&r=",rid,"&appid=15095155717939&f=","007566E38531ACD9C6432195");

  //rv=curl_easy_setopt(ch, CURLOPT_WRITEDATA, stdout);
  
  //打印相应头信息
  //rv=curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, *writefunction);
  //rv=curl_easy_setopt(ch, CURLOPT_HEADERDATA, stderr);
  rv=curl_easy_setopt(ch, CURLOPT_SSLCERTTYPE, "PEM");
  rv=curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 1L);
  
  //curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, strlen(data));		
  //curl_easy_setopt(ch,CURLOPT_POSTFIELDS,data);	
  curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, strlen(strPost));	
  curl_easy_setopt(ch,CURLOPT_POSTFIELDS,strPost);
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  
  curl_easy_setopt(ch,CURLOPT_CAINFO,"/etc/ssl/certs/ca-certificates.crt"); 
  //curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/cacert.pem"); 
  rv=curl_easy_setopt(ch, CURLOPT_URL, "https://api.yaokongyun.cn/chip/m.php");

  /* first try: retrieve page without cacerts' certificate -> will fail
   */  
  
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
	  ;
    //printf("\n*** transfer succeeded ***\n");
  else
    printf("\n***errno:[%d]  transfer failed ***\n",rv);
  
  curl_slist_free_all(headers); 
  curl_easy_cleanup(ch);
  curl_global_cleanup();
  return rv;
}

/*  获取完整码库数据 ，rid需要转义
 *
 */
int https_post_all(const char *c_rid,const unsigned char *secret_key,char *strResponse,char *strClient)
{
  shift = 0;
  char s_client[HEARD_SIZE] = {0};
  sprintf(s_client,"%s%s","client:",strClient);

  CURL *ch;
  CURLcode rv;
  
  struct curl_slist *headers=NULL; /* init to NULL is important */
  headers = curl_slist_append(headers, "Accept-Language: zh");
  headers = curl_slist_append(headers, s_client);

  rv=curl_global_init(CURL_GLOBAL_ALL);
  ch=curl_easy_init();
  rv=curl_easy_setopt(ch, CURLOPT_VERBOSE, 0L);
  rv=curl_easy_setopt(ch, CURLOPT_HEADER, 0L);
  rv=curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
  rv=curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1L);
  
  curl_easy_setopt(ch, CURLOPT_ACCEPT_ENCODING, "");   //自动解压gzip
  
  rv=curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, *writefunction);
  rv=curl_easy_setopt(ch, CURLOPT_WRITEDATA, strResponse);

  //打印请求信息
  //curl_easy_setopt(ch, CURLOPT_VERBOSE, 1); 
  char strPost[POSTSTR_SIZE] = {0};
  char *rid;
  
  rid = curl_easy_escape(ch, c_rid, strlen(c_rid));
  sprintf(strPost,"%s%s%s%s","c=d&m=none&r=",rid,"&appid=15095155717939&f=",secret_key);

  //rv=curl_easy_setopt(ch, CURLOPT_WRITEDATA, stdout);
  //rv=curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, *writefunction);
  //rv=curl_easy_setopt(ch, CURLOPT_HEADERDATA, stderr);
  rv=curl_easy_setopt(ch, CURLOPT_SSLCERTTYPE, "PEM");
  rv=curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 1L);
  
  curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, strlen(strPost));	
  curl_easy_setopt(ch,CURLOPT_POSTFIELDS,strPost);
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  
  curl_easy_setopt(ch,CURLOPT_CAINFO,"/etc/ssl/certs/ca-certificates.crt"); 
  //curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/cacert.pem"); 
  rv=curl_easy_setopt(ch, CURLOPT_URL, "https://api.yaokongyun.cn/chip/m.php");

  /* first try: retrieve page without cacerts' certificate -> will fail
   */  
  //printf("str:%s,\n",str);
  
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
    printf("\n*** transfer succeeded ***\n");
  else
    printf("\n***errno:[%d]  transfer failed ***\n",rv);

  curl_free(rid);
  curl_slist_free_all(headers); 
  curl_easy_cleanup(ch);
  curl_global_cleanup();
  return rv;
}


//3、获取被遥控设备类型列表,app已知
int get_Electrical_list()
{
	unsigned char secret_key[25] = "007566E38531ACD9C6432195";

	//获取被遥控设备类型列表
	char strPost[POSTSTR_SIZE] = {0};
	sprintf(strPost,"%s%s","c=t&m=none&appid=15095155717939&f=",secret_key);

	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	char strResponse[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s","none",secret_key);
	headers_client(strMD5,strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0)
		return -1;

	printf("strResponse:%s. \n",strResponse);
	
	FILE *fd;
	fd = fopen(ElectricalPATH,"w+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return -1;
	}
	fwrite(strResponse,strlen(strResponse)+1,1,fd);
	fclose(fd);
	
	return 0;

}
//4、根据设备类型获取被遥控设备品牌列表，app已知 t:电器类型
int get_Brand_list(char *t,int number)
{
	unsigned char secret_key[25] = "007566E38531ACD9C6432195";

	//获取被遥控设备类型列表
	char strPost[POSTSTR_SIZE] = {0};
	sprintf(strPost,"%s%s%s%s","c=f&m=none&t=",t,"&appid=15095155717939&f=",secret_key);

	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	char strResponse[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s%s","none",t,secret_key);
	headers_client(strMD5,strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0)
		return -1;

	printf("strResponse:%s. \n",strResponse);

#if 0	
	//json数据解析 第一步 获取品牌bid，品牌个数sm
	int sm = 0;
	int bid = 0;
	char name[50] ={0};
	char path[60] = {0};
	char set_v[5] = "4";

	char *result_json = NULL;
	result_json = strstr(strResponse,"{");
	int re_num = asr_1code_json(result_json,&sm,&bid,name,number);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}	
	
	if(bid < 0)
		return 0;
	printf("sm:%d bid:%d name:%s \n",sm,bid,name);
	
	
	//获取（部分数据-匹配用）
	memset(strPost,0,POSTSTR_SIZE);
	sprintf(strPost,"%s%d%s%s%s%s%s%s","c=l&m=none&bid=",bid,"&t=",t,"&v=",set_v,"&appid=15095155717939&f=",secret_key);

	memset(strClient,0,HEARD_SIZE);
	memset(strMD5,0,MD5_SIZE);
	memset(strResponse,0,RESPONSE_SIZE);

	sprintf(strMD5,"%s%d%s%s%s","none",bid,t,set_v,secret_key);
	headers_client(strMD5,strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0)
		return -1;
	
	//printf("strResponse:%s. \n",strResponse);
	//return 0;
	
	sprintf(path,"%s%s%s%s",ROOTPATH,t,"/",name);
	
	if (access(path,F_OK) == 0){
		
		//printf("1111111111111 i:%d  name:%s \n",number,name);
		return -2;
	}

	
	//json数据解析 第二步
	//型号个数
	//int sm2 = 0;
	//char rmodel[50] = {0};
	//int get_v = 0;
	
	char *result_json2 = NULL;
	result_json2 = strstr(strResponse,"{");
	re_num = asr_2code_json(result_json2,bid,path);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}
	//printf("sm2:%d  \n",sm2);

	
	printf("i:%d  name:%s \n",number,name);
#endif
	return 0;
}

//5、根据品牌ID、设备类型获取遥控器列表（部分数据-匹配用），写入本地
/*
 *  bid:品牌id   t:电器类型   v:码库版本 取值1、3、4   
 */
int get_Match_code(char *bid,char *t,char *v)
{
	unsigned char secret_key[25] = "007566E38531ACD9C6432195";

	//获取（部分数据-匹配用）
	char strPost[POSTSTR_SIZE] = {0};
	sprintf(strPost,"%s%s%s%s%s%s%s%s","c=l&m=none&bid=",bid,"&t=",t,"&v=",v,"&appid=15095155717939&f=",secret_key);

	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	char strResponse[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s%s%s%s","none",bid,t,v,secret_key);
	headers_client(strMD5,strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0)
		return -1;

	//返回的数据
	printf("strResponse:%s. \n",strResponse);

	
	return 0;
}

//6、根据遥控器ID获取遥控器码库（完整数据），写入本地
int get_Complete_code(char *t)
{
	
	//读取rid, c_path:唯一的码库名字
	char rid[128] = "eEyjcW0sEwiiAPw8v1S2kA==";
	unsigned char secret_key[25] = "007566E38531ACD9C6432195";

	//获取（完整数据）
	//char strPost[POSTSTR_SIZE] = {0};
	//sprintf(strPost,"%s%s%s%s","c=d&m=none&r=",rid,"&appid=15095155717939&f=",secret_key);

	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	char strResponse[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s%s","none",rid,secret_key);
	headers_client(strMD5,strClient);


	if(https_post_all(rid,secret_key,strResponse,strClient) != 0)
		return -1;

	//返回的数据
	printf("strResponse:%s. \n",strResponse);

	#if 0
	fd = fopen(c_path,"w+");
	
	if(fd == NULL){
		printf("open %s error!!!!.\n",c_path);
		return -1;
	}
	fwrite(strResponse,strlen(strResponse),1,fd);		
	fclose(fd);

	#endif 
	return 0;
}

int send_Complete_code(char *path,char *key)
{
	char strMatch[RESPONSE_SIZE] = {0};
	char new_path[50] = {0};
	char record[50] = {0};
	strcat(new_path,"/usr/fs/usr/yuan/");
	strcat(new_path,path);
	strcat(new_path,".txt");
	
	//判断码库不存在，退出
	if (access(new_path,F_OK) != 0)
		return -1;

	FILE *fd;
	fd = fopen(new_path,"r+");
	if(fd == NULL){
		printf("%s: open Complete_code.txt error!!!!.\n",__func__);
		return -1;
	}
	fread(strMatch,RESPONSE_SIZE,1,fd);
	
	fclose(fd);
	
	//json数据解析
	int zip = 0;
	char src[512] ={0};
	char rid[128] ={0};
	char name[50] ={0};

	char *result_json = NULL;
	result_json = strstr(strMatch,"{");
	int re_num = asr_complete_json(result_json,rid,name,&zip,src,key);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}

	//printf("rid:%s ,name:%s ,zip:%d ,src:%s \n",rid,name,zip,src);
	
	/*********发送码库数据*****************/
	//1.获取激活码
	unsigned char strActivation[128] = {0};
	
	fd = fopen("/usr/fs/usr/yuan/activation_code.txt","r+");
	if(fd == NULL){
		printf("open activation_code.txt error!!!!.\n");
		return -1;
	}
	fread(strActivation,128,1,fd);	
	fclose(fd);

	//bngo发送红外测试码
	char data[512] = {0};
	sprintf(data,"%s%d%s","0",zip,src);
	//printf("strActivation:%s data:%s .\n",strActivation,data);
	//if(IR_send_data(strActivation,(unsigned char *)data) != 0)
		//return -1;


	/*************记录历史命令*******************/
	if(strcmp(key,"off") != 0){
		strcat(record,"/usr/fs/usr/yuan/");
		strcat(record,path);
		strcat(record,".record");

		fd = fopen(record,"w+");
		if(fd == NULL){
			printf("%s: open record error!!!!.\n",__func__);
			return -1;
		}
		fwrite(key,strlen(key)+1,1,fd);
		
		fclose(fd);
	}
	return 0;
}


int get_all_key(char *t)
{
	unsigned char secret_key[25] = "007566E38531ACD9C6432195";
	FILE *fd;
	
	//判断品牌是否已经下载
	char t_path[128] = {0};
	sprintf(t_path,"%s%s%s%s",ROOTPATH,t,"/","品牌列表");
	if(access(t_path,F_OK) == 0)
		goto step2;

	//获取被遥控设备类型列表
	char strPost[POSTSTR_SIZE] = {0};
	sprintf(strPost,"%s%s%s%s","c=f&m=none&t=",t,"&appid=15095155717939&f=",secret_key);

	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	char strResponse[RESPONSE_SIZE] = {0};
	char strResponse1[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s%s","none",t,secret_key);
	headers_client(strMD5,strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0)
		return -1;

	//获取到品牌列表
	//printf("strResponse:%s. \n",strResponse);
	
	fd = fopen(t_path,"w+");
	if(fd == NULL){
		printf("%s: 1111111111 open record error!!!!.\n",__func__);
		return -1;
	}
	fwrite(strResponse,strlen(strResponse),1,fd);
	fclose(fd);

	//获取每个品牌第一个型号的键值
	

#if 1	
		//json数据解析 第一步 获取品牌bid，品牌个数sm
		int sm = 0;
		int bid = 0;
		char name[50] ={0};
		char path[60] = {0};
		char set_v[5] = "4";
		int number = 0;
		char *result_json = NULL;
step2:		
		memset(strResponse,0,RESPONSE_SIZE);
		//读品牌列表
		fd = fopen(t_path,"r+");
		if(fd == NULL){
			printf("%s: 22222222222 open record error!!!!.\n",__func__);
			return -1;
		}
		fread(strResponse,RESPONSE_SIZE,1,fd);
		fclose(fd);
		
		//printf("strResponse:%s. \n",strResponse);
		
		result_json = strstr(strResponse,"{");
		int re_num = asr_1code_json(result_json,&sm,&bid,name,0);
		if(re_num == -1)
		{
			printf("%s : parse json error.\n",__func__);
			return -1;//解析json失败
		}	
		
		if(bid < 0){
			printf("第一个品牌  number>>>>0 name:%s  bid:%d ###########\n",name,bid);
			return 0;
		}

		//printf("sm:%d bid:%d name:%s \n",sm,bid,name);
		
		//获取到品牌数量
		number = sm;
		//char rid[128] = {0};
		
		char *result_json2 = NULL;

		int i = 0;
		for(i= 0;i<number;i++){
step3:	

			//memset(strResponse,0,RESPONSE_SIZE);
			memset(name,0,50);
			sm = 0;
			bid = 0;
			//读品牌列表
			//fd = fopen(t_path,"r+");
			//if(fd == NULL){
				//printf("%s: 3333333333 open record error!!!!.\n",__func__);
				//return -1;
			//}
			//fread(strResponse,RESPONSE_SIZE,1,fd);
			//fclose(fd);

			
			result_json = strstr(strResponse,"{");
			int re_num = asr_1code_json(result_json,&sm,&bid,name,i);
			if(re_num == -1)
			{
				printf("%s : parse json error.\n",__func__);
				return -1;//解析json失败
			}	
			
			if(strcmp(t,"2") == 0 && strstr(name,"东宝") != NULL && strstr(name,"东凌") != NULL){
				strcpy(name,"东宝东凌");
			}
			
			printf("###########  number>>>>%d name:%s  bid:%d ###########\n",i,name,bid);
			
			if(bid < 0){
				//return 0;
				printf("bid小于0  number>>>>%d name:%s  bid:%d ###########\n",i,name,bid);
				i++;
				goto step3;
			}

			memset(path,0,60);
			
			sprintf(path,"%s%s%s%s",ROOTPATH,t,"/",name);

			if (access(path,F_OK) == 0){	
				printf("文件重复  number>>>>%d name:%s  bid:%d ###########\n",i,name,bid);
				i++;
				goto step3;
			}
			
		
			//获取（部分数据-匹配用）
			memset(strPost,0,POSTSTR_SIZE);
			sprintf(strPost,"%s%d%s%s%s%s%s%s","c=l&m=none&bid=",bid,"&t=",t,"&v=",set_v,"&appid=15095155717939&f=",secret_key);
		
			memset(strClient,0,HEARD_SIZE);
			memset(strMD5,0,MD5_SIZE);
			memset(strResponse1,0,RESPONSE_SIZE);
		
			sprintf(strMD5,"%s%d%s%s%s","none",bid,t,set_v,secret_key);
			headers_client(strMD5,strClient);
			
			if(https_post(strPost,strResponse1,strClient) != 0)
				return -1;
			
			//printf("strResponse:%s. \n",strResponse);
			//return 0;
		
			
			//json数据解析 第二步
			//获取品牌下的第一个 rid 完整码库
			result_json2 = strstr(strResponse1,"{");
			re_num = asr_mcode_msg(result_json2,bid,path);
			if(re_num == -1)
			{
				printf("%s : parse json error.\n",__func__);
				return -1;//解析json失败
			}
			//printf("rid:%s \n",rid);

		}
#endif	

	return 0;
}

//获取遥控器键值
#if 0
int get_all_key(char *t)
{
	unsigned char secret_key[25] = "007566E38531ACD9C6432195";
	FILE *fd;
	
	//判断品牌是否已经下载
	char t_path[128] = {0};
	sprintf(t_path,"%s%s%s%s",KEYPATH,t,"/","品牌列表");
	if(access(t_path,F_OK) == 0)
		goto step2;

	//获取被遥控设备类型列表
	char strPost[POSTSTR_SIZE] = {0};
	sprintf(strPost,"%s%s%s%s","c=f&m=none&t=",t,"&appid=15095155717939&f=",secret_key);

	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	char strResponse[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s%s","none",t,secret_key);
	headers_client(strMD5,strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0)
		return -1;

	//获取到品牌列表
	//printf("strResponse:%s. \n",strResponse);

	fd = fopen(t_path,"w+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		return -1;
	}
	fwrite(strResponse,strlen(strResponse),1,fd);
	fclose(fd);

	//获取每个品牌第一个型号的键值
	

#if 1	
		//json数据解析 第一步 获取品牌bid，品牌个数sm
		int sm = 0;
		int bid = 0;
		char name[50] ={0};
		char path[60] = {0};
		char set_v[5] = "4";
		int number = 0;
		char *result_json = NULL;
step2:		
		memset(strResponse,0,RESPONSE_SIZE);
		//读品牌列表
		fd = fopen(t_path,"r+");
		if(fd == NULL){
			printf("%s: 111111111 open record error!!!!.\n",__func__);
			return -1;
		}
		fread(strResponse,RESPONSE_SIZE,1,fd);
		fclose(fd);
		
		//printf("strResponse:%s. \n",strResponse);
		
		result_json = strstr(strResponse,"{");
		int re_num = asr_1code_json(result_json,&sm,&bid,name,0);
		if(re_num == -1)
		{
			printf("%s : parse json error.\n",__func__);
			return -1;//解析json失败
		}	
		
		if(bid < 0){
			printf("第一个品牌  number>>>>0 name:%s  bid:%d ###########\n",name,bid);
			return 0;
		}

		//printf("sm:%d bid:%d name:%s \n",sm,bid,name);
		
		//获取到品牌数量
		number = sm;
		char rid[128] = {0};
		
		char *result_json2 = NULL;

		int i = 0;
		for(i= 1;i<number;i++){
step3:	

			memset(strResponse,0,RESPONSE_SIZE);
			memset(name,0,50);
			sm = 0;
			bid = 0;
			//读品牌列表
			fd = fopen(t_path,"r+");
			if(fd == NULL){
				printf("%s: 33333333 open record error!!!!.\n",__func__);
				return -1;
			}
			fread(strResponse,RESPONSE_SIZE,1,fd);
			fclose(fd);

			
			result_json = strstr(strResponse,"{");
			int re_num = asr_1code_json(result_json,&sm,&bid,name,i);
			if(re_num == -1)
			{
				printf("%s : parse json error.\n",__func__);
				return -1;//解析json失败
			}	
			printf("###########  number>>>>%d name:%s  bid:%d ###########\n",i,name,bid);
			
			if(bid < 0){
				//return 0;
				printf("bid小于0  number>>>>%d name:%s  bid:%d ###########\n",i,name,bid);
				i++;
				goto step3;
			}

			memset(path,0,60);
			sprintf(path,"%s%s%s%s",KEYPATH,t,"/",name);
			
			if (access(path,F_OK) == 0){	
				printf("文件重复  number>>>>%d name:%s  bid:%d ###########\n",i,name,bid);
				i++;
				goto step3;
			}

		
			//获取（部分数据-匹配用）
			memset(strPost,0,POSTSTR_SIZE);
			sprintf(strPost,"%s%d%s%s%s%s%s%s","c=l&m=none&bid=",bid,"&t=",t,"&v=",set_v,"&appid=15095155717939&f=",secret_key);
		
			memset(strClient,0,HEARD_SIZE);
			memset(strMD5,0,MD5_SIZE);
			memset(strResponse,0,RESPONSE_SIZE);
		
			sprintf(strMD5,"%s%d%s%s%s","none",bid,t,set_v,secret_key);
			headers_client(strMD5,strClient);
			
			if(https_post(strPost,strResponse,strClient) != 0)
				return -1;
			
			//printf("strResponse:%s. \n",strResponse);
			//return 0;
		
			
			//json数据解析 第二步
			//获取品牌下的第一个 rid 完整码库
			result_json2 = strstr(strResponse,"{");
			re_num = asr_3code_json(result_json2,rid);
			if(re_num == -1)
			{
				printf("%s : parse json error.\n",__func__);
				return -1;//解析json失败
			}
			//printf("rid:%s \n",rid);


			//根据rid获取完整码库
			memset(strClient,0,HEARD_SIZE);
			memset(strMD5,0,MD5_SIZE);
			memset(strResponse,0,RESPONSE_SIZE);
			sprintf(strMD5,"%s%s%s","none",rid,secret_key);
			headers_client(strMD5,strClient);
			if(https_post_all(rid,secret_key,strResponse,strClient) != 0)
				return -1;

			//获取键值
			result_json2 = strstr(strResponse,"{");
			re_num = asr_4code_json(result_json2,path);
			if(re_num == -1)
			{
				printf("%s : parse json error.\n",__func__);
				return -1;//解析json失败
			}

		}
#endif	

	return 0;
}
#endif


