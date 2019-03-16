#include <stdio.h>
#include <openssl/md5.h> 
#include <string.h>  
#include <stdlib.h> 
#include <time.h>
#include "https_interface.h"


//取字符串下标1,3,7,15,31的字符，拼起来
int hex_to_String(const uint8 *Src,char *Dest, int len)
{
	uint8 ddl,ddh;
	char *yaokan_str = Dest;
	int i;
	for(i = 0;i<len;i++){
		ddh = 48 + Src[i]/16;
		ddl = 48 + Src[i]%16;
		if(ddh > 57)
			ddh += 7;
		if(ddl > 57)
			ddl += 7;
		Dest[i*2]   = tolower(ddh);
		Dest[i*2+1] = tolower(ddl);
	}
	
	//获取1、3、7、15、31下标字符
	for(i = 0;i<32;i++)
	{
		if(i==1 || i==3 || i == 7 || i==15 || i==31){
			*yaokan_str = Dest[i];
			yaokan_str++;
		}
	}
	*yaokan_str = '\0';
	
	return 0;
}

void headers_client(char *md5,char *Src)
{

    MD5_CTX Md5Ctx;  
    unsigned char md5_pw[16] = {0}; 
	char dst[50] = {0};   
	char h[20] = {0};  //harders clent
	
	memset(md5_pw,0,16);
	
	//获取时间戳
    time_t t;  
    t = time(NULL);    
    int ii = time(&t);
	ii /= 100;
	sprintf(h,"%d",ii);

	strcat(md5,h);	
	
    MD5_Init(&Md5Ctx);  //初始化  
    MD5_Update(&Md5Ctx, (unsigned char *)md5, strlen(md5)); //md5加密  
    MD5_Final(md5_pw, &Md5Ctx); //将加密后的密文放到md5_pw

	/*	
	int m = 0;
	for(m = 0;m<16;m++){
		printf("%x",md5_pw[m]);		
	}
	putchar('\n');
	*/
	
	hex_to_String(md5_pw,dst,16);     //提取下标 dst
	strcat(h,"_");
	strcat(h,dst);
	strcpy(Src,h);
}