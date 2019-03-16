/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
//#include <openssl/ssl.h>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include "cacertinmem.h"

//记录遥控服务器返回数据的大小，用于拼接
int shift = 0;


size_t writefunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
  //printf("buffer is %s\n",(char *)ptr);
 // fwrite(ptr, size, nmemb, stream);
	int page_size = size*nmemb;
  	memcpy(stream+shift,ptr,size*nmemb);
  	shift += page_size;
  	//printf("nmemb:%d .################################################# \n",size*nmemb);
	return (nmemb*size);
}
/*  注册、匹配使用的接口
 *
 */
int http_get(const char *strGet,char *strResponse)
{
  shift = 0;

  CURL *ch;
  CURLcode rv;

  ch=curl_easy_init();
  
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, *writefunction);
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, strResponse);

  curl_easy_setopt(ch, CURLOPT_URL, strGet);
  //30秒超时
  curl_easy_setopt(ch, CURLOPT_TIMEOUT, 30L);

  /* first try: retrieve page without cacerts' certificate -> will fail
   */  
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
    ;//printf("\n*** transfer succeeded ***\n");
  else
    printf("\n***errno:[%d]  transfer failed ***\n",rv);

  curl_easy_cleanup(ch);
  return rv;
}