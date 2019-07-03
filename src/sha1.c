#include "sha1.h"

#define SHA1CircularShift(bits,word)  (((word) << (bits)) | ((word) >> (32 - (bits))))

/* 初始化context结构体 */
int SHA1Reset(SHA1Context *context)
{
	if (!context)
	{
		return shaNull;
	}
	context->Length_Low = 0;
	context->Length_High = 0;
	context->Message_Block_Index = 0;
	context->Intermediate_Hash[0] = 0x67452301;//初始化5个32位的寄存器
	context->Intermediate_Hash[1] = 0xEFCDAB89;
	context->Intermediate_Hash[2] = 0x98BADCFE;
	context->Intermediate_Hash[3] = 0x10325476;
	context->Intermediate_Hash[4] = 0xC3D2E1F0;
	context->Computed = 0;
	context->Corrupted = 0;
	return shaSuccess;
} 

/*
* SHA1Result
* 生成160bit的摘要，放到参数里给出的Message_Digest数组中
*/
int SHA1Result(SHA1Context *context, uint8_t Message_Digest[SHA1HashSize])
{
	int i;
	if (!context || !Message_Digest)
		return shaNull;
	if (context->Corrupted)
		return context->Corrupted;
	if (!context->Computed){
		SHA1PadMessage(context); /* 处理完最后的块*/
		/*
		for (i = 0; i < 64; ++i)
			context->Message_Block[i] = 0;	
		context->Length_Low = 0;
		context->Length_High = 0;
		*/
		context->Computed = 1;
	}

	for (i = 0; i < SHA1HashSize; ++i){
		Message_Digest[i] = 
			context->Intermediate_Hash[i >> 2]>> 8 * (3 - (i & 0x03));
	}
	return shaSuccess;
}

/*
* SHA1Input
*/
int SHA1Input(SHA1Context *context, const uint8_t *message_array, unsigned length)
{
	if (!length)
		return shaSuccess;
	if (!context || !message_array)
		return shaNull;
	if (context->Computed){
		context->Corrupted = shaStateError;
		return shaStateError;
	}
	if (context->Corrupted)
		return context->Corrupted;

	while (length-- && !context->Corrupted)
	{
		context->Message_Block[context->Message_Block_Index++] =
					(*message_array & 0xFF);	//？？最后的没有数据的位补满0ma
		context->Length_Low += 8;
		if (context->Length_Low == 0){ /* 低位的满了，存到高位的变量里 */
			context->Length_High++;
			if (context->Length_High == 0)		/* 高位的变量也满了 */
				context->Corrupted = 1;
		}
		if (context->Message_Block_Index == 64)
			SHA1ProcessMessageBlock(context);		/* 每满一个块的长度就处理一个块 */
		message_array++; /* 取得下一个8位 */
	}
	return shaSuccess;
}

 

/*
* SHA1ProcessMessageBlock
* 以512位的分组处理消息
* 过程中只改变context结构体里5个寄存器的值，最后将Message_Block_Index置0
*/
void SHA1ProcessMessageBlock(SHA1Context *context)
{
	const uint32_t K[] = { /* Constants defined in SHA-1 */
		0x5A827999,
		0x6ED9EBA1,
		0x8F1BBCDC,
		0xCA62C1D6
	};
	int t; /* Loop counter */
	uint32_t temp; /* Temporary word value */
	uint32_t W[80]; /* 存放扩展后的80个字 */
	uint32_t A, B, C, D, E; /* 5个寄存器 */
		
	/*
	* 消息扩展算法，16个Wt扩展到80个Wt
	* 前16个Wt直接取自当前MessageBlock的16个字
	* 后面的用SHA1CircularShift处理
	*/
	for (t = 0; t < 16; t++)
	{
		W[t] = context->Message_Block[t * 4] << 24;
		W[t] |= context->Message_Block[t * 4 + 1] << 16;
		W[t] |= context->Message_Block[t * 4 + 2] << 8;
		W[t] |= context->Message_Block[t * 4 + 3];
	}

	for (t = 16; t < 80; t++)
		W[t] = SHA1CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
		
	/*
	* 轮函数  以初始化好的5个寄存器值为起点，进行轮函数运算
	* SHA1有4轮运算，每一轮包括20个步骤
	*/
		
		/* 
		*  另申请5个变量进行运算，
		*  将原来5个Intermediate_Hash保留，用于最后与A，B，C，D，E进行求和操作
		*/
	A = context->Intermediate_Hash[0];
	B = context->Intermediate_Hash[1];
	C = context->Intermediate_Hash[2];
	D = context->Intermediate_Hash[3];
	E = context->Intermediate_Hash[4];

	for (t = 0; t < 20; t++)
	{
		temp = SHA1CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}
	for (t = 20; t < 40; t++)
	{
		temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 40; t < 60; t++)
	{
		temp = SHA1CircularShift(5, A) +
					((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 60; t < 80; t++)
	{
		temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}
	context->Intermediate_Hash[0] += A;
	context->Intermediate_Hash[1] += B;
	context->Intermediate_Hash[2] += C;
	context->Intermediate_Hash[3] += D;
	context->Intermediate_Hash[4] += E;
	context->Message_Block_Index = 0;
}

/*
* SHA1PadMessage
* 对消息尾部的context的处理：附加若干bit填充位和64bit的长度指示位
* 最终变成一个总长度为512的Message_Block后再process
*/
void SHA1PadMessage(SHA1Context *context)
{
	/* 
	* 后面已经不够8个字节的长度来存length，
	* 先填充并处理完当前的块，再开另一个块
	*/
	if (context->Message_Block_Index > 55)
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;	/*1000'0000*/
		while (context->Message_Block_Index < 64)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
		SHA1ProcessMessageBlock(context);		/*每处理完一次,index重新变为0*/
		while (context->Message_Block_Index < 56)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}
	else
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;
		while (context->Message_Block_Index < 56)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}
	/*
	* 留最后8个字节（u_int8t）（64位）存消息长度
	*/
	context->Message_Block[56] = context->Length_High >> 24;
	context->Message_Block[57] = context->Length_High >> 16;
	context->Message_Block[58] = context->Length_High >> 8;
	context->Message_Block[59] = context->Length_High;
	context->Message_Block[60] = context->Length_Low >> 24;
	context->Message_Block[61] = context->Length_Low >> 16;
	context->Message_Block[62] = context->Length_Low >> 8;
	context->Message_Block[63] = context->Length_Low;
	SHA1ProcessMessageBlock(context);
}


