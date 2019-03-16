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
#include "http_get.h"

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
  
  curl_global_init(CURL_GLOBAL_DEFAULT);
  ch=curl_easy_init();
  
  //////////////////////////////////////////////////////////////////////////
  curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(ch, CURLOPT_VERBOSE, 1L );
  curl_easy_setopt(ch, CURLOPT_AUTOREFERER, 1);
  curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 3L);
  curl_easy_setopt(ch, CURLOPT_CONNECTTIMEOUT, 10);
  curl_easy_setopt(ch, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
  curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1L);
  //请求完成之后，关闭改连接
  curl_easy_setopt(ch, CURLOPT_FORBID_REUSE, 1L);
  ///////////////////////////////////////////////////////////////
  
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, *writefunction);
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, strResponse);

  curl_easy_setopt(ch, CURLOPT_URL, strGet);
  
  //15秒超时  
  curl_easy_setopt(ch, CURLOPT_TIMEOUT, 10L);

  /* first try: retrieve page without cacerts' certificate -> will fail
   */  
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
    ;//printf("\n*** transfer succeeded ***\n");
  else
    printf("\n***errno:[%d]  transfer failed ***\n",rv);

  curl_easy_cleanup(ch);
  curl_global_cleanup();
  return rv;
}