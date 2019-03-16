#include "https_result.h" 
#include <unistd.h>

/**
*功能:使用cJSON生成json字符串
*参数:
*format_data:语音压缩的格式,不区分大小写,pcm（不压缩）、wav、opus、speex、amr
*token_data:开放平台获取到的开发者 access_token
*cuid:用户唯一标识，用来区分用户，填写机器 MAC 地址或 IMEI 码，长度为60以内
*base64_data:真实的语音数据 ，需要进行base64 编码
*data_len:原始语音长度，单位字节
*
*/
char * asr_make_json(char* format_data,char *token_data,char *cuid,char* base64_data, int data_len)
{
	cJSON * pJsonRoot = NULL;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		printf("%s : create cJSON failed!\n",__func__);
		return NULL;
	}
	cJSON_AddStringToObject(pJsonRoot, "format", format_data);
	cJSON_AddStringToObject(pJsonRoot, "token", token_data);
	cJSON_AddStringToObject(pJsonRoot, "cuid", cuid);
	cJSON_AddNumberToObject(pJsonRoot, "rate", 8000);
	cJSON_AddNumberToObject(pJsonRoot, "channel", 1);
	cJSON_AddNumberToObject(pJsonRoot, "len", data_len);
	cJSON_AddStringToObject(pJsonRoot, "speech", base64_data);
	
	char * p = cJSON_Print(pJsonRoot);
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		printf("%s : create cJSON string failed!\n",__func__);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	
	return p;	
}

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int asr_get_http_result(char *response)  
{ 
    char *ptmp = NULL; 

	//没有http响应头
    ptmp = (char*)strstr(response,"HTTP/1.1");  
    if(!ptmp){  
        printf("%s : http/1.1 not find.\n",__func__);  
        return -1;  
    }
	//HTTP访问出错
    if(atoi(ptmp + 9)!=200){  
        printf("%s : http response no OK(200).\n",__func__);  
        return -1;  
    }
	
	return 0;
} 

/**
*功能:对http响应的json字符串进行解析,解析出参数ret_msg,ret_code
*参数:
*json_str:json字符串
*ret_msg:用来存储激活码
*ret_code:用来存储返回值
*
*/
int asr_1code_json(char *json_str,int *sm,int *bid,char *name,int n)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * r_object = NULL;
	cJSON * c_sm = NULL;
	cJSON * c_bid = NULL;
	cJSON * c_name = NULL;
	cJSON * arrayRs = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	
	//获取 sm
	c_sm= cJSON_GetObjectItem(root, "sm");
	if(NULL == c_sm)
	{
		printf("%s : get err_no failed!\n",__func__);
		return -1;//获取err_no失败
	}
	*sm = c_sm->valueint;
	
	//获取 rs 数组
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL == arrayRs)
	{
		printf("%s : get arrayRs failed!\n",__func__);
		return -1;//获取err_msg失败
	}

	//获取指定编号对应的匹配码库数据
	r_object = cJSON_GetArrayItem(arrayRs,n);

	//获取 rid
	c_bid = cJSON_GetObjectItem(r_object, "bid");
	if(NULL == c_bid)
	{
		printf("%s : get c_bid failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	*bid = c_bid->valueint;

	//获取 name
	c_name= cJSON_GetObjectItem(r_object, "name");
	if(NULL == c_name)
	{
		printf("%s : get c_name failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	memcpy(name,c_name->valuestring,strlen(c_name->valuestring));

	cJSON_Delete(root);
	return 0;
}

int asr_2code_json(char *json_str,int bid,char *path)
{
	char data[100] = {0};

	
	FILE *fd;
	fd = fopen(path,"w+");
	if(fd == NULL){
		printf("open 2.txt error!!!!.\n");
		return -1;
	}
	//printf("[%s] path:%s ###\n",__func__,path);

	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * c_sm = NULL;
	cJSON * arrayRs = NULL;
	cJSON * r_object = NULL;
	cJSON * rmodel = NULL;
	cJSON * v = NULL;
	
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	//获取 sm
	c_sm= cJSON_GetObjectItem(root, "sm");
	if(NULL == c_sm)
	{
		printf("%s : get err_no failed!\n",__func__);
		return -1;//获取err_no失败
	}
	int sm = c_sm->valueint;
	
	sprintf(data,"%d%s%d%s",bid,":",sm,"\n");	
	fwrite(data,strlen(data),1,fd);
	
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL == arrayRs)
	{
		printf("%s : get arrayRs failed!\n",__func__);
		return -1;//获取err_no失败
	}	
	
	int i;
	
	for(i = 0;i<sm;i++){
		//获取指定编号对应的匹配码库数据
		r_object = cJSON_GetArrayItem(arrayRs,i);
		
		rmodel = cJSON_GetObjectItem(r_object,"rmodel");
		if(NULL == rmodel)
		{
			printf("%s : get rmodel failed!\n",__func__);
			return -1;//获取err_no失败
		}

		v = cJSON_GetObjectItem(r_object,"v");
		if(NULL == v)
		{
			printf("%s : get v failed!\n",__func__);
			return -1;//获取err_no失败
		}	

		memset(data,0,100);
		sprintf(data,"%s%s%d%s",rmodel->valuestring,":",v->valueint,"\n");	
		fwrite(data,strlen(data),1,fd);	
	}


	cJSON_Delete(root);
	
	fclose(fd);	
	return 0;
}


//获取品牌下第一个rid
int asr_3code_json(char *json_str,char *rid)
{
	cJSON * root = NULL;
	cJSON * arrayRs = NULL;
	cJSON * r_object = NULL;
	cJSON * c_rid = NULL;
	
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL == arrayRs)
	{
		printf("%s : get arrayRs failed!\n",__func__);
		return -1;//获取err_no失败
	}	
	

	//获取指定编号对应的匹配码库数据
	r_object = cJSON_GetArrayItem(arrayRs,0);
		
	c_rid = cJSON_GetObjectItem(r_object,"rid");
	if(NULL == c_rid)
	{
		printf("%s : get rmodel failed!\n",__func__);
		return -1;//获取err_no失败
	}
	memcpy(rid,c_rid->valuestring,strlen(c_rid->valuestring));

	cJSON_Delete(root);

	return 0;
}

int asr_4code_json(char *json_str,char *path)
{
	cJSON * root = NULL;
	cJSON * error = NULL;
	cJSON * arrayItem = NULL;
	cJSON * arrayRs = NULL;
	
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return 5;
    }	

	//获取 error 错误数据，返回原因
	error = cJSON_GetObjectItem(root, "error");
	if(error != NULL)
	{
		printf("%s : get error failed![%s]\n",__func__,error->valuestring);
		cJSON_Delete(root);
		return 18;//获取err_no失败
	}

	//获取 rc_command 数组
	arrayItem = cJSON_GetObjectItem(root, "rc_command");
	if(NULL == arrayItem)
	{
		printf("%s : get arrayRc failed!\n",__func__);
		cJSON_Delete(root);
		return 5;//获取err_msg失败
	}

	
	FILE *fd;
	fd = fopen(path,"w+");
	if(fd == NULL){
		printf("open 2.txt error!!!!.\n");
		return -1;
	}

	int size =cJSON_GetArraySize(arrayItem);  
	//printf("cJSON_GetArraySize: size=%d\n",size); 

	int i = 0;
	for(i = 0;i<size;i++){
		//printf("arrayItem:%s \n",arrayItem->string);
		arrayRs = cJSON_GetArrayItem(arrayItem,i);
		fwrite(arrayRs->string,strlen(arrayRs->string),1,fd);	
		if(i != (size-1))
			fwrite(":",strlen(":"),1,fd);
	}

	fclose(fd);
	cJSON_Delete(root);

	return 0;
}

