#ifndef __BNGO_LED_H_
#define __BNGO_LED_H_

int bngo_led_open(char *pin);
int bngo_led_close(char *pin);
//夜灯
int bngo_led_write(unsigned int led_level);

//bngo按键调节夜灯亮度
int key_use_led();
//APP按键调节夜灯亮度
int app_use_led(char *appCommand);
//语音调节夜灯亮度
int server_use_led(char *new_command,char *ASR_COMMAND);

#endif //__BNGO_LED_H_