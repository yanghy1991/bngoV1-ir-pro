#ifndef __ALL_HANDLER_H_
#define __ALL_HANDLER_H_

#include "cacertinmem.h"

#define CODEPATH    "/usr/data/"

//ҹ��
int led_handler_func(char  *n_path,char *token,char *ASR_COMMAND);
//������
int box_handler_func(char *v,char *name,char  *n_path,char *token);
//����
int tv_handler_func(char *v,char *name,char  *n_path,char *token);
//�յ�
int air_handler_func(char *v,char *name,char  *n_path,char *token,char *ASR_COMMAND);
//����
int fan_handler_func(char *v,char *name,char  *n_path,char *token);



//��ͨƥ��ģʽ
int match_handler_func(char *token,result_to_mozart_t *sds_results_msg);
//һ��ƥ��ģʽ
int akeymatch_handler_func(char *token,char *ASR_COMMAND);
//һ��ƥ�䷢��ģʽ
int akmsend_handler_func(char *token,result_to_mozart_t *sds_results_msg);
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

//����������� ��code_list.txt
int save_code_msg(char *code_v,char *code_name);
//ɾ��������� ��code_list.txt ���line = 1 ��������һ����������
int delete_code_name(char *del_code_name);
#endif //__ALL_HANDLER_H_