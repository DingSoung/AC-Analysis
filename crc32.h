#ifndef CRC32_H
#define CRC32_H

/*
endian = 
0: msb,	RefIn = TRUE,	RefOut = TRUE
1: lsb,	RefIn = FALSE,	RefOut = FALSE
*/

/* CRC-32*/
#define Crc32_Init	0xFFFFFFFF
#define Crc32_XorOut	0xFFFFFFFF
#define endian32	0

/* CRC-32 MPEG2
#define Crc32_Init	0xFFFFFFFF
#define Crc32_XorOut	0x00000000
#define endian32	1
*/


static unsigned long crc32table[256];

static void init_crc32_table(void) {
#if endian32
	unsigned int Crc32_Poly = 0x04C11DB7;
	for (unsigned int i = 0; i < 256; i++) {
		unsigned int crc = i << 24;
		for (unsigned char j = 0; j < 8; j++)
			crc = (crc << 1) ^ ((crc & 0x80000000) ? Crc32_Poly : 0);
		crc32table[i] = crc & 0xFFFFFFFF;
	}
#else
	unsigned int Crc32_Poly = 0xEDB88320;
	for (unsigned int i = 0; i < 256; i++) {
		unsigned int crc = i;
		for (unsigned char j = 0; j < 8; j++)
			crc = (crc >> 1) ^ ((crc & 0x00000001) ? Crc32_Poly : 0);
		crc32table[i] = crc & 0xFFFFFFFF;
	}
#endif
}
static void crc32Init(unsigned int *pCrc32) {
	*pCrc32 = Crc32_Init;
}
static void crc32Update(unsigned int *pCrc32, unsigned char *pData, unsigned int uSize) {
#if endian32
	for(unsigned int i = 0; i < uSize; i++)
		*pCrc32 = ((*pCrc32) << 8) ^ crc32table[(pData[i] ^ (*pCrc32 >>24)) & 0xFF];
#else
	for(unsigned int i = 0; i < uSize; i++)
		*pCrc32 = ((*pCrc32) >> 8) ^ crc32table[(pData[i] ^ *pCrc32) & 0xFF];
#endif
}
static void crc32Finish(unsigned int *pCrc32) {
#if endian32
#else
	*pCrc32 ^= Crc32_XorOut;
#endif
}

#endif

//ex:
//	void main(){
//		unsigned int CRC32BIN;
//		init_crc_table();
//		crc32Init(&CRC32BIN);
//		while( ... ) {
//			crc32Update(&CRC32BIN, pBuffer, Length + 1);
//		}
//		crc32Finish(&CRC32BIN);
//	}       

