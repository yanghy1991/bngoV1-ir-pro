#ifndef __BNGO_IR_LEARN_H_
#define __BNGO_IR_LEARN_H_

typedef unsigned char uint8;
typedef unsigned int uint32;

//红外学习接受的数据
typedef struct learn_head {
	char  t[10];
	char  bid[502];
}learn_head_t;


//红外发送接口
int IR_SEND(uint32 time,int set);
//读取学习数据，然后发送
int READ_IR_LEARN();
//读取本地学习数据，然后发送
int SEND_IR_LEARN(char *filename,int key);
//进入红外学习模式
int IR_LEARN_SET(char byte);

//一键匹配模式的红外学习数据
int MATCH_IR_LEARN(char *match_learn_src);

//所有电器自学习函数
int Ele_learn_func(char *t,char *bid,char *file_name,char *appCommand,char *ASR_COMMAND);

//所有电器发送函数 APP使用
int Ele_send_func(char *t,char *file_name,char *appCommand);

//机顶盒 自学习语音控制接口
int box_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND);
//风扇 自学习语音控制接口
int fan_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND);
//空调 自学习语音控制接口
int air_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND);




//获取学习码库头部信息
int get_learn_info(learn_head_t *info,char *file_name);
//根据命令 获取偏移量
int get_learn_offset(int *offset,char *appCommand);


#endif //__BNGO_IR_LEARN_H_
