#ifndef __BNGO_IR_LEARN_H_
#define __BNGO_IR_LEARN_H_

typedef unsigned char uint8;
typedef unsigned int uint32;

//����ѧϰ���ܵ�����
typedef struct learn_head {
	char  t[10];
	char  bid[502];
}learn_head_t;


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

//���е�����ѧϰ����
int Ele_learn_func(char *t,char *bid,char *file_name,char *appCommand,char *ASR_COMMAND);

//���е������ͺ��� APPʹ��
int Ele_send_func(char *t,char *file_name,char *appCommand);

//������ ��ѧϰ�������ƽӿ�
int box_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND);
//���� ��ѧϰ�������ƽӿ�
int fan_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND);
//�յ� ��ѧϰ�������ƽӿ�
int air_learn_func(char *t,char *file_name,char *token,char *ASR_COMMAND);




//��ȡѧϰ���ͷ����Ϣ
int get_learn_info(learn_head_t *info,char *file_name);
//�������� ��ȡƫ����
int get_learn_offset(int *offset,char *appCommand);


#endif //__BNGO_IR_LEARN_H_
