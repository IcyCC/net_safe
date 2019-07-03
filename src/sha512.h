#include <stdint.h>
#include <string.h>

#define shaSuccess 0
#define shaNull 1
#define shaStateError 2

#define SHA512HashSize 64	//（单位：字节）即512bit的摘要长度

/*
* 该系统字长为64位，一个摘要包括8个字，一个块包括16个字
*/
typedef struct SHA512Context
{
	uint64_t Intermediate_Hash[SHA512HashSize/8]; /* 8个64位的寄存器，存放生成的摘要*/
	uint64_t Length_Low;				/* 用两个64位的变量存长度 */
	uint64_t Length_High;

	int_least16_t Message_Block_Index;
	uint8_t Message_Block[128];				/* 1024bit的块 */
	int Computed;							/* 是否处理完最后的计算 */
	int Corrupted;							/* 溢出标志 */
}SHA512Context;

int SHA512Reset(SHA512Context *);
int SHA512Input(SHA512Context *, const uint8_t *, unsigned int);
int SHA512Result(SHA512Context *, uint8_t Message_Digest[SHA512HashSize]);
void SHA512PadMessage(SHA512Context *);
void SHA512ProcessMessageBlock(SHA512Context *);
int getHash(const char *,uint8_t *)

