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
#include <error.h>

#include "yaokan_ir.h"
#include "cacertinmem.h"
#include "https_interface.h"
#include "https_result.h"
#include "bngo_ir_learn.h"
#include "URLEncode.h"


//记录遥控服务器返回数据的大小，用于拼接
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
  
  //curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/data/ca-certificates.crt"); 
  curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/data/cacert.pem"); 
  rv=curl_easy_setopt(ch, CURLOPT_URL, "https://api.yaokongyun.cn/chip/m.php");

  /* first try: retrieve page without cacerts' certificate -> will fail
   */  

  //30秒超时  
  rv=curl_easy_setopt(ch, CURLOPT_TIMEOUT, 30L);
  
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
    ;//printf("\n*** transfer succeeded ***\n");
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
  
  //curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/data/ca-certificates.crt"); 
  curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/data/cacert.pem"); 
  rv=curl_easy_setopt(ch, CURLOPT_URL, "https://api.yaokongyun.cn/chip/m.php");

  //30秒超时  
  rv=curl_easy_setopt(ch, CURLOPT_TIMEOUT, 30L);

  /* first try: retrieve page without cacerts' certificate -> will fail
   */  
  //printf("str:%s,\n",str);
  
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
    ;//printf("\n*** transfer succeeded ***\n");
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
	FILE *fd;
		
	if (access("/usr/data/secret_key.txt",F_OK) == 0){
		//判断设备id是否无效
		unsigned char check_secret_key[25] = {0};
		fd = fopen("/usr/data/secret_key.txt","r+");
		if(fd == NULL){
			printf("open secret_key.txt error!!!!.\n");
			return -1;
		}
		fread(check_secret_key,25,1,fd);	
		fclose(fd);	
		
		if(strlen(check_secret_key) < 10){
			remove("/usr/data/secret_key.txt");
			return SECRET_KEY_ERR;
		}
		return 0;
	}

	unsigned char secret_key[25] = {0};

	int i;
	i =  Registration_equipment(secret_key);
	if(i != 0){
		printf("equipment error!\n");
		return i;
	}
	//printf("%s\n",secret_key);

	fd = fopen("/usr/data/secret_key.txt","w+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fwrite(secret_key,strlen((char *)secret_key),1,fd);
	fflush(fd); 
	fclose(fd);
	
	return 0;
}

