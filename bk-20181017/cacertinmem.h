#ifndef _CACERTINMEM_H_
#define _CACERTINMEM_H_

#define POSTSTR_SIZE 256
#define HEARD_SIZE 22
#define MD5_SIZE  256
#define RESPONSE_SIZE 200*1024

/*
enum error_list{
	SECRET_KEY_ERR = 1,      //bngo��ȡ�豸idʧ��
	ACTIVATION_CODE_ERR,	 //bngo��ȡ������ʧ��
	ELECTRICAL_LIST_ERR,	 //bngo��ȡ��������ʧ��
	BRAND_LIST_ERR,			 //bngo��ȡƷ���б�ʧ��
	MATCH_GCODE_ERR,		 //bngo��ȡƥ�����ʧ��
	MATCH_SCODE_ERR,		 //bngo����ƥ�����ʧ��
	COMPLETE_GCODE_ERR,		 //bngo��ȡ�������ʧ��
	COMPLETE_SCODE_ERR,		 //bngo�����������ʧ��
	INVALID_COMMAND,		 //bngo���ܵ���Ч����
	CODE_NOT_EXIST,			 //bngoû�з��ֱ������(�ļ���)
	OPEN_FILE_ERR,			 //bngo�򿪱����ļ�����
	INVALID_OLD_COMMAND,	 //bngo��ʷ�����ʽ����
	READ_IR_LEARN_ERR,		 //bngo��ȡ����ѧϰ����ʧ��
	LEARN_PATH_EMPTY,		 //bngo��ȡ���Ժ���ѧϰ�ļ�Ϊ��
	SEND_IR_LEARN_ERR,		 //bngo���ͺ���ѧϰ����ʧ��
	CODE_V_ERR,				 //���汾У��ʧ��
	SETIR_FILE_ERR,			 //����Ĭ��ң����ʧ��
};
*/

enum error_list{
	SECRET_KEY_ERR = 1,
	ACTIVATION_CODE_ERR,
	ELECTRICAL_LIST_ERR,
	BRAND_LIST_ERR,
	MATCH_GCODE_ERR,
	MATCH_SCODE_ERR,
	COMPLETE_GCODE_ERR,
	COMPLETE_SCODE_ERR,
	INVALID_COMMAND,
	CODE_NOT_EXIST,
	OPEN_FILE_ERR,			//���ļ�ʧ��  11 
	INVALID_OLD_COMMAND,	//��ʷ������Ч  12 
	READ_IR_LEARN_ERR,		//ƥ�䳬ʱ  13
	LEARN_PATH_EMPTY,
	SEND_IR_LEARN_ERR,
	CODE_V_ERR,
	SETIR_FILE_ERR,
	AKEYMATCH_GCODE_ERR,
	IR_LEARN_EXIT,			//�˳�����ѧϰģʽ 19
	SET_ASR_IR,
	ADD_CODE_ERR,
	DEL_CODE_ERR,
	LEARN_MOD_ERR,
	LEARN_SED_ERR,
	JSON_PARESE_ERR,		//��� json�������� 25
	JSON_KEY_NOT_EXIST,		//���û�иð�����Ϣ 26
	JSON_CODE_ERR,          //������error 27
	BIND_NOT_EXIST,			//�󶨹�ϵ������error 28
	NOCH_ERR,				//û�����Ƶ���б� 29
};

typedef struct code_info{
	int key;
	char old_command[128];
}code_info_t;

typedef struct result_to_mozart{
	int key;
	char ASR_COMMAND[1024];
}result_to_mozart_t;


//success return CURLE_OK
int https_post(const char *strPost,char *strResponse,char *strClient);
//��ȡ�������
int https_post_all(const char *c_rid,const unsigned char *secret_key,char *strResponse,char *strClient);

//1����f:�豸idд�뱾��
int get_secret_key();
//2��ע���豸id����ȡ�����룬д�뱾��
int get_Activation_code();
//3����ȡ��ң���豸�����б�,app��֪
int get_Electrical_list();
//4�������豸���ͻ�ȡ��ң���豸Ʒ���б�app��֪
int get_Brand_list(char *t);
//5������Ʒ��ID���豸���ͻ�ȡң�����б���������-ƥ���ã���д�뱾��
int get_Match_code(char *bid,char *t,char *v);
//6������ң����ID��ȡң������⣨�������ݣ���д�뱾��,number:ƥ���������
int get_Complete_code(char *t,result_to_mozart_t *sds_results_msg);
//7������ң����ID�������ࡢ����������һ��ƥ��ģʽ
int ont_key_match(char *bid,char *t,char *src);

//����ƥ�����ݣ�number:ƥ���������
int send_Match_code(int t,int number,char *key,result_to_mozart_t *sds_results_msg);
//�����������ݣ�path:������ƣ�key:ң������  �յ���(APP)
int send_Complete_code(char *path,char *key);

//�����������ݣ�path:������ƣ�key:ң������  �ǿյ���(APP)
int send_Complete_code_other(char *path,char *key);

//6������ң����ID��ȡң������⣨�������ݣ���д�뱾��
int get_amatch_Complete_code(char *t,result_to_mozart_t *sds_results_msg);

#endif //_CACERTINMEM_H_