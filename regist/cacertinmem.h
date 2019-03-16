#ifndef _CACERTINMEM_H_
#define _CACERTINMEM_H_

#define POSTSTR_SIZE 256
#define RESPONSE_SIZE 200*1024

//success return CURLE_OK
int http_get(const char *strGet,char *strResponse);

#endif //_CACERTINMEM_H_