//2、注册设备id，获取激活码，写入本地
int get_Activation_code()
{
	unsigned char secret_key[25] = {0};
	FILE *fd;

	//判断激活码是否存在
	if (access("/usr/data/activation_code.txt",F_OK) == 0){
		//判断激活码是否为空

		//1.获取激活码
		unsigned char strActivation[128] = {0};
		
		fd = fopen("/usr/data/activation_code.txt","r+");
		if(fd == NULL){
			printf("open activation_code.txt error!!!!.\n");
			return OPEN_FILE_ERR;
		}
		fread(strActivation,128,1,fd);	
		fclose(fd);
		
		if(strlen(strActivation) < 10){
			remove("/usr/data/activation_code.txt");
			printf("[%s] get strActivation error\n",__func__);
			return ACTIVATION_CODE_ERR;
		}
		
		return 0;	
	}

	fd = fopen("/usr/data/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);
	//printf("%s\n",secret_key);
		
	fclose(fd);	


	//注册设备
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

	if(ret_code == 1 || ret_code == 10011){
		//获取激活码
		memset(strPost,0,POSTSTR_SIZE);
		sprintf(strPost,"%s%s","c=ac&m=none&appid=15095155717939&f=",secret_key);

		memset(strClient,0,HEARD_SIZE);
		memset(strMD5,0,MD5_SIZE);
		memset(strResponse,0,RESPONSE_SIZE);

		ret_code = 0;
		memset(ret_msg,0,128);

		sprintf(strMD5,"%s%s","none",secret_key);
		headers_client(strMD5,strClient);
	
		if(https_post(strPost,strResponse,strClient) != 0)
			return -1;

		//printf("strResponse:%s. \n",strResponse);

		char *result_json2 = NULL;
		result_json2 = strstr(strResponse,"{");
		int re_num2 = asr_acode_json(result_json2,ret_msg,&ret_code);
		if(re_num2 == -1)
		{
			printf("%s : parse json error.\n",__func__);
			return -1;//解析json失败
		}

		if(ret_code != 1)
			return ret_code;
		
		//写入激活码
		fd = fopen("/usr/data/activation_code.txt","w+");
		if(fd == NULL){
			printf("open activation_code.txt error!!!!.\n");
			return -1;
		}
		fwrite(ret_msg,strlen(ret_msg),1,fd);
		fflush(fd); 
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
	fd = fopen("/usr/data/secret_key.txt","r+");
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

	//printf("strResponse:%s. \n",strResponse);
	return 0;

}
//4、根据设备类型获取被遥控设备品牌列表，app已知 t:电器类型
int get_Brand_list(char *t)
{
	unsigned char secret_key[25] = {0};
	
	FILE *fd;
	fd = fopen("/usr/data/secret_key.txt","r+");
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

	//printf("strResponse:%s. \n",strResponse);
	
	
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
	fd = fopen("/usr/data/secret_key.txt","r+");
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

	//写入该电器类型和bid 避免重复下载
	char info[50] = {0};
	sprintf(info,"###%s%s%s###",bid,t,v);

	//返回的数据
	//printf("strResponse:%s. \n",strResponse);
	
	fd = fopen("/usr/data/match_code.txt","w+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return -1;
	}
	fwrite(info,strlen(info),1,fd);		
	fwrite(strResponse,strlen(strResponse)+1,1,fd);	
	fflush(fd);
	fclose(fd);
	
	return 0;
}

//7、根据遥控器ID、电器类、按键，进入一键匹配模式
/*
 *  bid:品牌id   t:电器类型   v:码库版本 取值1、3、4   
 */
int ont_key_match(char *bid,char *t,char *src)
{
	int ret = 0;
	//进入学习模式，同时将学习到的数据保存到本地 红灯亮
	ret = IR_LEARN_SET(1);
	if(ret != 0)
		return ret;

	usleep(100);

	//获取转化后的电平数据
	char match_learn_src[1024] = {0};
	char match_learn_src_encode[2048] = {0};
	
	ret = MATCH_IR_LEARN(match_learn_src);
	if(ret != 0){
		return ret;
	}

	//printf("\n\n size:%d match_learn_src:%s\n",strlen(match_learn_src),match_learn_src);

	//将match_learn_src src  进行urlencode 编码
	URLEncode(match_learn_src, strlen(match_learn_src), match_learn_src_encode, 2048);
	char src_encode[50] = {0};
	URLEncode(src, strlen(src), src_encode, 50);

	//printf("\n\n src_encode:%s size:%d match_learn_src_encode:%s\n",src_encode,strlen(match_learn_src_encode),match_learn_src_encode);


#if 1
	unsigned char secret_key[25] = {0};
	
	FILE *fd;
	fd = fopen("/usr/data/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");		
		return OPEN_FILE_ERR;
	}
	fread(secret_key,25,1,fd);
	//printf("%s\n",secret_key);		
	fclose(fd);

	//获取（部分数据-匹配用）
	char strPost[4096] = {0};
	sprintf(strPost,"%s%s%s%s%s%s%s%s%s%s","c=o&m=none&appid=15095155717939&f=",secret_key,"&r=",match_learn_src_encode,"&t=",t,"&bid=",bid,"&cmp_key=",src_encode);

	//printf("\n\n size:%d strPost : %s \n",strlen(strPost),strPost);
	
	char strClient[HEARD_SIZE] = {0};
	char strMD5[1024] = {0};
	char strResponse[RESPONSE_SIZE] = {0};

	sprintf(strMD5,"%s%s%s%s%s%s","none",secret_key,match_learn_src,t,bid,src);

	//printf("\n\n size:%d strMD5 : %s \n",strlen(strMD5),strMD5);

	
	headers_client(strMD5,strClient);
	
	//printf("strClient:%s \n",strClient);
	
	if(https_post(strPost,strResponse,strClient) != 0){
		printf("[%s] get match data error \n");
		return MATCH_GCODE_ERR;
	}

	//printf("strResponse:%s \n",strResponse);

	
	char info[50] = {0};
	//sprintf(info,"###%s%s%s###",bid,t,src);


	//返回的数据
	//printf("strResponse:%s. \n",strResponse);

	
	//保存匹配数据
	fd = fopen("/usr/data/match_code.txt","w+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fwrite(info,strlen(info),1,fd);	
	fwrite(strResponse,strlen(strResponse)+1,1,fd);		
	fclose(fd);

	return 0;
#endif 

}


//发送匹配数据
int send_Match_code(int t,int number,char *key,result_to_mozart_t *sds_results_msg)
{
	char strMatch[RESPONSE_SIZE] = {0};
	//判断匹配码库是否存在
	if (access("/usr/data/match_code.txt",F_OK) != 0){
		printf("[%s] match_code.txt not exit \n",__func__);
		return -1;
	}

	
	FILE *fd;
	fd = fopen("/usr/data/match_code.txt","r+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return -1;
	}
	fread(strMatch,RESPONSE_SIZE,1,fd);
	
	fclose(fd);

	//printf("strMatch:%s\n",strMatch);	

	//json数据解析
	int sm = 0;
	int zip = 0;
	int v = 0;
	char src[512] ={0};
	char rid[128] ={0};
	char name[50] ={0};
	char path[60] = {0};
	char str_v[10] = {0};

	char *result_json = NULL;
	result_json = strstr(strMatch,"{");
	int re_num = asr_mcode_json(result_json,&sm,rid,&v,name,&zip,src,number,key,sds_results_msg);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}

	//printf("#################sm:%d ,rid:%s ,zip:%d ,src:%s \n",sm,rid,zip,src);
	//sprintf(path,"%s%d",name,number);
	sprintf(path,"%d",number);
	sprintf(str_v,"%d",v);

	#if 0
	/*************将rid:遥控id写入本地，方便获取完整码库*******************/
	fd = fopen("/usr/data/rid.txt","w+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return -1;
	}
	printf("############################# rid:%s ##########################################",rid);
	
	fwrite(rid,128,1,fd);
	fwrite(str_v,10,1,fd);
	fwrite(name,strlen(name),1,fd);
	fwrite(path,60,1,fd);
	fclose(fd);
	#endif 

	/*********发送匹配数据*****************/
	//1.获取激活码
	unsigned char strActivation[128] = {0};
	
	fd = fopen("/usr/data/activation_code.txt","r+");
	if(fd == NULL){
		printf("open activation_code.txt error!!!!.\n");
		return -1;
	}
	fread(strActivation,128,1,fd);	
	fclose(fd);

	//判断激活码是否为空
	if(strlen(strActivation) < 10){
		remove("/usr/data/activation_code.txt");
		printf("[%s] get strActivation error\n");
		return ACTIVATION_CODE_ERR;
	}

	//bngo发送红外测试码
	char data[512] = {0};
	sprintf(data,"%s%d%s","0",zip,src);
	
	//printf("strActivation:%s data:%s .\n",strActivation,data);
	if(IR_send_data(strActivation,(unsigned char *)data) != 0){
		printf("[%s] send ir data error \n",__func__);
		return -1;
	}


	return 0;	
}

//6、根据遥控器ID获取遥控器码库（完整数据），写入本地
int get_Complete_code(char *t,result_to_mozart_t *sds_results_msg)
{
	//获取匹配编号
	char amatch_num[10] = {0};
	t = strtok( NULL,"=");
	if(t == NULL)
		return(INVALID_COMMAND);
	strcpy(amatch_num,t);
	if(amatch_num[0] < '0' ||  amatch_num[0] > '9')
		return(INVALID_COMMAND);
	
	int number = atoi(amatch_num);

	//printf("######## amatch_num:%s number:%d  ###############",amatch_num,number);


	char strMatch[RESPONSE_SIZE] = {0};
	//判断匹配码库是否存在
	if (access("/usr/data/match_code.txt",F_OK) != 0){
		printf("[%s] match_code.txt not exit \n",__func__);
		return -1;
	}

	
	FILE *fd;
	fd = fopen("/usr/data/match_code.txt","r+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return -1;
	}
	fread(strMatch,RESPONSE_SIZE,1,fd);
	
	fclose(fd);

	//printf("strMatch:%s\n",strMatch);	

	//json数据解析
	int sm = 0;
	int zip = 0;
	int v = 0;
	char src[512] ={0};
	char rid[128] ={0};
	char name[50] ={0};
	char path[60] = {0};
	char str_v[10] = {0};

	char *result_json = NULL;
	result_json = strstr(strMatch,"{");
	int re_num = asr_mcode_json(result_json,&sm,rid,&v,name,&zip,src,number,"on",sds_results_msg);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}

	sprintf(path,"%d",number);
	sprintf(str_v,"%d",v);
	/*************将rid:遥控id写入本地，方便获取完整码库*******************/
	fd = fopen("/usr/data/rid.txt","w+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return -1;
	}
	
	//printf("######## rid:%s str_v:%s name:%s path:%s ###############",rid,str_v,name,path);
	
	fwrite(rid,128,1,fd);
	fwrite(str_v,10,1,fd);
	fwrite(name,strlen(name),1,fd);
	fwrite(path,60,1,fd);
	fflush(fd);
	fclose(fd);

	/****************https 获取码库*********************************/
	unsigned char secret_key[25] = {0};
	
	fd = fopen("/usr/data/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);		
	fclose(fd);

	//headers_client: MD5加密头部
	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	//char strResponse[RESPONSE_SIZE] = {0};
	memset(strMatch,0,sizeof(strMatch));

	sprintf(strMD5,"%s%s%s","none",rid,secret_key);
	headers_client(strMD5,strClient);

	//post:请求完整码库数据
	if(https_post_all(rid,secret_key,strMatch,strClient) != 0){
		printf("[%s] get yaokan data error \n",__func__);
		return -1;
	}


	//返回的数据
	//printf("strResponse:%s. \n",strResponse);

	
	//判断码库是否是含有特殊按键
	code_info_t code_info_msg;
	memset(&code_info_msg,0,sizeof(code_info_t));
	int key = 0;
	result_json = strstr(strMatch,"{");
	re_num = check_key_json(result_json,&key,sds_results_msg);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}
	code_info_msg.key = key;

	sds_results_msg->key = key;
	
	//获取新码库时 将码库按键类型写入
	fd = fopen("/usr/data/bngoIR.record","w+");
	if(fd == NULL){
		printf("open bngoIR.txt error!!!!.\n");
		return -1;
	}
	fwrite(&code_info_msg,sizeof(code_info_msg),1,fd);
	fflush(fd);
	fclose(fd);


	//fd = fopen(c_path,"w+");
	//保存为测试文件  bngoIR
	fd = fopen("/usr/data/bngoIR.txt","w+");
	if(fd == NULL){
		printf("open bngoIR.txt error!!!!.\n");
		return -1;
	}
	fwrite(strMatch,strlen(strMatch)+1,1,fd);		
	fclose(fd);


	char *result_to_app_json = NULL;
	result_to_app_json = strstr(strMatch,"{");
	int result_to_app_ret = match_to_app_json_other(result_to_app_json,sds_results_msg->ASR_COMMAND);
	if(result_to_app_ret != 0)
	{
		printf("%s : parse json error.\n",__func__);
		return result_to_app_ret;//解析json失败
	}

	return 0;
}


