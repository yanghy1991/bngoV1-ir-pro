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

#include "yaokan_ir.h"
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
  
  //curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/ca-certificates.crt"); 
  curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/cacert.pem"); 
  rv=curl_easy_setopt(ch, CURLOPT_URL, "https://api.yaokongyun.cn/chip/m.php");

  /* first try: retrieve page without cacerts' certificate -> will fail
   */  
  
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
    printf("\n*** transfer succeeded ***\n");
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
  
  //curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/ca-certificates.crt"); 
  curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/cacert.pem"); 
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



//1、将f:设备id写入本地
int get_secret_key()
{
	if (access("/usr/fs/usr/yuan/secret_key.txt",F_OK) == 0)
		return 0;
	
	unsigned char secret_key[25] = {0};

	int i;
	i =  Registration_equipment(secret_key);
	if(i != 0){
		printf("equipment error!\n");
		return i;
	}
	//printf("%s\n",secret_key);

	FILE *fd;

	fd = fopen("/usr/fs/usr/yuan/secret_key.txt","w+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fwrite(secret_key,strlen((char *)secret_key)+1,1,fd);
	
	fclose(fd);
	
	return 0;
}

//2、注册设备id，获取激活码，写入本地
int get_Activation_code()
{
	unsigned char secret_key[25] = {0};

	//判断激活码是否存在
	if (access("/usr/fs/usr/yuan/activation_code.txt",F_OK) == 0)
		return 0;	

	FILE *fd;
	fd = fopen("/usr/fs/usr/yuan/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);
	//printf("%s\n",secret_key);
		
	fclose(fd);	


	//获取激活码
	char strPost[POSTSTR_SIZE] = {0};
	sprintf(strPost,"%s%s","c=r&m=none&appid=15095155717939&f=",secret_key);

	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	char strResponse[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s","none",secret_key);
	headers_client(strMD5,strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0)
		return -1;

	//json数据解析
	int ret_code = 0;
	char ret_msg[128] ={0};
	//printf("strResponse:%s. \n",strResponse);

	char *result_json = NULL;
	result_json = strstr(strResponse,"{");
	int re_num = asr_acode_json(result_json,ret_msg,&ret_code);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}

	if(ret_code == 1){
		fd = fopen("/usr/fs/usr/yuan/activation_code.txt","w+");
		if(fd == NULL){
			printf("open activation_code.txt error!!!!.\n");
			return -1;
		}
		fwrite(ret_msg,strlen(ret_msg)+1,1,fd);
		//fwrite("K4rfzhTJS3rnJB8rvyWPDOz8VVj47le3FOXXILEB4KE=",strlen("K4rfzhTJS3rnJB8rvyWPDOz8VVj47le3FOXXILEB4KE=")+1,1,fd);
		
		fclose(fd);			
	} else{
		printf("ret_code:%d .ret_msg:%s.\n",ret_code,ret_msg);
		return ret_code;
	}
	//printf("ret_code:%d .ret_msg:%s.\n",ret_code,ret_msg);
	
	return 0;
}

//3、获取被遥控设备类型列表,app已知
int get_Electrical_list()
{
	unsigned char secret_key[25] = {0};
	
	FILE *fd;
	fd = fopen("/usr/fs/usr/yuan/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);
	//printf("%s\n",secret_key);		
	fclose(fd);

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
	return 0;

}
//4、根据设备类型获取被遥控设备品牌列表，app已知 t:电器类型
int get_Brand_list(char *t)
{
	unsigned char secret_key[25] = {0};
	
	FILE *fd;
	fd = fopen("/usr/fs/usr/yuan/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);
	//printf("%s\n",secret_key);		
	fclose(fd);

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

	return 0;
}

//5、根据品牌ID、设备类型获取遥控器列表（部分数据-匹配用），写入本地
/*
 *  bid:品牌id   t:电器类型   v:码库版本 取值1、3、4   
 */
int get_Match_code(char *bid,char *t,char *v)
{
	unsigned char secret_key[25] = {0};
	
	FILE *fd;
	fd = fopen("/usr/fs/usr/yuan/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);
	//printf("%s\n",secret_key);		
	fclose(fd);

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
	//printf("strResponse:%s. \n",strResponse);
	
	fd = fopen("/usr/fs/usr/yuan/match_code.txt","w+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return -1;
	}
	fwrite(strResponse,strlen(strResponse)+1,1,fd);		
	fclose(fd);
	
	return 0;
}

//发送匹配数据
int send_Match_code(int t,int number,char *key)
{
	char strMatch[RESPONSE_SIZE] = {0};
	//判断匹配码库是否存在
	if (access("/usr/fs/usr/yuan/match_code.txt",F_OK) != 0)
		return -1;
	
	FILE *fd;
	fd = fopen("/usr/fs/usr/yuan/match_code.txt","r+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return -1;
	}
	fread(strMatch,RESPONSE_SIZE,1,fd);
	
	fclose(fd);

	//printf("strMatch:%s\n",strMatch);	

	if(t == 7){
	//json数据解析
	int sm = 0;
	int zip = 0;
	char src[512] ={0};
	char rid[128] ={0};
	char name[50] ={0};
	char path[60] = {0};

	char *result_json = NULL;
	result_json = strstr(strMatch,"{");
	int re_num = asr_mcode_json(result_json,&sm,rid,name,&zip,src,number,key);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}

	//printf("sm:%d ,rid:%s ,zip:%d ,src:%s \n",sm,rid,zip,src);
	sprintf(path,"%s%d",name,number);
	/*************将rid:遥控id写入本地，方便获取完整码库*******************/
	fd = fopen("/usr/fs/usr/yuan/rid.txt","w+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return -1;
	}
	fwrite(rid,128,1,fd);	
	fwrite(path,60,1,fd);
	fclose(fd);

	/*********发送匹配数据*****************/
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
	if(IR_send_data(strActivation,(unsigned char *)data) != 0)
		return -1;
		}
	return 0;	
}

//6、根据遥控器ID获取遥控器码库（完整数据），写入本地
int get_Complete_code(char *t)
{
	//判断rid:遥控id是否存在
	if (access("/usr/fs/usr/yuan/rid.txt",F_OK) != 0)
		return -1;
	
	//读取rid, c_path:唯一的码库名字
	char rid[128] ={0};
	char h_path[60] ={0};
	FILE *fd;
	fd = fopen("/usr/fs/usr/yuan/rid.txt","r+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return -1;
	}
	fread(rid,128,1,fd);
	fread(h_path,60,1,fd);
	fclose(fd);
	
	//printf("rid:%s ,path:%s \n",rid,c_path);
	

	char c_path[60] ={0};
	sprintf(c_path,"%s%s%s","/usr/fs/usr/yuan/",h_path,".txt");
	if (access(c_path,F_OK) == 0)
		return 0;

	/****************https 获取码库*********************************/
	unsigned char secret_key[25] = {0};
	
	fd = fopen("/usr/fs/usr/yuan/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);
	//printf("%s\n",secret_key);		
	fclose(fd);

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
	//printf("strResponse:%s. \n",strResponse);


	fd = fopen(c_path,"w+");
	
	if(fd == NULL){
		printf("open %s error!!!!.\n",c_path);
		return -1;
	}
	fwrite(strResponse,strlen(strResponse)+1,1,fd);		
	fclose(fd);


	/**************语音控制配置文件*****************/
	char asr_path[60] ={0};
	sprintf(asr_path,"%s%s%s","/usr/fs/usr/yuan/",t,".txt");

	if (access(asr_path,F_OK) != 0){
		fd = fopen(asr_path,"w+");
		if(fd == NULL){
			printf("open asr path.txt error!!!!.\n");
			return -1;
		
		}
		fwrite(h_path,strlen(h_path)+1,1,fd);
		fclose(fd);		
	}

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
	if(IR_send_data(strActivation,(unsigned char *)data) != 0)
		return -1;


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
