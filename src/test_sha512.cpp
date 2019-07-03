
#include <stdint.h>
#include <iostream>
#include <string.h>
#include "sha512.h"
using namespace std;

int getHash(const char *input,uint8_t *res)
{
	SHA512Context sha;
	int i, j, err;
	
	char *raw=new char[strlen(input)];
    strcpy(raw , input);
	
	/* 开始测试 */
	err = SHA512Reset(&sha);
	err = SHA512Input(&sha,(const unsigned char *) raw,strlen(raw));
	if (err)
	{
		cout<<"SHA512Input Error "<<err<<".\n";
		return err; 
	}

	err = SHA512Result(&sha, res);
	if (err){
		cout<<"SHA512Result Error "<<err<<", could not compute message digest.\n";
		return err;
	}
	return 0;
}
int main()
{
	SHA512Context sha;
	int i, j, err;
	uint8_t Message_Digest[64];
	char a[100000];
	cin>>a;
	err=getHash(a,Message_Digest);
			
	// for (i = 0; i < 64; ++i)
	// {
	// 	printf("%02x", Message_Digest[i]);
	// }
	// cout<<endl;
	return 0;
}