#include "sha512.h"

#define Ch( x, y, z )     (z ^ (x & (y ^ z)))
#define Maj(x, y, z )     (((x | y) & z) | (x & y))
#define ROTR( value, bits ) (((value) >> (bits)) | ((value) << (64 - (bits))))
#define Roffset( x, n )         (((x)&0xFFFFFFFFFFFFFFFFULL)>>((uint64_t)n))
#define Sigma0( x )       (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define Sigma1( x )       (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define Gamma0( x )       (ROTR(x, 1) ^ ROTR(x, 8) ^ Roffset(x, 7))
#define Gamma1( x )       (ROTR(x, 19) ^ ROTR(x, 61) ^ Roffset(x, 6))


#define Sha512Round( a, b, c, d, e, f, g, h, i ) t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];t1 = Sigma0(a) + Maj(a, b, c);d += t0;h  = t0 + t1;


/* 初始化context结构体 */
int SHA512Reset(SHA512Context *context)
{
	if (!context) {
		return shaNull;
	}
	context->Length_Low = 0;
	context->Length_High = 0;
	context->Message_Block_Index = 0;
	context->Intermediate_Hash[0] = 0x6a09e667f3bcc908;//初始化8个64位的寄存器
	context->Intermediate_Hash[1] = 0xbb67ae8584caa73b;
	context->Intermediate_Hash[2] = 0x3c6ef372fe94f82b;
	context->Intermediate_Hash[3] = 0xa54ff53a5f1d36f1;
	context->Intermediate_Hash[4] = 0x510e527fade682d1;
	context->Intermediate_Hash[5] = 0x9b05688c2b3e6c1f;
	context->Intermediate_Hash[6] = 0x1f83d9abfb41bd6b;
	context->Intermediate_Hash[7] = 0x5be0cd19137e2179;
	context->Computed = 0;
	context->Corrupted = 0;
	return shaSuccess;
}

/*
* SHA512Result
* 生成512bit的摘要，放到参数里给出的Message_Digest数组中
*/
int SHA512Result(SHA512Context *context, uint8_t Message_Digest[SHA512HashSize])
{
	int i;
	if (!context || !Message_Digest)
		return shaNull;
	if (context->Corrupted)
		return context->Corrupted;
	if (!context->Computed) {
		SHA512PadMessage(context); /* 处理完最后的块*/

		for (i = 0; i < 64; ++i)
			context->Message_Block[i] = 0;
		context->Length_Low = 0;
		context->Length_High = 0;

		context->Computed = 1;
	}

	for (i = 0; i < SHA512HashSize; ++i) {
		Message_Digest[i] =
			context->Intermediate_Hash[i >> 3] >> 8 * (7 - (i % 8));
	}
	return shaSuccess;
}

/*
* SHA512Input
*/
int SHA512Input(SHA512Context *context, const uint8_t *message_array, unsigned length)
{
	if (!length)
		return shaSuccess;
	if (!context || !message_array)
		return shaNull;
	if (context->Computed) {
		context->Corrupted = shaStateError;
		return shaStateError;
	}
	if (context->Corrupted)
		return context->Corrupted;

	while (length-- && !context->Corrupted)
	{
		context->Message_Block[context->Message_Block_Index++] =
			(*message_array & 0xFF);	/* 最后的没有数据的位补满0ma */
		context->Length_Low += 8;
		if (context->Length_Low == 0) { /* 低位的满了，存到高位的变量里 */
			context->Length_High++;
			if (context->Length_High == 0)		/* 高位的变量也满了 */
				context->Corrupted = 1;
		}
		if (context->Message_Block_Index == 128)
			SHA512ProcessMessageBlock(context);/* 每满一个块的长度就处理一个块 */
		message_array++; /* 取得下一个8位 */
	}
	return shaSuccess;
}



