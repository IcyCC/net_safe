#ifndef _SHA1_H_
	#define _SHA1_H_
	#include <stdint.h>

	#ifndef _SHA_enum_
		#define _SHA_enum_
		enum
		{
			shaSuccess      = 0,
			shaNull,         /* Null pointer parameter */
			shaInputTooLong, /* input data too long */
			shaStateError    /* called Input after Result */
		};
	#endif

	#define SHA1HashSize 20	//（单位：字节）即160bit的摘要长度

	/*
	* structure holds context information for the SHA-1 hashing operation
	* 很多地方以字节(8bit)为单位操作，这样移位操作不需要写那么多次
	* 该系统字长为32位，一个摘要包括5个字，一个块包括16个字
	*/
	typedef struct SHA1Context
	{
		uint32_t Intermediate_Hash[SHA1HashSize / 4]; /* 5个32位的寄存器，存放生成的摘要*/
		uint32_t Length_Low;											 /* 用两个32位的变量存长度 */
		uint32_t Length_High;
	
		int_least16_t Message_Block_Index;
		uint8_t Message_Block[64];									/* 512bit的块，以字节为单位操作（64*8）=512-bit  */
		int Computed;														/* 是否处理完最后的计算 */
		int Corrupted;														/* 溢出标志 */
	} SHA1Context;

	int SHA1Reset(SHA1Context *);
	int SHA1Input(SHA1Context *, const uint8_t *, unsigned int);
	int SHA1Result(SHA1Context *, uint8_t Message_Digest[SHA1HashSize]);
	void SHA1PadMessage(SHA1Context *);
	void SHA1ProcessMessageBlock(SHA1Context *);

#endif

