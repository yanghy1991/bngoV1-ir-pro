#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define SYSFS_GPIO_EXPORT        "/sys/class/gpio/export"
#define SYSFS_GPIO_DIR_OUT       "out"
#define SYSFS_GPIO_PIN_VAL_H     "1"
#define SYSFS_GPIO_PIN_VAL_L     "0"


//双色指示灯 LEDA1- :PA10  LEDA2- :PA7
#define SYSFS_GPIO_PIN_PA10 		"10"
#define SYSFS_GPIO_PIN_PA7  		"7"
#define SYSFS_GPIO_PIN_PA10_DIR      "/sys/class/gpio/gpio10/direction"
#define SYSFS_GPIO_PIN_PA7_DIR      "/sys/class/gpio/gpio7/direction"
#define SYSFS_GPIO_PIN_PA10_VAL      "/sys/class/gpio/gpio10/value"
#define SYSFS_GPIO_PIN_PA7_VAL      "/sys/class/gpio/gpio7/value"



//双色指示灯 LEDB1- :PB12  LEDB2- :PB14
#define SYSFS_GPIO_PIN_PB12 		"44"
#define SYSFS_GPIO_PIN_PB14  		"46"
#define SYSFS_GPIO_PIN_PB12_DIR      "/sys/class/gpio/gpio44/direction"
#define SYSFS_GPIO_PIN_PB14_DIR      "/sys/class/gpio/gpio46/direction"
#define SYSFS_GPIO_PIN_PB12_VAL      "/sys/class/gpio/gpio44/value"
#define SYSFS_GPIO_PIN_PB14_VAL      "/sys/class/gpio/gpio46/value"




//大灯橙色: PB11 白色: PB15
#define SYSFS_GPIO_PIN_PB11			 "43"
#define SYSFS_GPIO_PIN_PB15 		 "47"
#define SYSFS_GPIO_PIN_PB11_DIR      "/sys/class/gpio/gpio43/direction"
#define SYSFS_GPIO_PIN_PB15_DIR      "/sys/class/gpio/gpio47/direction"
#define SYSFS_GPIO_PIN_PB11_VAL      "/sys/class/gpio/gpio43/value"
#define SYSFS_GPIO_PIN_PB15_VAL      "/sys/class/gpio/gpio47/value"


int echo_led_export(char *pin,char *pinDir)
{
 	if(access(pinDir,F_OK) == 0)
		return 0;
	
	/* O_WRONLY O_RDONLY  O_RDWR
	 * echo 91 > export
	 */
	int fd;
	fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	if(fd < 0){
		printf("echo_led_export:open export error\n");
		return -1;	
	}

	write(fd, pin,sizeof(pin));
	close(fd);

	/*
	 *  /sys/class/gpio/gpio91# echo out > direction
	 */
	fd = open(pinDir, O_WRONLY);
	if(fd < 0){
		printf("echo_led_export:busy pin direction open error\n");
		return -2;	
	}	

	write(fd, SYSFS_GPIO_DIR_OUT, sizeof(SYSFS_GPIO_DIR_OUT));
	close(fd);
	
    return 0;
}

int bngo_led_open(char *pin)
{
	int fd;
	if(strcmp(pin,"PA10") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA10,SYSFS_GPIO_PIN_PA10_DIR);
		fd = open(SYSFS_GPIO_PIN_PA10_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA7") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA7,SYSFS_GPIO_PIN_PA7_DIR);
		fd = open(SYSFS_GPIO_PIN_PA7_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB12") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB12,SYSFS_GPIO_PIN_PB12_DIR);
		fd = open(SYSFS_GPIO_PIN_PB12_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB14") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB14,SYSFS_GPIO_PIN_PB14_DIR);
		fd = open(SYSFS_GPIO_PIN_PB14_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB11") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB11,SYSFS_GPIO_PIN_PB11_DIR);
		fd = open(SYSFS_GPIO_PIN_PB11_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB15") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB15,SYSFS_GPIO_PIN_PB15_DIR);
		fd = open(SYSFS_GPIO_PIN_PB15_VAL, O_WRONLY);
	} else {
		   return -1;
	}

	if(fd < 0) {
		printf("bngo_led_open: open export error.\n");
		return -1;
	}
	
	write(fd, SYSFS_GPIO_PIN_VAL_L,sizeof(SYSFS_GPIO_PIN_VAL_L));
	close(fd);	
	return 0;
}

int bngo_led_close(char *pin)
{
	int fd;
	if(strcmp(pin,"PA10") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA10,SYSFS_GPIO_PIN_PA10_DIR);
		fd = open(SYSFS_GPIO_PIN_PA10_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA7") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA7,SYSFS_GPIO_PIN_PA7_DIR);
		fd = open(SYSFS_GPIO_PIN_PA7_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB12") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB12,SYSFS_GPIO_PIN_PB12_DIR);
		fd = open(SYSFS_GPIO_PIN_PB12_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB14") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB14,SYSFS_GPIO_PIN_PB14_DIR);
		fd = open(SYSFS_GPIO_PIN_PB14_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB11") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB11,SYSFS_GPIO_PIN_PB11_DIR);
		fd = open(SYSFS_GPIO_PIN_PB11_VAL, O_WRONLY);
	} else if(strcmp(pin,"PB15") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PB15,SYSFS_GPIO_PIN_PB15_DIR);
		fd = open(SYSFS_GPIO_PIN_PB15_VAL, O_WRONLY);
	} else {
		return -1;
	}

	if(fd < 0) {
		printf("bngo_led_close: open export error.\n");
		return -1;
	}
	write(fd, SYSFS_GPIO_PIN_VAL_H,sizeof(SYSFS_GPIO_PIN_VAL_H));
	close(fd);

	return 0;
}




