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
		jerror = cJSON_GetObjectItem(root, "error");
		if(jerror != NULL)
		{
			printf("%s : get err_msg failed! error:[%s]\n",__func__,jerror->string);
			return -1;//获取err_msg失败
		}
		
		code = cJSON_GetObjectItem(root, "ret_code");
		if(NULL == code)
		{
			printf("%s : get ret_code failed!\n",__func__);
			return -1;//获取err_no失败
		}
		//printf("err_no : %d\n", err_no->valueint);
		*ret_code = code->valueint;
		
		msg = cJSON_GetObjectItem(root, "ret_msg");
		if(NULL == msg)
		{
			printf("%s : get err_msg failed!\n",__func__);
			return -1;//获取err_msg失败
		}
		//printf("err_msg : %s\n", cJSON_Print(err_msg));
		
		memcpy(ret_msg,msg->valuestring,strlen(msg->valuestring)+1);
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
int asr_mcode_json(char *json_str,int *sm,char *rid,int *v,char *name,int *zip,char *src,int n,char *key,result_to_mozart_t *sds_results_msg)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * error = NULL;
	cJSON * r_object = NULL;
	cJSON * c_sm = NULL;
	cJSON * c_rid = NULL;
	cJSON * c_v = NULL;
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
	error = cJSON_GetObjectItem(root, "error");
	if(error != NULL)
	{
		printf("%s : get error failed![%s]\n",__func__,error->valuestring);
		memcpy(sds_results_msg->ASR_COMMAND,error->valuestring,strlen(error->valuestring)+1);
		goto error_ret;
		//获取err_no失败
	}

	//获取 sm
	c_sm= cJSON_GetObjectItem(root, "sm");
	if(NULL == c_sm)
	{
		printf("%s : get err_no failed!\n",__func__);
		goto error_ret;
		//获取err_no失败

	}
	*sm = c_sm->valueint;

	//获取 rs 数组
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL == arrayRs)
	{
		printf("%s : get arrayRs failed!\n",__func__);
		goto error_ret;
		//获取arrayRs失败

	}

	int size =cJSON_GetArraySize(arrayRs);  
	if(n > size - 1){
		printf("[%s] 指定序号超出匹配码库编号 \n",__func__);
		goto error_ret;
	}
    //printf("cJSON_GetArraySize: size=%d\n",size); 

	//获取指定编号对应的匹配码库数据
	r_object = cJSON_GetArrayItem(arrayRs,n);

	//获取 rid
	c_rid = cJSON_GetObjectItem(r_object, "rid");
	if(NULL == c_rid)
	{
		printf("%s : get c_rid failed!\n",__func__);
		goto error_ret;
		//获取c_rid失败

	}
	memcpy(rid,c_rid->valuestring,strlen(c_rid->valuestring)+1);

	//获取 v
	c_v = cJSON_GetObjectItem(r_object, "v");
	if(NULL == c_v)
	{
		printf("%s : get c_v failed!\n",__func__);
		goto error_ret;
		//获取c_v失败

	}	
	*v = c_v->valueint;
	
	//获取 name
	c_name= cJSON_GetObjectItem(r_object, "name");
	if(NULL == c_name)
	{
		printf("%s : get c_name failed!\n",__func__);
		goto error_ret;
		//获取c_name失败

	}
	memcpy(name,c_name->valuestring,strlen(c_name->valuestring)+1);

	//获取 zip
	c_zip = cJSON_GetObjectItem(r_object, "zip");
	if(NULL == c_zip)
	{
		printf("%s : get c_zip failed!\n",__func__);
		goto error_ret;
		//获取c_zip失败

	}
	*zip = c_zip->valueint;

	//获取 rc_command 数组
	arrayRc = cJSON_GetObjectItem(r_object, "rc_command");
	if(NULL == arrayRc)
	{
		printf("%s : get arrayRc failed!\n",__func__);
		goto error_ret;
		//获取arrayRc失败

	}

	//获取 on 数组
	c_key = cJSON_GetObjectItem(arrayRc, key);
	if(NULL == c_key)
	{
		printf("%s : get c_key failed!\n",__func__);
		goto error_ret;
		//获取c_key失败

	}
	
	c_src = cJSON_GetObjectItem(c_key,"src");
	if(NULL == c_src)
	{
		printf("%s : get c_src failed!\n",__func__);
		goto error_ret;
		//获取c_src失败
	}
	memcpy(src,c_src->valuestring,strlen(c_src->valuestring)+1);

	cJSON_Delete(root);
	return 0;

