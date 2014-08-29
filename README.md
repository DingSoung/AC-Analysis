DDT
===
Digital Data Transmition,数字数据传输,加密,通信协议,通信接口,USART,IIC,JTAG,USB,CRC,HASH


###CRC32 CRC16 CRC8
Refer 
[HezhouW](https://github.com/HezhouW/CRC16_CCITT), 
[htmlxx](http://blog.csdn.net/htmlxx/article/details/17369105#comments),
[http://depa.usst.edu.cn/](http://depa.usst.edu.cn/chenjq/www2/software/crc/CRC_Javascript/CRCcalculation.htm)

	#include "crc16.h"
	#include "crc32.h"
	int main(){
		unsigned char dadaBuffer[] = "Hello CRC!";
		
		unsigned short Crc16Result;
		init_crc16_table();
		crc16Init(&Crc16Result);
		crc16Update_by_byte(&Crc16Result, dadaBuffer, sizeof(dadaBuffer)-1);
		crc16Finish(&Crc16Result);

		unsigned int Crc32Result;
		init_crc32_table();
		crc32Init(&Crc32Result);
		crc32Update(&Crc32Result, dadaBuffer, sizeof(dadaBuffer)-1);
		crc32Finish(&Crc32Result);
		
		return 0;
	}
