#include "https_result.h" 

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
int asr_acode_json(char *json_str,char *ret_msg,int *ret_code)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * code = NULL;
	cJSON * msg = NULL;
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }
	else
	{
		code = cJSON_GetObjectItem(root, "ret_code");
		if(NULL == code)
		{
			printf("%s : get err_no failed!\n",__func__);
			return -1;//获取err_no失败
		}
		//printf("err_no : %d\n", err_no->valueint);
		*ret_code = code->valueint;
		
		msg = cJSON_GetObjectItem(root, "ret_msg");
		if(NULL == ret_msg)
		{
			printf("%s : get err_msg failed!\n",__func__);
			return -1;//获取err_msg失败
		}
		//printf("err_msg : %s\n", cJSON_Print(err_msg));
		
		strcpy(ret_msg,cJSON_Print(msg));
	}

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


















