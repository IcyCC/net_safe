#include<vector>
class RC4{
public:
    RC4();  
    void rc4Init(char *key, long Len);
    void rc4KeyStream(long len);
    void rc4DeCrypt( char *Secret, long Len);/* 加/解密 */
    void rc4EnCrypt(char *Data, long Len);
    char *getMiwen();
    
private:
    char S[256]; //状态向量   
    char T[256]; //临时向量   
    std::vector<char> k;       //密钥流
    char *miwen;        //加密后的密文
};