/**
*功能:对http响应的json字符串进行解析,解析出参数rs,sm
*参数:
*json_str:json字符串
*rs:用来存储匹配数据
*sm:用来存储型号总数
*rid:遥控器id
*name:品牌名称
*zip:压缩方式
*n:匹配码库序号
*key:发送的匹配键值
*/
int asr_mcode_json(char *json_str,int *sm,char *rid,char *name,int *zip,char *src,int n,char *key)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * r_object = NULL;
	cJSON * c_sm = NULL;
	cJSON * c_rid = NULL;
	cJSON * c_name = NULL;
	cJSON * c_zip = NULL;
	cJSON * c_src = NULL;
	cJSON * arrayRs = NULL;
	cJSON * arrayRc = NULL;
	cJSON * c_key = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	//获取 sm
	c_sm= cJSON_GetObjectItem(root, "sm");
	if(NULL == c_sm)
	{
		printf("%s : get err_no failed!\n",__func__);
		return -1;//获取err_no失败
	}
	*sm = c_sm->valueint;

	//获取 rs 数组
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL == arrayRs)
	{
		printf("%s : get arrayRs failed!\n",__func__);
		return -1;//获取err_msg失败
	}

	//int size =cJSON_GetArraySize(arrayRs);  
    //printf("cJSON_GetArraySize: size=%d\n",size); 

	//获取指定编号对应的匹配码库数据
	r_object = cJSON_GetArrayItem(arrayRs,n);

	//获取 rid
	c_rid = cJSON_GetObjectItem(r_object, "rid");
	if(NULL == c_rid)
	{
		printf("%s : get c_rid failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	memcpy(rid,c_rid->valuestring,strlen(c_rid->valuestring));

	//获取 name
	c_name= cJSON_GetObjectItem(r_object, "name");
	if(NULL == c_rid)
	{
		printf("%s : get c_name failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	memcpy(name,c_name->valuestring,strlen(c_name->valuestring));

	//获取 zip
	c_zip = cJSON_GetObjectItem(r_object, "zip");
	if(NULL == c_zip)
	{
		printf("%s : get c_zip failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	*zip = c_zip->valueint;

	//获取 rc_command 数组
	arrayRc = cJSON_GetObjectItem(r_object, "rc_command");
	if(NULL == arrayRc)
	{
		printf("%s : get arrayRc failed!\n",__func__);
		return -1;//获取err_msg失败
	}

	//获取 on 数组
	c_key = cJSON_GetObjectItem(arrayRc, key);
	if(NULL == c_key)
	{
		printf("%s : get c_key failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	
	c_src = cJSON_GetObjectItem(c_key,"src");
	if(NULL == c_src)
	{
		printf("%s : get c_src failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	memcpy(src,c_src->valuestring,strlen(c_src->valuestring));

	cJSON_Delete(root);
	return 0;
}



/**
*功能:对http响应的json字符串进行解析,解析得出src:发送的键值
*参数:
*json_str:json字符串
*rid:遥控器id
*name:品牌名称
*zip:压缩方式
*src:发送的码库数据
*key:键值
*/
int asr_complete_json(char *json_str,char *rid,char *name,int *zip,char *src,char *key)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * c_rid = NULL;
	cJSON * c_name = NULL;
	cJSON * c_zip = NULL;
	cJSON * c_src = NULL;
	cJSON * arrayRc = NULL;
	cJSON * c_key = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	
	//获取 rid
	c_rid = cJSON_GetObjectItem(root, "rid");
	if(NULL == c_rid)
	{
		printf("%s : get c_rid failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	memcpy(rid,c_rid->valuestring,strlen(c_rid->valuestring));

	//获取 name
	c_name= cJSON_GetObjectItem(root, "name");
	if(NULL == c_rid)
	{
		printf("%s : get c_name failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	memcpy(name,c_name->valuestring,strlen(c_name->valuestring));

	//获取 zip
	c_zip = cJSON_GetObjectItem(root, "zip");
	if(NULL == c_zip)
	{
		printf("%s : get c_zip failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	*zip = c_zip->valueint;	

	//获取 rc_command 数组
	arrayRc = cJSON_GetObjectItem(root, "rc_command");
	if(NULL == arrayRc)
	{
			printf("%s : get arrayRc failed!\n",__func__);
			return -1;//获取err_msg失败
	}
	
	//获取 key 数组
	c_key = cJSON_GetObjectItem(arrayRc, key);
	if(NULL == c_key)
	{
		printf("%s : get c_key failed!\n",__func__);
		return -1;//获取err_msg失败
	}
		
	c_src = cJSON_GetObjectItem(c_key,"src");
	if(NULL == c_src)
	{
		printf("%s : get c_src failed!\n",__func__);
		return -1;//获取err_msg失败
	}
	memcpy(src,c_src->valuestring,strlen(c_src->valuestring));
		
	cJSON_Delete(root);
	return 0;
}
static int count = 0; 

int asr_mcode_msg(char *json_str,int bid,char *path)
{
	
	
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * r_object = NULL;
	cJSON * c_sm = NULL;
	cJSON * c_name = NULL;
	cJSON * arrayRs = NULL;
	cJSON * c_v = NULL;
	
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	//获取 sm
	c_sm= cJSON_GetObjectItem(root, "sm");
	if(NULL == c_sm)
	{
		printf("%s : get err_no failed!\n",__func__);
		return -1;//获取err_no失败
	}
	//*sm = c_sm->valueint;

	//获取 rs 数组
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL == arrayRs)
	{
		printf("%s : get arrayRs failed!\n",__func__);
		return -1;//获取err_msg失败
	}

	//int size =cJSON_GetArraySize(arrayRs);  
    //printf("cJSON_GetArraySize: size=%d\n",size); 
	
	#if 0
	char buf[512] = {0};
	sprintf(buf,"%s%s%d",path,":",c_sm->valueint);
	count += c_sm->valueint;
	char src[512] = {0};
	strcat(src,"echo \"");
	strcat(src,buf);
	strcat(src,"\" >> air_code.txt");
	system(src);
	usleep(100000);
	
	printf("count :%d #####\n",count);
	#endif
	
	#if 1
	FILE *fd;
	fd = fopen(path,"w+");
	if(fd == NULL){
		printf("open 2.txt error!!!!.\n");
		return -1;
	}
	
	char buf[512] = {0};
	
	sprintf(buf,"%d%s%d%s",bid,":",c_sm->valueint,"\n");
	fwrite(buf,strlen(buf),1,fd);
	
	printf("buf:%s \n",buf);

	int i = 0;
	for(i = 0;i<c_sm->valueint;i++){
			//获取指定编号对应的匹配码库数据
			r_object = cJSON_GetArrayItem(arrayRs,i);

			//获取 name
			c_name = cJSON_GetObjectItem(r_object, "be_rmodel");
			if(NULL == c_name)
			{
				printf("%s : get be_rmodel failed!\n",__func__);
				return -1;//获取err_msg失败
			}
			
			//获取 name
			c_v = cJSON_GetObjectItem(r_object,"v");
			if(NULL == c_v)
			{
				printf("%s : get c_v failed!\n",__func__);
				return -1;//获取err_msg失败
			}
			
			//printf("c:%d \n",c_v->valuestring);
			
			memset(buf,0,512);
			sprintf(buf,"%s%s%d%s",c_name->valuestring,":",c_v->valueint,"\n");
			fwrite(buf,strlen(buf),1,fd);
		
	}
	
	fclose(fd);
	
	#endif
	cJSON_Delete(root);
	return 0;
}

















