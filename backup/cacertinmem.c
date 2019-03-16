/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.
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
 * CA cert in memory with OpenSSL to get a HTTPS page.
 * </DESC>
 */
//#include <openssl/ssl.h>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

size_t writefunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
  //printf("buffer is %s\n",(char *)ptr);
  fwrite(ptr, size, nmemb, stream);
  return (nmemb*size);
}

int main(void)
{
  CURL *ch;
  CURLcode rv;
  const char *data = "c=d&m=none&r=HyyThvYgCdvUkZV0SAETZQ==&appid=15095155717939&f=007566E38531ACD9C6432195";
  struct curl_slist *headers=NULL; /* init to NULL is important */
  headers = curl_slist_append(headers, "Accept-Language: zh");
  headers = curl_slist_append(headers, "client:15099596_d3e0b");

  rv=curl_global_init(CURL_GLOBAL_ALL);
  ch=curl_easy_init();
  rv=curl_easy_setopt(ch, CURLOPT_VERBOSE, 0L);
  rv=curl_easy_setopt(ch, CURLOPT_HEADER, 0L);
  rv=curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
  rv=curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1L);
  
  curl_easy_setopt(ch, CURLOPT_ACCEPT_ENCODING, "");   //自动解压gzip
  
  rv=curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, *writefunction);
  rv=curl_easy_setopt(ch, CURLOPT_WRITEDATA, stdout);
  rv=curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, *writefunction);
  rv=curl_easy_setopt(ch, CURLOPT_HEADERDATA, stderr);
  rv=curl_easy_setopt(ch, CURLOPT_SSLCERTTYPE, "PEM");
  rv=curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 1L);
  
  curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, strlen(data));		
  curl_easy_setopt(ch,CURLOPT_POSTFIELDS,data);	
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  
  //curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/ca-certificates.crt"); 
  curl_easy_setopt(ch,CURLOPT_CAINFO,"/usr/fs/usr/yuan/cacert.pem"); 
  rv=curl_easy_setopt(ch, CURLOPT_URL, "https://api.yaokongyun.cn/chip/m.php");

  /* first try: retrieve page without cacerts' certificate -> will fail
   */
  rv=curl_easy_perform(ch);
  if(rv==CURLE_OK)
    printf("\n*** transfer succeeded ***\n");
  else
    printf("\n***errno:[%d]  transfer failed ***\n",rv);
  
  curl_slist_free_all(headers); 
  curl_easy_cleanup(ch);
  curl_global_cleanup();
  return rv;
}
