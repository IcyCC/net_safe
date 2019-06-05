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

	#define SHA1HashSize 20	//����λ���ֽڣ���160bit��ժҪ����

	/*
	* structure holds context information for the SHA-1 hashing operation
	* �ܶ�ط����ֽ�(8bit)Ϊ��λ������������λ��������Ҫд��ô���
	* ��ϵͳ�ֳ�Ϊ32λ��һ��ժҪ����5���֣�һ�������16����
	*/
	typedef struct SHA1Context
	{
		uint32_t Intermediate_Hash[SHA1HashSize / 4]; /* 5��32λ�ļĴ�����������ɵ�ժҪ*/
		uint32_t Length_Low;											 /* ������32λ�ı����泤�� */
		uint32_t Length_High;
	
		int_least16_t Message_Block_Index;
		uint8_t Message_Block[64];									/* 512bit�Ŀ飬���ֽ�Ϊ��λ������64*8��=512-bit  */
		int Computed;														/* �Ƿ��������ļ��� */
		int Corrupted;														/* �����־ */
	} SHA1Context;

	int SHA1Reset(SHA1Context *);
	int SHA1Input(SHA1Context *, const uint8_t *, unsigned int);
	int SHA1Result(SHA1Context *, uint8_t Message_Digest[SHA1HashSize]);
	void SHA1PadMessage(SHA1Context *);
	void SHA1ProcessMessageBlock(SHA1Context *);

#endif

