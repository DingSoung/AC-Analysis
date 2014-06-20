#ifndef ___CRC_H___
#define ___CRC_H___

void crc32Init(unsigned long *pCrc32);
void crc32Update(unsigned long *pCrc32, unsigned char *pData, unsigned long uSize);
void crc32Finish(unsigned long *pCrc32);

#endif /* ___CRC_H___ */
