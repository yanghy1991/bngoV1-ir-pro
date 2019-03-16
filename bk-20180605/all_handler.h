#ifndef __ALL_HANDLER_H_
#define __ALL_HANDLER_H_

#include "cacertinmem.h"

#define CODEPATH    "/usr/data/"

//夜灯
int led_handler_func(char  *n_path,char *token,char *ASR_COMMAND);
//机顶盒
int box_handler_func(char *v,char *name,char  *n_path,char *token);
//电视
int tv_handler_func(char *v,char *name,char  *n_path,char *token);
//空调
int air_handler_func(char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND);
//风扇
int fan_handler_func(char *v,char *name,char  *n_path,char *token);



//普通匹配模式
int match_handler_func(char *token,result_to_mozart_t *sds_results_msg);
//一键匹配模式
int akeymatch_handler_func(char *token,char *ASR_COMMAND);
//一键匹配发送模式
int akmsend_handler_func(char *token,result_to_mozart_t *sds_results_msg);
//app 使用码库
int use_handler_func(char *token);
//进入学习模式
int learn_handler_func(char *token);
//服务器使用码库
int server_handler_func(char *token,char *ASR_COMMAND);
//app 删除码库
int delete_handler_func(char *token);
//app 添加码库
int add_handler_func(char *token);
//setir 设置默认遥控器
int setir_handler_func(char *token);

//保存码库名称 在code_list.txt
int save_code_msg(char *code_v,char *code_name);
//删除码库名称 从code_list.txt 如果line = 1 返回最有一个码库的名称
int delete_code_name(char *del_code_name);
#endif //__ALL_HANDLER_H_