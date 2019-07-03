#include "rc4.h"
#include <iostream>
#include <string.h>
using namespace std;

int main()
{
    RC4 rc4;
    char key[256];
    char data[1000000];
    cout<<"输入密钥：";
    cin>>key;
    cout<<"输入明文：";
    cin>>data;
    rc4.rc4Init(key,strlen(key));/* 用密钥初始化S盒 */
    rc4.rc4EnCrypt(data,strlen(data));
    char *m=rc4.getMiwen();
    if(m!=NULL)
        rc4.rc4DeCrypt(m,strlen(m));
    else
        cout<<"还未生成密文，无法解密\n";
    return 0;
}