//6、根据遥控器ID获取遥控器码库（完整数据），写入本地
int get_amatch_Complete_code(char *t,result_to_mozart_t *sds_results_msg)
{
	//获取电器类型
	char amatch_t[10] = {0};
	t = strtok( NULL,"=");
	if(t == NULL)
		return(INVALID_COMMAND);
	strcpy(amatch_t,t);
	//获取匹配编号
	char amatch_num[10] = {0};
	t = strtok( NULL,"=");
	if(t == NULL)
		return(INVALID_COMMAND);
	strcpy(amatch_num,t);
	int number = atoi(amatch_num);
	//获取键值
	char amatch_key[10] = {0};
	t = strtok( NULL,"=");
	if(t == NULL)
		return(INVALID_COMMAND);
	strcpy(amatch_key,t);

	
	char strMatch[RESPONSE_SIZE] = {0};
	//判断匹配码库是否存在
	if (access("/usr/data/match_code.txt",F_OK) != 0){
		printf("[%s] match_code.txt not exit \n",__func__);
		return CODE_NOT_EXIST;
	}

	
	FILE *fd;
	fd = fopen("/usr/data/match_code.txt","r+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return -1;
	}
	fread(strMatch,RESPONSE_SIZE,1,fd);
	
	fclose(fd);

	//printf("strMatch:%s\n",strMatch); 

	//json数据解析
	int sm = 0;
	int zip = 0;
	int v = 0;
	char src[512] ={0};
	char rid[128] ={0};
	char name[50] ={0};
	char path[60] = {0};
	char str_v[10] = {0};

	char *result_json = NULL;
	result_json = strstr(strMatch,"{");
	int re_num = asr_mcode_json(result_json,&sm,rid,&v,name,&zip,src,number,amatch_key,sds_results_msg);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}

	//printf("#################sm:%d ,rid:%s ,zip:%d ,src:%s \n",sm,rid,zip,src);
	//sprintf(path,"%s%d",name,number);
	sprintf(path,"%d",number);
	sprintf(str_v,"%d",v);
	/*************将rid:遥控id写入本地，方便获取完整码库*******************/
	fd = fopen("/usr/data/rid.txt","w+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		return -1;
	}
	//printf("############ rid:%s #################",rid);

	
	fwrite(rid,128,1,fd);
	fwrite(str_v,10,1,fd);
	fwrite(name,strlen(name),1,fd);
	fwrite("AKMSEND",strlen("AKMSEND"),1,fd);
	fwrite(path,60,1,fd);
	fclose(fd);
	
	//////////////////////////////////////////////////////////

	/****************https 获取码库*********************************/
	unsigned char secret_key[25] = {0};
	
	fd = fopen("/usr/data/secret_key.txt","r+");
	if(fd == NULL){
		printf("open secret_key.txt error!!!!.\n");
		return -1;
	}
	fread(secret_key,25,1,fd);		
	fclose(fd);

	//headers_client: MD5加密头部
	char strClient[HEARD_SIZE] = {0};
	char strMD5[MD5_SIZE] = {0};
	//char strResponse[RESPONSE_SIZE] = {0};
	memset(strMatch,0,sizeof(strMatch));

	sprintf(strMD5,"%s%s%s","none",rid,secret_key);
	headers_client(strMD5,strClient);

	//post:请求完整码库数据
	if(https_post_all(rid,secret_key,strMatch,strClient) != 0){
		printf("[%s] get yaokan data error \n",__func__);
		return -1;
	}


	//返回的数据
	//printf("strResponse:%s. \n",strResponse);

	//判断码库是否是含有特殊按键
	code_info_t code_info_msg;
	memset(&code_info_msg,0,sizeof(code_info_t));
	int key = 0;
	result_json = strstr(strMatch,"{");
	re_num = check_key_json(result_json,&key,sds_results_msg);
	if(re_num == -1)
	{
		printf("%s : parse json error.\n",__func__);
		return -1;//解析json失败
	}
	code_info_msg.key = key;
	sds_results_msg->key = key;
	
	//获取新码库时 将码库按键类型写入
	fd = fopen("/usr/data/bngoIR.record","w+");
	if(fd == NULL){
		printf("open bngoIR.txt error!!!!.\n");
		return -1;
	}
	fwrite(&code_info_msg,sizeof(code_info_msg),1,fd);
	fflush(fd);
	fclose(fd);


	//fd = fopen(c_path,"w+");
	//保存为测试文件  bngoIR
	fd = fopen("/usr/data/bngoIR.txt","w+");
	if(fd == NULL){
		printf("open bngoIR.txt error!!!!.\n");
		return -1;
	}
	fwrite(strMatch,strlen(strMatch)+1,1,fd);		
	fflush(fd);
	fclose(fd);

	return 0;
}