error_ret:
	cJSON_Delete(root);
	return -1;

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
	cJSON * jerror = NULL;
	
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return -1;
    }

	jerror = cJSON_GetObjectItem(root, "error");
	if(jerror != NULL)
	{
		printf("%s : get err_msg failed! error:[%s]\n",__func__,jerror->string);
		goto error_ret;//获取err_msg失败
	}
	
	//获取 rid
	c_rid = cJSON_GetObjectItem(root, "rid");
	if(NULL == c_rid)
	{
		printf("%s : get c_rid failed!\n",__func__);
		goto error_ret;//获取err_msg失败
	}
	memcpy(rid,c_rid->valuestring,strlen(c_rid->valuestring));

	//获取 name
	c_name= cJSON_GetObjectItem(root, "name");
	if(NULL == c_rid)
	{
		printf("%s : get c_name failed!\n",__func__);
		goto error_ret;//获取err_msg失败
	}
	memcpy(name,c_name->valuestring,strlen(c_name->valuestring)+1);

	//获取 zip
	c_zip = cJSON_GetObjectItem(root, "zip");
	if(NULL == c_zip)
	{
		printf("%s : get c_zip failed!\n",__func__);
		goto error_ret;//获取err_msg失败
	}
	*zip = c_zip->valueint;	

	//获取 rc_command 数组
	arrayRc = cJSON_GetObjectItem(root, "rc_command");
	if(NULL == arrayRc)
	{
			printf("%s : get arrayRc failed!\n",__func__);
			goto error_ret;//获取err_msg失败
	}

	//printf("############ [key:%s] #############\n",key);
	
	//获取 key 数组
	c_key = cJSON_GetObjectItem(arrayRc, key);
	if(NULL == c_key)
	{
		/*
		//删除扫风
		char *u_src = NULL;
		u_src = strstr(key,"l");
		if(u_src != NULL){
			strcpy(u_src,u_src+2);
			//printf("key:%s ######################\n",key);
			c_key = cJSON_GetObjectItem(arrayRc, key);
			if(NULL == c_key){
				printf("%s : get c_key failed!\n",__func__);
				goto error_ret;//获取err_msg失败
			}
		} else {
			printf("%s : get c_key failed!\n",__func__);
			goto error_ret;//获取err_msg失败
		}
		*/
		printf("%s : get c_key failed!\n",__func__);
		goto error_ret;//获取err_msg失败

	}
		
	c_src = cJSON_GetObjectItem(c_key,"src");
	if(NULL == c_src)
	{
		printf("%s : get c_src failed!\n",__func__);
		goto error_ret;//获取err_msg失败
	}
	memcpy(src,c_src->valuestring,strlen(c_src->valuestring)+1);
		
	cJSON_Delete(root);
	return 0;

error_ret:
	cJSON_Delete(root);
	return -1;

}

/**
*功能:删除无用的对象，把数据发回APP、空调类
*参数:
*json_str:json字符串
*ASR_COMMAND:返回的一键匹配数据如，rid v
*/
int akeymatch_to_app_json_air(char *json_str,char *ASR_COMMAND)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * arrayRs = NULL;
	cJSON * r_object = NULL;
	cJSON * error = NULL;
	
	root = cJSON_Parse(json_str);
	if (!root) 
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		printf("%s : parse json error.\n",__func__);  
		return 5;
    }	

	//获取 error
	error = cJSON_GetObjectItem(root, "error");
	if(error != NULL)
	{
		memcpy(ASR_COMMAND,error->valuestring,strlen(error->valuestring)+1);
		printf("%s : get error failed![%s]\n",__func__,error->valuestring);
		cJSON_Delete(root);
		return 18;//获取err_no失败
	}

	
	//获取 rs 数组
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL == arrayRs)
	{
		printf("%s : get arrayRs failed!\n",__func__);
		cJSON_Delete(root);
		return 5;//获取err_msg失败
	}

	int size =cJSON_GetArraySize(arrayRs);  
	//printf("cJSON_GetArraySize: size=%d\n",size); 

	int i;
	for(i = 0;i<size;i++){
		//获取指定编号对应的匹配码库数据
		r_object = cJSON_GetArrayItem(arrayRs,i);
		cJSON_DeleteItemFromObject(r_object, "name");
		cJSON_DeleteItemFromObject(r_object, "t");
		cJSON_DeleteItemFromObject(r_object, "be_rmodel");
		cJSON_DeleteItemFromObject(r_object, "rmodel");
		cJSON_DeleteItemFromObject(r_object, "rdesc");
		cJSON_DeleteItemFromObject(r_object, "order_no");
		cJSON_DeleteItemFromObject(r_object, "zip");
		cJSON_DeleteItemFromObject(r_object, "rc_command");
	}
		
	//printf("[%s]  :[size:%d ] %s \n",__func__,strlen(cJSON_Print(root)),cJSON_Print(root));
	
	//strcpy(ASR_COMMAND,cJSON_Print(root));
	//strcpy(ASR_COMMAND,cJSON_PrintUnformatted(root));
	char *s = cJSON_PrintUnformatted(root);
	memcpy(ASR_COMMAND,s,strlen(s)+1);


	if(s)
		free(s);

	cJSON_Delete(root);

	return 0;
}


