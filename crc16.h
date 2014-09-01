#ifndef CRC16_H
#define CRC16_H

/*
endian = 
0: msb,	RefIn = TRUE,	RefOut = TRUE
1: lsb,	RefIn = FALSE,	RefOut = FALSE
*/

/* CRC-16-CCITT */
#define Crc16_Init	0x0000
#define Crc16_XorOut	0x0000
#define endian16	0

/* CRC-16-CCITT Xmodem
#define Crc16_Init	0x0000
#define Crc16_XorOut	0x0000
#define endian16	1
*/
/* CRC-16 X25 
#define Crc16_Init	0xFFFF
#define Crc16_XorOut	0xFFFF
#define endian16	0
*/

static unsigned short crc16table[256];
static void init_crc16_table(void) {
#if endian16
	unsigned short Crc16_Poly = 0x1021;
	for (unsigned short i = 0; i < 256; i++ ) {
		unsigned short crc = i << 8;
		for ( unsigned char j = 0; j < 8; j++ )
			crc = (crc << 1) ^ ((crc & 0x8000) ? Crc16_Poly : 0);	
		crc16table[i] = crc & 0xFFFF;
	}
#else
	unsigned short Crc16_Poly = 0x8408;
	for (unsigned short i = 0; i < 256; i++ ) {
		unsigned short crc = i;
		for ( unsigned char j = 0; j < 8; j++ )
			crc = (crc >> 1) ^ ((crc & 0x0001) ? Crc16_Poly : 0);	
		crc16table[i] = crc & 0xFFFF;
	}
#endif
}
static void crc16Init(unsigned short *pCrc16) {
	*pCrc16 = Crc16_Init;
}
static void crc16Update(unsigned short *pCrc16, unsigned char *ptr, unsigned int len) {
#if endian16
	for(unsigned int i = 0; i < len; i++)
		*pCrc16 = ((*pCrc16) << 8) ^ crc16table[(ptr[i] ^ (*pCrc16 >> 8)) & 0xFF];
#else
	for(unsigned int i = 0; i < len; i++)
		*pCrc16 = ((*pCrc16) >> 8) ^ crc16table[(ptr[i] ^ *pCrc16) & 0xFF];
#endif
}
static void crc16Finish(unsigned short *pCrc16) {
	*pCrc16 ^=  Crc16_XorOut;
}
#endif

//ex:
//int main() {
//	unsigned char dadaBuffer[] = "Hello CRC!";
//	unsigned short Crc16Result;
//	crc16Init(&Crc16Result);
//	crc16Update_by_bit(&Crc16Result, dadaBuffer, sizeof(dadaBuffer)-1);
//	crc16Finish(&Crc16Result);
//	return 0;
//}