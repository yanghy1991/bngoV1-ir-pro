#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#include "cacertinmem.h"

int main(int argc,char *argv[])
{

	//second
	//if(get_Activation_code() != 0)
		//return ACTIVATION_CODE_ERR;

	//three 获取电器类型
	//if(get_Electrical_list() != 0)
		//return ELECTRICAL_LIST_ERR;

	//four 获取品牌
	//if(get_Brand_list("7",177) != 0)
			//return BRAND_LIST_ERR;
	
	#if 0
	if(get_Match_code("479","2","4") != 0)
		printf("get_Match_code error!\n");
	#endif 
	
	#if 0
	if(get_Complete_code("2") != 0)
		printf("get_Complete_code error!\n");
	#endif 
	
	#if 0
	int count = 0;
	int ret = 0;
	char t[5] = "7";
	int n = 209;
	
	int i;
	for(i = 0;i<n;i++){
		ret = get_Brand_list(t,i);
		if(ret == -2){
			count++;
		} else if(ret != 0){
			return BRAND_LIST_ERR;		
		}
		
	}
	printf("count:%d  \n",count);
	#endif
	
	//获取码库品牌 型号信息
	get_all_key("10");
	

	return 0;
}
