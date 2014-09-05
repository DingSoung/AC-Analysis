#ifndef CRC32_H
#define CRC32_H

static void init_crc32_table(void) ;
static void crc32Init(unsigned int *pCrc32) ;
static void crc32Update(unsigned int *pCrc32, unsigned char *pData, unsigned int uSize);
static void crc32Finish(unsigned int *pCrc32);

#endif