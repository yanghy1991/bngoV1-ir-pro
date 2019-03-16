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
	OPEN_FILE_ERR,
	INVALID_OLD_COMMAND,
	READ_IR_LEARN_ERR,
	LEARN_PATH_EMPTY,
	SEND_IR_LEARN_ERR,
};


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
//4�������豸���ͻ�ȡ��ң���豸Ʒ���б���app��֪
int get_Brand_list(char *t);
//5������Ʒ��ID���豸���ͻ�ȡң�����б�����������-ƥ���ã���д�뱾��
int get_Match_code(char *bid,char *t,char *v);
//6������ң����ID��ȡң������⣨�������ݣ���д�뱾��,number:ƥ���������
int get_Complete_code(char *t);

//����ƥ�����ݣ�number:ƥ���������
int send_Match_code(int t,int number,char *key);
//�����������ݣ�path:������ƣ�key:ң������
int send_Complete_code(char *path,char *key);

#endif //_CACERTINMEM_H_