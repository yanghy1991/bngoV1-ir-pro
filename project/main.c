#include <string.h>
#include <stdio.h>
#include "https_interface.h"

int main(int argc,char *argv[])
{
	//char src[256] = "noneHyyThvYgCdvUkZV0SAETZQ==007566E38531ACD9C643219515099596";
	char h_clent[20] = {0};
	char src[256] = "none007566E38531ACD9C6432195";
	headers_client(src,h_clent);
	printf("str = %s\n", h_clent);	
	
	
	return 0;
}