/**
*功能:删除无用的对象，把数据发回APP、非空调类
*参数:
*json_str:json字符串
*ASR_COMMAND:返回的一键匹配数据如，rid v
*/
int akeymatch_to_app_json_other(char *json_str,char *ASR_COMMAND)
{
	//printf("%s\n",json_str);
	cJSON * root = NULL;
	cJSON * next_cmp_key = NULL;
	cJSON * r_object = NULL;
	cJSON * error = NULL;
	cJSON * arrayRs = NULL;
	cJSON * arrayItem = NULL;
	cJSON * c_rid = NULL;
	
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
		memcpy(ASR_COMMAND,error->valuestring,strlen(error->valuestring)+1);
		printf("%s : get error failed![%s]\n",__func__,error->valuestring);
		cJSON_Delete(root);
		return 18;//获取err_no失败
	}


	char *s = cJSON_PrintUnformatted(root);

	//获取 next_cmp_key 正常数据  返回next_cmp_key 和 rid
	next_cmp_key = cJSON_GetObjectItem(root, "next_cmp_key");
	if(next_cmp_key != NULL)
	{
		memcpy(ASR_COMMAND,s,strlen(s)+1);

		if(s)
			free(s);

		cJSON_Delete(root);
		return 0;//获取err_no失败
	}

#if 1
	//多个列表
	//获取 rs 数组
	arrayRs = cJSON_GetObjectItem(root, "rs");
	if(NULL != arrayRs)
	{
		int size =cJSON_GetArraySize(arrayRs);	
			//printf("cJSON_GetArraySize: size=%d\n",size); 
		
			int i;
			for(i = 0;i<size;i++){
				//获取指定编号对应的匹配码库数据
				r_object = cJSON_GetArrayItem(arrayRs,i);
				cJSON_DeleteItemFromObject(r_object, "name");
				cJSON_DeleteItemFromObject(r_object, "t");
				cJSON_DeleteItemFromObject(r_object, "be_rmodel");
				cJSON_DeleteItemFromObject(r_object, "rmodel");
				cJSON_DeleteItemFromObject(r_object, "rdesc");
				cJSON_DeleteItemFromObject(r_object, "order_no");
				cJSON_DeleteItemFromObject(r_object, "zip");
				cJSON_DeleteItemFromObject(r_object, "rc_command");
			}
				
			//printf("[%s]  :[size:%d ] %s \n",__func__,strlen(cJSON_Print(root)),cJSON_Print(root));
			//memcpy(ASR_COMMAND,cJSON_PrintUnformatted(root),strlen(cJSON_PrintUnformatted(root))+1);

		memcpy(ASR_COMMAND,s,strlen(s)+1);

		if(s)
			free(s);
		
		cJSON_Delete(root);
		return 0;//获取err_msg失败
	}