int send_Complete_code(char *path,char *key)
{
	char new_path[200] = {0};

	strcat(new_path,"/usr/data/");
	strcat(new_path,path);
	strcat(new_path,".txt");
	
	//判断码库不存在，退出
	if (access(new_path,F_OK) != 0)
		return CODE_NOT_EXIST;


	
	char record[200] = {0};
	strcat(record,"/usr/data/");
	strcat(record,path);
	strcat(record,".record");

	code_info_t code_info_msg;
	memset(&code_info_msg,0,sizeof(code_info_t));

	FILE *fd;

	//record 保存了按键信息
	fd = fopen(record,"r+");
	if(fd == NULL){
		printf("%s: open record error!!!!.\n",__func__);
		return CODE_NOT_EXIST;
	}
	fread(&code_info_msg,sizeof(code_info_t),1,fd);
	fclose(fd);

	//printf("---------read record msg: [cmd:%s] [key:%d ] ------\n",code_info_msg.old_command,code_info_msg.key);
	
	char *u_src = NULL;
	char rub_src[50] = {0};
	//判断rub_src2 是否含有特殊按键
	char rub_src2[50] = {0};
	// rub_src2 为发送出去的命令
	strcpy(rub_src2,key);
	
	if(code_info_msg.key == 2){
		//去掉u
		u_src = strstr(rub_src2,"u");
		if(u_src != NULL){
			strcpy(u_src,u_src+2);
		} 
	} else if(code_info_msg.key == 4){
		//去掉l
		
		u_src = strstr(rub_src2,"l");
		if(u_src != NULL){
			strcpy(u_src,u_src+2);
		}

	} else if(code_info_msg.key == 0){
		//如果有特殊按键  去掉u和l
		
		if(strstr(rub_src2,"*") == NULL){
			
			u_src = strstr(rub_src2,"u");
			if(u_src != NULL){	
				strcpy(u_src,u_src+2);
			} 
			u_src = strstr(rub_src2,"l");
			if(u_src != NULL){
				strcpy(u_src,u_src+2);
			}
			
		}
	} 
	
	//strcpy(code_info_msg.old_command,rub_src2);
		
	char strMatch[RESPONSE_SIZE] = {0};

	//读取码库
	fd = fopen(new_path,"r+");
	if(fd == NULL){
		printf("%s: open Complete_code.txt error!!!!.\n",__func__);
		return CODE_NOT_EXIST;
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
	int re_num = asr_complete_json(result_json,rid,name,&zip,src,rub_src2);
	if(re_num == -1)
	{
		printf("%s : json resolve error.\n",__func__);
		return INVALID_COMMAND;//解析json失败
	}

	//printf("rid:%s ,name:%s ,zip:%d ,src:%s \n",rid,name,zip,src);
	
	/*********发送码库数据*****************/
	//1.获取激活码
	unsigned char strActivation[128] = {0};
	
	fd = fopen("/usr/data/activation_code.txt","r+");
	if(fd == NULL){
		printf("open activation_code.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fread(strActivation,128,1,fd);	
	fclose(fd);


	//bngo发送红外测试码
	char data[512] = {0};
	sprintf(data,"%s%d%s","0",zip,src);
	//printf("strActivation:%s data:%s .\n",strActivation,data);
	if(IR_send_data(strActivation,(unsigned char *)data) != 0)
		return COMPLETE_SCODE_ERR;

	//复制key 到code_info
	//strcpy(code_info_msg.old_command,rub_src2);
	//printf("\n ###发送 rub_src2:%s 旧的 code_info_msg.old_command:%s 新的 key:%s ### \n",rub_src2,code_info_msg.old_command,key);
	strcpy(code_info_msg.old_command,key);
	
	/*************记录历史命令*******************/
	if(strcmp(key,"off") != 0 && strstr(key,"*") == NULL){
		fd = fopen(record,"w+");
		if(fd == NULL){
			printf("%s: open record error!!!!.\n",__func__);
			return OPEN_FILE_ERR;
		}
		fwrite(&code_info_msg,sizeof(code_info_t),1,fd);
		fflush(fd);
		fclose(fd);

		//printf("----fflush write record msg: [cmd:%s] [key:%d ] ------\n",code_info_msg.old_command,code_info_msg.key);
	}
	
	return 0;
}

int akeymatch_to_app_air(char *ASR_COMMAND)
{
	char strMatch[RESPONSE_SIZE] = {0};
	//判断匹配码库是否存在
	if (access("/usr/data/match_code.txt",F_OK) != 0)
		return MATCH_GCODE_ERR;
	
	FILE *fd;
	fd = fopen("/usr/data/match_code.txt","r+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fread(strMatch,RESPONSE_SIZE,1,fd);
	
	fclose(fd);

	//printf("strMatch:%s\n",strMatch);	

	char *result_json = NULL;
	result_json = strstr(strMatch,"{");
	int re_num = akeymatch_to_app_json_air(result_json,ASR_COMMAND);
	if(re_num != 0)
	{
		printf("%s : parse json error.\n",__func__);
		return re_num;//解析json失败
	}

	return 0;
}


int akeymatch_to_app_other(char *ASR_COMMAND)
{
	char strMatch[RESPONSE_SIZE] = {0};
	//判断匹配码库是否存在
	if (access("/usr/data/match_code.txt",F_OK) != 0)
		return MATCH_GCODE_ERR;
	
	FILE *fd;
	fd = fopen("/usr/data/match_code.txt","r+");
	if(fd == NULL){
		printf("open match_code.txt error!!!!.\n");
		return OPEN_FILE_ERR;
	}
	fread(strMatch,RESPONSE_SIZE,1,fd);
	
	fclose(fd);

	//printf("strMatch:%s\n",strMatch);	

	char *result_json = NULL;
	result_json = strstr(strMatch,"{");
	int re_num = akeymatch_to_app_json_other(result_json,ASR_COMMAND);
	if(re_num != 0)
	{
		printf("%s : parse json error.\n",__func__);
		return re_num;//解析json失败
	}

	return 0;
}

