#ifndef CRC8_H
#define CRC8_H

/* CRC-8-CCITT */
#define Crc8_Poly	0x07
#define Crc8_Init	0x00
#define Crc8_XorOut	0x00

static unsigned char crc8table[256];

static void init_crc8_table() {
	for (int i=0; i<256; i++) {
		unsigned char crc = i;
		for (int j=0; j<8; j++)
			crc = (crc << 1) ^ ((crc & 0x80) ? Crc8_Poly : 0);
		crc8table[i] = crc & 0xFF;
	}
}
static void crc8Init(unsigned char *pCrc8) {
	*pCrc8 = Crc8_Init;
}
static void crc8Update(unsigned char *pCrc8, unsigned char *pData, unsigned int uSize){
	for (unsigned int i = 0; i < uSize; i++) {
		*pCrc8 = crc8table[*pCrc8 ^ *pData++];
	}
}
static void crc8Finish(unsigned char *pCrc8) {
	*pCrc8 = *pCrc8 ^ Crc8_XorOut;
}

#endif

//ex:
//int main() {
//	unsigned char dadaBuffer[] = "Hello CRC!";
//	unsigned char Crc8Result;
//	init_crc8_table();
//	crc8Init(&Crc8Result);
//	crc8Update(&Crc8Result, dadaBuffer, sizeof(dadaBuffer)-1);
//	crc8Finish(&Crc8Result);
//	return 0;
//}
