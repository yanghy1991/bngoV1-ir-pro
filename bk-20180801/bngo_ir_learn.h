#ifndef __BNGO_IR_LEARN_H_
#define __BNGO_IR_LEARN_H_

typedef unsigned char uint8;
typedef unsigned int uint32;

//���ⷢ�ͽӿ�
int IR_SEND(uint32 time,int set);
//��ȡѧϰ���ݣ�Ȼ����
int READ_IR_LEARN();
//��ȡ����ѧϰ���ݣ�Ȼ����
int SEND_IR_LEARN(char *filename,int key);
//�������ѧϰģʽ
int IR_LEARN_SET(char byte);

//һ��ƥ��ģʽ�ĺ���ѧϰ����
int MATCH_IR_LEARN(char *match_learn_src);

#endif //__BNGO_IR_LEARN_H_
