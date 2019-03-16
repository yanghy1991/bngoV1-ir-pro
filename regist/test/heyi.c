#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#include "cacertinmem.h"

int main(int argc,char *argv[])
{
	char strResponse[5120] = {0};
	
	http_get("http://192.168.1.196:8080/ota/canna_v1.0_ap6181/v2.2.2/appfs",strResponse);
		
	printf("strResponse:%s \n",strResponse);

	return 0;
}
