#ifndef __ALL_HANDLER_H_
#define __ALL_HANDLER_H_

//夜灯
int led_handler_func(char  *n_path,char *token);
//机顶盒
int box_handler_func(char *v,char *name,char  *n_path,char *token);
//电视
int tv_handler_func(char *v,char *name,char  *n_path,char *token);
//空调
int air_handler_func(char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND);
//风扇
int fan_handler_func(char *v,char *name,char  *n_path,char *token);



//匹配模式
int match_handler_func(char *token);
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
#endif //__ALL_HANDLER_H_