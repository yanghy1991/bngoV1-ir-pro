/*
 * =====================================================================================
 *       Filename:  url_encode.cpp
 *    Description:  url encode
 *
 *        Created:  07/04/2012 08:32:24 PM
 *
 *         Author:  zhy (), izualzhy@163.com
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>

/**
 * @brief URLEncode 对字符串URL编码
 *
 * @param str 原字符串
 * @param strSize 原字符串长度(不包括最后的\0)
 * @param result 结果缓冲区的地址
 * @param resultSize 结果缓冲区的大小(包括最后的\0)
 *
 * @return: >0:resultstring 里实际有效的长度
 *            0: 解码失败.
 */
int URLEncode(const char* str, const int strSize, char* result, const int resultSize)
{
    int i;
    int j = 0;//for result index
    char ch;

    if ((str==NULL) || (result==NULL) || (strSize<=0) || (resultSize<=0)) {
        return 0;
    }

    for ( i=0; (i<strSize)&&(j<resultSize); ++i) {
        ch = str[i];
        if (((ch>='A') && (ch<'Z')) ||
            ((ch>='a') && (ch<'z')) ||
            ((ch>='0') && (ch<'9'))) {
            result[j++] = ch;
        } else if (ch == ' ') {
            result[j++] = '+';
        } else if (ch == '.' || ch == '-' || ch == '_' || ch == '*') {
            result[j++] = ch;
        } else {
            if (j+3 < resultSize) {
                sprintf(result+j, "%%%02X", (unsigned char)ch);
                j += 3;
            } else {
                return 0;
            }
        }
    }

    result[j] = '\0';
    return j;
}
/*
int main(int argc, char* argv[])
{
    char* src = argv[1];
    unsigned int srclength = strlen(src);
    printf("src length: %d\n", strlen(src));
    

    char obj[100] = {0};
    URLEncode(src, srclength, obj, 100);

    printf("obj: %s\n", obj);
    printf("size: %d\n", strlen(obj));

    return 0;
}
*/