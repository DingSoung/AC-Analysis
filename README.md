##DDT
Digital Data Transmition,CRC

###CRC32 CRC16 CRC8
Usage:
	#include "crc8.h"
	#include "crc16.h"
	#include "crc32.h"
	int main() {
		unsigned char dadaBuffer[] = "Hello CRC!";
		
		unsigned char Crc8Result2;
		crc8Init(&Crc8Result2);
		crc8Update_Direct(&Crc8Result2, dadaBuffer, sizeof(dadaBuffer)-1);
		crc8Finish(&Crc8Result2);
		
		unsigned char Crc8Result;
		init_crc8_table();
		crc8Init(&Crc8Result);
		crc8Update(&Crc8Result, dadaBuffer, sizeof(dadaBuffer)-1);
		crc8Finish(&Crc8Result);
		
		unsigned short Crc16Result;
		init_crc16_table();
		crc16Init(&Crc16Result);
		crc16Update(&Crc16Result, dadaBuffer, sizeof(dadaBuffer)-1);
		crc16Finish(&Crc16Result);
		
		unsigned int Crc32Result;
		init_crc32_table();
		crc32Init(&Crc32Result);
		crc32Update(&Crc32Result, dadaBuffer, sizeof(dadaBuffer)-1);
		crc32Finish(&Crc32Result);
		return 0;
	}

References:
[Wikipedia](http://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks)
[HezhouW](https://github.com/HezhouW/CRC16_CCITT), 
[dtdns](http://www.macs123.dtdns.net/algo/cpp/cpp025.html)
[ocasas](https://github.com/ocasas/crc8)
[htmlxx](http://blog.csdn.net/htmlxx/article/details/17369105#comments),
[http://depa.usst.edu.cn/](http://depa.usst.edu.cn/chenjq/www2/software/crc/CRC_Javascript/CRCcalculation.htm)
