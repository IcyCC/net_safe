
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sha512.h"

#define TEST1 "abcd"
#define TEST2a "abcdbcdecdefdefgefghfghighijhi"
#define TEST2b "jkijkljklmklmnlmnomnopnopq"

#define TEST2 TEST2a TEST2b

#define TEST3a "01234567012345670123456701234567"
#define TEST3b "01234567012345670123456701234567"

/* TEST3:512 bits */
#define TEST3 TEST3a TEST3b


char *testarray[4] =
{
	TEST1,
	TEST2,
	TEST3
};

/* 应该得到的正确结果，用来比对 */
char *resultarray[3] =
{
	"d8022f2060ad6efd297ab73dcc5355c9b214054b0d1776a136a669d26a7d3b14f73aa0d0ebff19ee333368f0164b6419a96da49e3e481753e7e96b716bdccb6f",
	"204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445",
	"846e0ef73436438a4acb0ba7078cfe381f10a0f5edebcb985b3790086ef5e7ac5992ac9c23c77761c764bb3b1c25702d06b99955eb197d45b82fb3d124699d78"
};

int main()
{
	SHA512Context sha;
	int i, j, err;
	uint8_t Message_Digest[64];
	
	/* 开始测试 */
	for (j = 0; j < 3; ++j)
	{
		printf( "Test %d:\n",j+1);
		err = SHA512Reset(&sha);
		//if (err)
		//{
			//fprintf(stderr, "SHA512Reset Error %d.\n", err );
			//break; 
		//}
		
		err = SHA512Input(&sha,(const unsigned char *) testarray[j],strlen(testarray[j]));
		if (err)
		{
			fprintf(stderr, "SHA512Input Error %d.\n", err);
			break; 
		}

		err = SHA512Result(&sha, Message_Digest);
		if (err)
			fprintf(stderr,
				"SHA512Result Error %d, could not compute message digest.\n",
				err);
		else
		{
			for (i = 0; i < 64; ++i)
			{
				printf("%02x", Message_Digest[i]);
			}
			printf("\n");
		}
		printf("Should match:\n");
		printf("%s\n\n", resultarray[j]);
	}
	return 0;
}