#include "city_sohu.h"

int main(int argc,char *argv[])
{
	char rsp[5120] = {0};
	sohu_get_city(rsp);
	
	
	return 0;
}