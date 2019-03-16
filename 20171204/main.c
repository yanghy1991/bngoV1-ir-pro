#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#include "cacertinmem.h"

int bngo_ir(char *command)
{
	//设置为实时进程
	struct sched_param param; 
	int maxpri; 
	maxpri = sched_get_priority_max(SCHED_FIFO);
	if(maxpri == -1) 
	{ 
		perror("sched_get_priority_max() failed"); 
		exit(1); 
	} 
		param.sched_priority = maxpri; 
	if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) //设置优先级
	{ 
		perror("sched_setscheduler() failed"); 
		exit(1); 
	} 


	//first
	if(get_secret_key() != 0)
		return SECRET_KEY_ERR;

	//second
	//if(get_Activation_code() != 0)
		//return ACTIVATION_CODE_ERR;

	//three 电器类型
	//if(get_Electrical_list() != 0)
		//return ELECTRICAL_LIST_ERR;

	//four 获取品牌
	//if(get_Brand_list("15") != 0)
		//return BRAND_LIST_ERR;

	#if 0
	//five
	if(get_Match_code("104","7","4") != 0)
		return MATCH_GCODE_ERR;
		
	if(send_Match_code(7,0,"on") !=0)
		return MATCH_SCODE_ERR;

	if(get_Complete_code("7") != 0)
		return COMPLETE_GCODE_ERR;

	#endif
	
	//if(send_Complete_code("\u683c\u529b0","r_s0_26_u0_l0_p0") != 0)
	if(send_Complete_code("\u683c\u529b0","off") != 0)
		return COMPLETE_SCODE_ERR;
	
	return 0;
}
