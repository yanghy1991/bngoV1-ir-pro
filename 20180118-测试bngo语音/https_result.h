/***************************************************************************
 * 
 * 数据解析
 * 
 **************************************************************************/
#ifndef  __HTTPS_RESULT_H_
#define  __HTTPS_RESULT_H_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"


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
char * asr_make_json(char* format_data,char *token_data,char *cuid,char* base64_data, int data_len);

/**
*功能:判断http返回是否正确
*参数:
*response:http响应的字符串
*
*/
int asr_get_http_result(char *response);

/**
*功能:对http响应的json字符串进行解析,解析出参数ret_msg,ret_code
*参数:
*json_str:json字符串
*ret_msg:用来存储激活码
*ret_code:用来存储返回值
*
*/
int asr_acode_json(char *json_str,char *ret_msg,int *ret_code);


/**
*功能:对http响应的json字符串进行解析,解析出参数err_msg,err_no,result
*参数:
*json_str:json字符串
*sm:用来存储数组总数
*rid:遥控器id
*name:品牌名称
*zip:压缩方式
*n:发送码库序号
*/
int asr_mcode_json(char *json_str,int *sm,char *rid,int *v,char *name,int *zip,char *src,int n,char *k);

/**
*功能:对http响应的json字符串进行解析,解析得出src:发送的键值
*参数:
*json_str:json字符串
*rid:遥控器id
*name:品牌名称
*zip:压缩方式
*src:发送的码库数据
*key:键值
*v:码库的版本
*/
int asr_complete_json(char *json_str,char *rid,char *name,int *zip,char *src,char *key);



