#ifndef __ALL_HANDLER_H_
#define __ALL_HANDLER_H_

//ҹ��
int led_handler_func(char  *n_path,char *token);
//������
int box_handler_func(char *v,char *name,char  *n_path,char *token);
//����
int tv_handler_func(char *v,char *name,char  *n_path,char *token);
//�յ�
int air_handler_func(char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND);
//����
int fan_handler_func(char *v,char *name,char  *n_path,char *token);



//ƥ��ģʽ
int match_handler_func(char *token);
//app ʹ�����
int use_handler_func(char *token);
//����ѧϰģʽ
int learn_handler_func(char *token);
//������ʹ�����
int server_handler_func(char *token,char *ASR_COMMAND);
//app ɾ�����
int delete_handler_func(char *token);
//app ������
int add_handler_func(char *token);
//setir ����Ĭ��ң����
int setir_handler_func(char *token);
#endif //__ALL_HANDLER_H_