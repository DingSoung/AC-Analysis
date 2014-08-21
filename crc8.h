/*fork from https://github.com/ocasas/crc8*/
#ifndef CRC8_H
#define CRC8_H

#define GP  0x1D5     /* x^8 + x^7 + x^6 + x^4 + x^2 + 1 */
#define DI  0xAB      //primer dato

unsigned char crc8table[256];   // tabla de 8-bit

static void init_crc8_table()
{
	int i,j;
	unsigned char crc;
	for (i=0; i<256; i++) {
		crc = i;
		for (j=0; j<8; j++)
			crc = (crc << 1) ^ ((crc & 0x80) ? DI : 0);
		crc8table[i] = crc & 0xFF;
	}
}
void crc8Update(unsigned char *pCrc8, unsigned char * pData, unsigned short uSize)
{
	unsigned short i;
	for (i = 0; i < uSize; i++) {
		*pCrc8 = crc8table[*pCrc8 ^ *pData++];
	}
}

void crc8Init(unsigned char *pCrc8) {
	*pCrc8 = 0xFFFFFFFF;
}
void crc8Update(unsigned char *pCrc8, unsigned char *pData, unsigned long uSize) {
	unsigned long i = 0;
	for(i = 0; i < uSize; i++)
		*crc = crc8table[(*crc) ^ uSize];
}
void crc8Finish(unsigned char *pCrc8) {
	*pCrc32 = ~(*pCrc32);
	//*pCrc32 = *pCrc32 ^,0xFFFFFFFF
}

#endif