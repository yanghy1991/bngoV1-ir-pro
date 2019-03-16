#ifndef _CACERTINMEM_H_
#define _CACERTINMEM_H_

#define POSTSTR_SIZE 256
#define HEARD_SIZE 22
#define MD5_SIZE  256
#define RESPONSE_SIZE 200*1024
#define ElectricalPATH  "/yuan/pro/https/20171120/YAOKAN/Electrical"
#define ROOTPATH  "/yuan/pro/https/20171120/YAOKAN/"
#define KEYPATH  "/yuan/pro/https/20171120/YAOKAN/key/"


/*
enum error_list{
	SECRET_KEY_ERR = 1,      //bngo获取设备id失败
	ACTIVATION_CODE_ERR,	 //bngo获取激活码失败
	ELECTRICAL_LIST_ERR,	 //bngo获取电器类型失败
	BRAND_LIST_ERR,			 //bngo获取品牌列表失败
	MATCH_GCODE_ERR,		 //bngo获取匹配码库失败
	MATCH_SCODE_ERR,		 //bngo发送匹配码库失败
	COMPLETE_GCODE_ERR,		 //bngo获取完整码库失败
	COMPLETE_SCODE_ERR,		 //bngo发送码库数据失败
	INVALID_COMMAND,		 //bngo接受到无效命令
	CODE_NOT_EXIST,			 //bngo没有发现本地码库(文件名)
	OPEN_FILE_ERR,			 //bngo打开本地文件出错
	INVALID_OLD_COMMAND,	 //bngo历史命令格式不对
	READ_IR_LEARN_ERR,		 //bngo读取红外学习数据失败
	LEARN_PATH_EMPTY,		 //bngo读取测试红外学习文件为空
	SEND_IR_LEARN_ERR,		 //bngo发送红外学习数据失败
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
//获取完整码库
int https_post_all(const char *c_rid,const unsigned char *secret_key,char *strResponse,char *strClient);

//1、将f:设备id写入本地
int get_secret_key();
//2、注册设备id，获取激活码，写入本地
int get_Activation_code();
//3、获取被遥控设备类型列表,app已知
int get_Electrical_list();
//4、根据设备类型获取被遥控设备品牌列表，app已知
int get_Brand_list(char *t,int number);
//5、根据品牌ID、设备类型获取遥控器列表（部分数据-匹配用），写入本地
int get_Match_code(char *bid,char *t,char *v);
//6、根据遥控器ID获取遥控器码库（完整数据），写入本地,number:匹配数据序号
int get_Complete_code(char *t);

//发送匹配数据，number:匹配数据序号
int send_Match_code(int t,int number,char *key);
//发送完整数据，path:码库名称，key:遥控命令
int send_Complete_code(char *path,char *key);
//获取遥控器键值
int get_all_key(char *t);

#endif //_CACERTINMEM_H_