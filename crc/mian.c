#include "crc8.h"
#include "crc16.h"
#include "crc32.h"
#include "stdio.h"
int main() {
    unsigned char dadaBuffer[] = "Hello CRC!";
        
	unsigned char crc8Result;
   	crc8Init(&crc8Result);
	crc8Update_Direct(&crc8Result, dadaBuffer, sizeof(dadaBuffer)-1);
	crc8Finish(&crc8Result);
	printf("crc8 direct result = %x \n",crc8Result);
	
	init_crc8_table();
	crc8Init(&crc8Result);
	crc8Update(&crc8Result, dadaBuffer, sizeof(dadaBuffer)-1);
	crc8Finish(&crc8Result);
    printf("crc8 use table result = %x \n",crc8Result);

	unsigned short crc16Result;
	init_crc16_table();
    crc16Init(&crc16Result);
    crc16Update(&crc16Result, dadaBuffer, sizeof(dadaBuffer)-1);
    crc16Finish(&crc16Result);
	printf("crc16 result = %x \n",crc16Result);
	
	unsigned int crc32Result;
	init_crc32_table();
	crc32Init(&crc32Result);
	crc32Update(&crc32Result, dadaBuffer, sizeof(dadaBuffer)-1);
	crc32Finish(&crc32Result);
	printf("crc32 result = %x \n",crc32Result);
	return 0;
}

