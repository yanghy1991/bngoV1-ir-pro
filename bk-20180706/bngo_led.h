#ifndef __BNGO_LED_H_
#define __BNGO_LED_H_

int bngo_led_open(char *pin);
int bngo_led_close(char *pin);
//ҹ��
int bngo_led_write(unsigned int led_level);

//bngo��������ҹ������
int key_use_led();
//APP��������ҹ������
int app_use_led(char *appCommand);
//��������ҹ������
int server_use_led(char *new_command,char *ASR_COMMAND);

#endif //__BNGO_LED_H_