#endif

	//单个列表
	//获取 rc_command 数组
	arrayItem = cJSON_GetObjectItem(root, "rc_command");
	if(NULL == arrayItem)
	{
		printf("%s : get arrayRc failed!\n",__func__);
		free(s);
		cJSON_Delete(root);
		return 5;//获取err_msg失败
	}

	//获取 rid
	c_rid = cJSON_GetObjectItem(root, "rid");
	if(NULL == c_rid)
	{
		printf("%s : get c_rid failed!\n",__func__);
		free(s);
		cJSON_Delete(root);
		return 5;//获取err_msg失败
	}
	//memcpy(rid,c_rid->valuestring,strlen(c_rid->valuestring));


	//保存rid.txt  ADD添加时需要校验
	FILE *fd;
	fd = fopen("/usr/data/rid.txt","w+");
	if(fd == NULL){
		printf("open rid.txt error!!!!.\n");
		free(s);
		cJSON_Delete(root);
		return 5;
	}
	fwrite(c_rid->valuestring,strlen(c_rid->valuestring),1,fd);
	fclose(fd);

	
	//保存为缓存文件bngoIR
	fd = fopen("/usr/data/bngoIR.txt","w+");
	if(fd == NULL){
		printf("open bngoIR.txt error!!!!.\n");
		free(s);
		cJSON_Delete(root);
		return -1;
	}
	fwrite(s,strlen(s),1,fd);		
	fclose(fd);

	//删除无用数据 返回rid 和 v
	cJSON_DeleteItemFromObject(root, "codeset");
	cJSON_DeleteItemFromObject(root, "name");
	cJSON_DeleteItemFromObject(root, "t");
	cJSON_DeleteItemFromObject(root, "be_rmodel");
	cJSON_DeleteItemFromObject(root, "rmodel");
	cJSON_DeleteItemFromObject(root, "rdesc");
	cJSON_DeleteItemFromObject(root, "zip");
	cJSON_DeleteItemFromObject(root, "rc_command");
	cJSON_DeleteItemFromObject(root, "_o");
	cJSON_DeleteItemFromObject(root, "_z");

	//memcpy(ASR_COMMAND,cJSON_PrintUnformatted(root),strlen(cJSON_PrintUnformatted(root))+1);
	memcpy(ASR_COMMAND,s,strlen(s)+1);

	if(s)
		free(s);
	
	cJSON_Delete(root);
	
	return 0;
}


/**
*功能:判断获取的码库是否有特殊按键
*参数:
*json_str:json字符串
*key:发送的匹配键值
*/
int check_key_json(char *json_str,int *key,result_to_mozart_t *sds_results_msg)
{

		//printf("%s\n",json_str);
		cJSON * root = NULL;
		cJSON * c_rid = NULL;
		cJSON * c_v = NULL;
		cJSON * c_zip = NULL;
		cJSON * c_src = NULL;
		cJSON * arrayRc = NULL;
		cJSON * c_key = NULL;
		cJSON * jerror = NULL;
		cJSON * r_object = NULL;
		
		root = cJSON_Parse(json_str);
		if (!root) 
		{
			//printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			printf("%s : parse json error.\n",__func__);  
			return -1;
		}
	
		jerror = cJSON_GetObjectItem(root, "error");
		if(jerror != NULL)
		{
			memcpy(sds_results_msg->ASR_COMMAND,jerror->valuestring,strlen(jerror->valuestring)+1);
			printf("%s : get err_msg failed! error:[%s]\n",__func__,jerror->string);
			goto error_ret;//获取err_msg失败
		}

		//获取 v
		c_v = cJSON_GetObjectItem(root, "v");
		if(NULL == c_v)
		{
			printf("%s : get c_v failed!\n",__func__);
			goto error_ret;
			//获取c_v失败

		}	
		//*v = c_v->valueint;
		
		if(c_v->valueint == 1){
			//1版本的码库 key 6 为正常
			*key = 6;
			cJSON_Delete(root);
			return 0;
			
		}
		
		//获取 rc_command 数组
		arrayRc = cJSON_GetObjectItem(root, "rc_command");
		if(NULL == arrayRc)
		{
				printf("%s : get arrayRc failed!\n",__func__);
				goto error_ret;//获取err_msg失败
		}

		
		//获取指定编号对应的匹配码库数据
		r_object = cJSON_GetArrayItem(arrayRc,0);
		if(strstr(r_object->string,"*") != NULL){
			//特殊按键
			*key =  0; 
		} else {
			if(strstr(r_object->string,"l") != NULL){
				*key  |= (1<<1);
			}
			
			if(strstr(r_object->string,"u") != NULL){
				*key |=  (1<<2);
			}
		} 
		//printf(" -- key :[%d] --\n",*key);
		cJSON_Delete(root);
		return 0;
	
	error_ret:
		cJSON_Delete(root);
		return -1;
}

