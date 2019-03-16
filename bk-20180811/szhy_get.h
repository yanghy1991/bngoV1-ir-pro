#ifndef __SZHY_GET_H_
#define __SZHY_GET_H_

//bngo与服务器的通信接口
//cmd: del_alarm  data:闹钟id
//cmd: del_cozy   data:定时id
//cmd: del_code   data:码库id
int szhy_get(char *cmd,char *data);

#endif //__SZHY_GET_H_