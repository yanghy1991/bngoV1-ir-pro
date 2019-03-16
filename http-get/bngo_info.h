#ifndef __BNGO_INFO_H_
#define __BNGO_INFO_H_

/**
 * @brief music_info_t BNGO��Ϣ�ṹ��.
 */
typedef struct bngo_info_t {
	/**
	 * @brief led ҹ������.
	 */
	char led_val[10];
	/**
	 * @brief led ҹ�ƿ��ر�־.
	 */
	int led_ison;
	/**
	 * @brief ��ʱϨ�����ӱ�־.
	 */
	int led_alarm_flag;

	/**
	 * @brief title ϵͳ����.
	 */
	int sleep_flag;

	/**
	 * @brief title ��ʾʱ��
	 */
	int time_flag;
	
	/**
	 * @brief title ҹ��ģʽ
	 */
	int night_flag;
	/**
	 * @brief title �������� cmd = 5
	 */
	int sleepMusic_flag;
}bngo_info_t;

/**
 * @brief sds_sleepmusic_file APP���õġ��������ߡ����ݽṹ��.
 */
typedef struct sds_sleepmusic_file{
	/**
	 * @brief input ��Ƶ
	 */
	char sm_source[10];
	/**
	 * @brief output ����ʱ��.
	 */
	char sm_times[10];
	/**   
	 * @brief src ���������־
	 */  
	char sm_volume_flag[10];
	/**   
	 * @brief src ��ʼ����
	 */  
	char sm_volume_start[10];
	/**   
	 * @brief src ҹ�ƽ����־
	 */  
	char sm_led_flag[10];
	/**   
	 * @brief src ��ʼҹ������
	 */  
	char sm_led_start[10];

}sds_sleepmusic_file;

int set_bngo_info(int cmd,bngo_info_t bngo_info_msg);
int get_bngo_info(bngo_info_t *bngo_info_msg);
//����bngo info ��app
int send_bngo_info(char *ASR_COMMAND);
//�޸��豸����
int set_bngo_name(char *name,char *ASR_COMMAND);
//��ȡbngo����
int get_bngo_city(char *city);

//��ȡwifi�ź�ǿ��
int get_wifi_signal(unsigned char *dbm);
//��ȡwifi SSID
int get_wifi_ssid(char *ssid);
//����ʱ������ʱ���
int get_Timestamps(char *startDate);
//��ȡ�豸id aes ����
int get_bngoID(char *bngoId);
//��ȡ�豸id û�о�������
int get_bngoID_mqttpub(char *bngoId);
//��������ID ɾ����Ӧ������
int remove_Alarm(char *alarmId,char *alarmType);
//ɾ���������� apalarm bgalarm
int remove_ALLalarm(void);




#endif //__BNGO_INFO_H_

