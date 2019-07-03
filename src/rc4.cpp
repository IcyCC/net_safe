#include "rc4.h"
#include <iostream>
using namespace std;

RC4::RC4()
{
    this->k.clear();
    this->miwen=NULL;
}

/* 
初始化算法 
接受密钥和密钥长度
生成S盒
*/
void RC4::rc4Init(char *key, long Len)
{
    int i;
    this->k.clear();
    this->miwen=NULL;
    if (key == NULL || Len == 0)
    {
        cout<<"rc4 key or len is 0, return!\n";
        return ;
    }
    
    /* 将0到255的互不重复的元素装入S盒 */
    for (i = 0; i < 256 ; i++) {
        S[i] = i;
        T[i]=key[i%Len];
    }
    
    /* 根据密钥打乱S盒 */
    int j = 0;
    char tmp;
    for (i = 0; i < 256; i++)
    {
        j = ( j + S[i] + key[i % Len] ) % 256;
        /* 交换S[i]和S[j] */
        tmp = S[i];
        S[i] = S[j]; 
        S[j] = tmp;
    }
}

/* 
生成密钥流
 */
void RC4::rc4KeyStream(long len) 
{
    int j=0;      
    for(int i=0;i<256;++i){            
        j=(j+S[i]+T[i])%256;       
        S[i]=S[i]+S[j];       
        S[j]=S[i]-S[j];        
        S[i]=S[i]-S[j];  
    }     
    int i=0,t;
    j=0;    
    while(len--){        
        i=(i+1)%256;        
        j=(j+S[i])%256;         
        S[i]=S[i]+S[j];        
        S[j]=S[i]-S[j];        
        S[i]=S[i]-S[j];         
        t=(S[i]+S[j])%256;        
        this->k.push_back(S[t]);    
    }
}

/* 
 用密钥流和密文进行解密
*/
void RC4::rc4DeCrypt(char *Secret, long Len)
{
    cout<<"解密结果："<<endl;
    int i;
    for(i=0;i<Len;++i){
        cout<<(unsigned char)(Secret[i]^this->k[i]);   
    }
    cout<<endl;
}

/* 
 加密
*/
void RC4::rc4EnCrypt(char *Data, long Len)
{
    this->miwen=new char[Len+1];
    /*生成密钥流*/
    rc4KeyStream(Len);
    cout<<endl<<"加密结果："<<endl;
    int i;
    for(i=0;i<Len;++i){
        cout<<(unsigned char)(this->k[i]^Data[i]);
        this->miwen[i]=(unsigned char)(this->k[i]^Data[i]);
    }
    this->miwen[i]='\0';
    cout<<endl; 
}


char * RC4::getMiwen()
{
    return this->miwen;
}
