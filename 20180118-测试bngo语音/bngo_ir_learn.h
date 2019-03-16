#ifndef __BNGO_IR_LEARN_H_
#define __BNGO_IR_LEARN_H_

typedef unsigned char uint8;
typedef unsigned int uint32;

//红外发送接口
int IR_SEND(uint32 time,int set);
//读取学习数据，然后发送
int READ_IR_LEARN();
//读取本地学习数据，然后发送
int SEND_IR_LEARN(char *filename,int key);
//进入红外学习模式
int IR_LEARN_SET(char byte);


#endif //__BNGO_IR_LEARN_H_
