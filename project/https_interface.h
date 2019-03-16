#ifndef _HTTPS_INTERFACE_H_
#define _HTTPS_INTERFACE_H_
#include <ctype.h>

typedef unsigned char    uint8;

void headers_client(char *md5,char *Src);
int hex_to_String(const uint8 *Src,char *Dest, int len);

#endif //_HTTPS_INTERFACE_H_