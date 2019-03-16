#ifndef __BNGO_INFO_H_
#define __BNGO_INFO_H_

/**
 * @brief music_info_t BNGO信息结构体.
 */
typedef struct bngo_info_t {
	/**
	 * @brief led 夜灯亮度.
	 */
	char led_val[10];
	/**
	 * @brief led 夜灯开关标志.
	 */
	int led_ison;
	/**
	 * @brief 延时熄灯闹钟标志.
	 */
	int led_alarm_flag;

	/**
	 * @brief title 系统休眠.
	 */
	int sleep_flag;

	/**
	 * @brief title 显示时间
	 */
	int time_flag;
	
	/**
	 * @brief title 夜间模式
	 */
	int night_flag;
	/**
	 * @brief title 音乐助眠 cmd = 5
	 */
	int sleepMusic_flag;
}bngo_info_t;

/**
 * @brief sds_sleepmusic_file APP设置的“音乐助眠”内容结构体.
 */
typedef struct sds_sleepmusic_file{
	/**
	 * @brief input 音频
	 */
	char sm_source[10];
	/**
	 * @brief output 播放时长.
	 */
	char sm_times[10];
	/**   
	 * @brief src 音量渐变标志
	 */  
	char sm_volume_flag[10];
	/**   
	 * @brief src 开始音量
	 */  
	char sm_volume_start[10];
	/**   
	 * @brief src 夜灯渐变标志
	 */  
	char sm_led_flag[10];
	/**   
	 * @brief src 开始夜灯亮度
	 */  
	char sm_led_start[10];

}sds_sleepmusic_file;

int set_bngo_info(int cmd,bngo_info_t bngo_info_msg);
int get_bngo_info(bngo_info_t *bngo_info_msg);
//发送bngo info 给app
int send_bngo_info(char *ASR_COMMAND);
//修改设备名称
int set_bngo_name(char *name,char *ASR_COMMAND);
//获取bngo城市
int get_bngo_city(char *city);

//获取wifi信号强度
int get_wifi_signal(unsigned char *dbm);
//获取wifi SSID
int get_wifi_ssid(char *ssid);
//根据时间生成时间戳
int get_Timestamps(char *startDate);
//获取设备id aes 加密
int get_bngoID(char *bngoId);
//获取设备id 没有经过加密
int get_bngoID_mqttpub(char *bngoId);
//根据闹钟ID 删除对应的闹钟
int remove_Alarm(char *alarmId,char *alarmType);
//删除所有闹钟 apalarm bgalarm
int remove_ALLalarm(void);




#endif //__BNGO_INFO_H_