/*
* SHA512ProcessMessageBlock
* 以1024位的分组处理消息
* 过程中只改变context结构体里8个寄存器的值，最后将Message_Block_Index置0
*/
void SHA512ProcessMessageBlock(SHA512Context *context)
{
	const uint64_t K[] = { /* 定义SHA-512常数 */
		0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
		0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
		0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
		0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
		0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
		0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
		0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
		0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
		0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
		0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
		0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
		0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
		0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
		0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
		0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
		0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
		0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
		0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
		0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
		0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
	};
	int t;
	uint64_t temp;
	uint64_t W[80]; /* 存放扩展后的80个字 */
	uint64_t A, B, C, D, E, F, G, H; /* 8个寄存器 */

									 /*
									 * 消息扩展算法，16个Wt扩展到80个Wt
									 * 前16个Wt直接取自当前MessageBlock的16个字
									 * 后面的另作处理
									 */
	int a;
	for (t = 0; t < 16; t++)
	{
		W[t] = ((context->Message_Block[t * 8]) | 0x0000000000000000ULL) << 56;
		for (a = 1; a<8; a++) {
			W[t] |= ((context->Message_Block[t * 8 + a]) | 0x0000000000000000ULL) << 56 - a * 8;
		}
	}

	for (t = 16; t < 80; t++)
	{
		W[t] = Gamma1(W[t - 2]) + W[t - 7] + Gamma0(W[t - 15]) + W[t - 16];
	}

	/*
	* 轮函数  以初始化好的8个寄存器值为起点，进行轮函数运算
	*/

	/*
	*  另申请8个变量进行运算，
	*  将原来8个Intermediate_Hash保留，用于最后与A，B，C，D，E, F, G, H进行求和操作
	*/
	A = context->Intermediate_Hash[0];	E = context->Intermediate_Hash[4];
	B = context->Intermediate_Hash[1];	F = context->Intermediate_Hash[5];
	C = context->Intermediate_Hash[2];	G = context->Intermediate_Hash[6];
	D = context->Intermediate_Hash[3];	H = context->Intermediate_Hash[7];

	int i;
	uint64_t t0, t1;
	for (i = 0; i < 80; i += 8)
	{
		Sha512Round(A, B, C, D, E, F, G, H, i + 0);
		Sha512Round(H, A, B, C, D, E, F, G, i + 1);
		Sha512Round(G, H, A, B, C, D, E, F, i + 2);
		Sha512Round(F, G, H, A, B, C, D, E, i + 3);
		Sha512Round(E, F, G, H, A, B, C, D, i + 4);
		Sha512Round(D, E, F, G, H, A, B, C, i + 5);
		Sha512Round(C, D, E, F, G, H, A, B, i + 6);
		Sha512Round(B, C, D, E, F, G, H, A, i + 7);
	}
	context->Intermediate_Hash[0] += A;	context->Intermediate_Hash[4] += E;
	context->Intermediate_Hash[1] += B; context->Intermediate_Hash[5] += F;
	context->Intermediate_Hash[2] += C; context->Intermediate_Hash[6] += G;
	context->Intermediate_Hash[3] += D; context->Intermediate_Hash[7] += H;

	context->Message_Block_Index = 0;
}

/*
* SHA512PadMessage
* 对消息尾部的context的处理：附加若干bit填充位和128bit的长度指示位
* 最终变成一个总长度为1024的Message_Block后再process
*/
void SHA512PadMessage(SHA512Context *context)
{
	/*
	* 如果后面已经不够16个字节的长度来存length，
	* 先填充并处理完当前的块，再开另一个块
	*/
	if (context->Message_Block_Index > 111)
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;	/*BIN：10000000*/
		while (context->Message_Block_Index < 128)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
		SHA512ProcessMessageBlock(context);		/*每处理完一次,index重新变为0*/
		while (context->Message_Block_Index < 112)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}
	else
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;
		while (context->Message_Block_Index < 120)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}
	/*
	* 留最后16个字节（u_int8t）（128位）存消息长度
	*/
	int i;
	for (i = 0; i<8; i++) {
		context->Message_Block[120 + i] = context->Length_Low >> 56 - i * 8;
		context->Message_Block[112 + i] = context->Length_High >> 56 - i * 8;
	}
	SHA512ProcessMessageBlock(context);
}

std::string CharToString(unsigned char *p)
{
    std::string str="";
    int len=strlen((char*)p);
    for(int i=0;i<len;++i){
        str+=p[i];
    } 
    return str;
}

std::string getHash(const char *input)
{
	uint8_t res[64];
	bzero(res, 64);
	std::string hash;
	SHA512Context sha;
	int i, j, err=0;
	
	char *raw=new char[strlen(input)];
    strcpy(raw , input);
	
	/* 开始测试 */
	err = SHA512Reset(&sha);
	err = SHA512Input(&sha,(const unsigned char *) raw,strlen(raw));
	err = SHA512Result(&sha, res);
	
	hash=CharToString(res);
	return hash.substr(0,64);
}
