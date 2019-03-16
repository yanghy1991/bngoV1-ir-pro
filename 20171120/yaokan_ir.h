#ifndef __YAOKAN_IR_H_
#define __YAOKAN_IR_H_
/*************************************************
使用本程序时，按照以下流程使用
1.注册设备,注册成功返回0，失败 -1
密钥返回到secret_key，用secret_key到云端激活该设备，云端返回激活码Activation code
*************************************************/
int Registration_equipment(unsigned char*secret_key);
/*************************************************
2.发射红外码，发射成功返回0，失败-1
Activation code为该设备的激活码，云端接口可以获取
data 数据结构，数据皆为字符串  0+加密方式+云端码库数据据+换行符（'/n'）
加密方式有1与2，红外码库数据可以获取到加密方式，data大小设置成1024字节
*************************************************/
int IR_send_data(unsigned char*Activation_code,unsigned char* data);
#endif
