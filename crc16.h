#ifndef CRC16_H
#define CRC16_H

//一、CRC16/IBM 或 CRC16/ARC 或 CRC16/LHA：
//    公式：x16+x15+x2+1
//    宽度：16
//    Poly值：0x8005
//    初始值：0x0000
//    基准输入：true
//    基准输出：true
//    标志位：0x0000
// 
//二、CRC16/MAXIM：
//    公式：x16+x15+x2+1
//    宽度：16
//    Poly值：0x8005
//    初始值：0x0000
//    基准输入：true
//    基准输出：true
//    标志位：0xFFFF
// 
//三、CRC16/USB：
//    公式：x16+x15+x2+1
//    宽度：16
//    Poly值：0x8005
//    初始值：0xFFFF
//    基准输入：true
//    基准输出：true
//    标志位：0xFFFF
// 
//四、CRC16/MODBUS(最常见)：
//    公式：x16+x15+x2+1
//    宽度：16
//    Poly值：0x8005
//    初始值：0x0000
//    基准输入：true
//    基准输出：true
//    标志位：0x0000
// 
//五、CRC16/CCITT 或 CRC-CCITT 或CRC16/CCITT-TRUE或 CRC16/KERMIT：
//    公式：x16+x15+x5+1
//    宽度：16
//    Poly值：0x1021
//    初始值：0x0000
//    基准输入：true
//    基准输出：true
//    标志位：0x0000
// 
//六、 CRC16/CCITT-FALSE：
//    公式：x16+x15+x5+1
//    宽度：16
//    Poly值：0x1021
//    初始值：0xFFFF
//    基准输入：false
//    基准输出：false
//    标志位：0x0000
// 
//七、CRC16/X25：
//    公式：x16+x15+x5+1
//    宽度：16
//    Poly值：0x1021
//    初始值：0x0000
//    基准输入：true
//    基准输出：true
//    标志位：0xFFFF
// 
//八、CRC16/XMODEM 或 CRC16/ZMODEM 或 CRC16/ACORN：
//    公式：x16+x15+x5+1
//    宽度：16
//    Poly值：0x1021
//    初始值：0x0000
//    基准输入：false
//    基准输出：false
//    标志位：0x0000
// 
//九、CRC16/DNP：
//    公式：x16+x13+x12+x11+x10+x8+x6+x5+x2+1
//    宽度：16
//    Poly值：0x3D65
//    初始值：0x0000
//    基准输入：true
//    基准输出：true
//    标志位：0xFFFF
//   使用：M-Bus, ect

/* CRC-CCITT XMODEM */
#define Crc16_Poly	0x1021	/*0x1021 0x3D65 0x8005*/
#define Crc16_Init	0x0000	/*0x0000 0xFFFF*/
#define Crc16_XorOut	0x0000	/*0x0000 0xFFFF*/
#define Crc16_RefIn	0	/*FALSE = 0, TRUE = !FALSE*/
#define	Crc16_RefOut	0

static unsigned short crc16table[256] /*  = { Ploy = 0x1021 
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0,
}*/;
static unsigned int crc4table[16]={ /* CRC 半字节余式表 */
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};

static void init_crc16_table(void) {
	for ( unsigned short i = 0; i < 256; i++ ) {
		unsigned short crc = ( unsigned short )( i << 8 );
		for ( int j = 0; j < 8; j++ ) {
			if( crc & 0x8000 )
				crc = ( crc << 1 ) ^ Crc16_Poly;
			else
				crc <<= 1;
		}
		crc16table[i] = (unsigned short)crc;
	}
}

static void crc16Init(unsigned short *pCrc16) {
	*pCrc16 = Crc16_Init;
}

/*CRC算法一，适用于程序空间苛刻但CRC计算速度要求不高的微控制系统
算法概要说明：计算本位后的CRC 码等于上一位CRC 码乘以2 后除
以多项式，所得的余数再加上本位值除以多项式所得的余数
*/
static void crc16Update_by_bit(unsigned short *pCrc16, unsigned char *ptr, unsigned int len) {
	while(len-- != 0) {
		for(unsigned char i = 0x80; i != 0; i >>= 1) {
			*pCrc16 <<= 1;
			if((*pCrc16&0x10000) !=0) //上一位CRC乘 2后，若首位是1，则除以 0x11021
				*pCrc16 ^= 0x11021;
			if((*ptr&i) != 0)    //如果本位是1，那么CRC = 上一位的CRC + 本位/CRC_CCITT
				*pCrc16 ^= Crc16_Poly;
		}
		ptr++;
	}
}

/*CRC算法二，适用于程序空间较大且CRC计算速度要求较高的计算机或微控制系统
算法概要说明：计算本字节后的CRC 码等于上一字节余式CRC码的低8位左移8位后，
再加上上一字节CRC 右移8 位（也既取高8 位）和本字节之和后所求得的CRC码
*/
static void crc16Update_by_byte(unsigned short *pCrc16, unsigned char *ptr, unsigned int len) {
	for(unsigned int i = 0; i < len; i++)
		*pCrc16 = ((*pCrc16) << 8) ^ crc16table[((*pCrc16) >> 8) ^ ptr[i]];
	//*pCrc16 = (*pCrc16 << 8) ^ crc16table[(*pCrc16 >> 8) ^ ptr[i]];
		//*pCrc16 = ((*pCrc16) >> 8) ^ crc16table[(ptr[i]) ^ ((*pCrc16) & 0x00FF)];
	
	//result  = ((result >> 8) & 0xff) ^ CRC16Table[(result & 0xff) ^ (dataIn[i] & 0xff)];
	// CRC16_1 = ((CRC16_1 << 8) | *point++) ^ Table_CRC16[(CRC16_1 >> 8) & 0xFF]; 
	//*pCrc16 = (*pCrc16 >> 8) ^ crc16table[*pCrc16 ^ *ptr];
	//*pCrc16 = (*pCrc16 << 8) ^ crc16table[((*pCrc16 >> 8) ^ *ptr) & 0xFF]; 
	//*pCrc16 = (*pCrc16 << 8) ^ crc16table[*ptr ^ (*pCrc16 >> 8)];
	//*pCrc16 = ((*pCrc16 << 8) & 0xFFFF) ^ crc16table[(*pCrc16 >> 8) ^ (*ptr & 0xFF)];
}

/*CRC算法三，适用于程序空间不太大且CRC计算速度又不可以太慢的微控制系统*/
static void crc16Update_by_halfbyte(unsigned short *pCrc16, unsigned char* ptr, unsigned int len) {
	while(len-- != 0) {
		unsigned char high = (unsigned char)(*pCrc16/4096); //暂存CRC的高4位
		*pCrc16 <<= 4;
		*pCrc16 ^= crc4table[high^(*ptr/16)];
		high = (unsigned char)(*pCrc16/4096);
		*pCrc16 <<= 4;
		*pCrc16 ^= crc4table[high^(*ptr&0x0f)];
		ptr++;
	}
}
static void icePub_makeCrc16(unsigned short *pCrc16, unsigned char *buffer,int bufferLen) {
	int i = 0;
	while (i < bufferLen) {
		for (int j = 0; j < 8; j++) {
			char c15 = ((*pCrc16 >> 15 & 1) == 1);
			char bit = ((buffer[i] >> (7 - j) & 1) == 1);
			*pCrc16 <<= 1;
			if (c15 ^ bit)
				*pCrc16 ^= Crc16_Poly;
		}
		i++;
	}
}

static void crc16Finish(unsigned short *pCrc16) {
	*pCrc16 ^=  Crc16_XorOut;
}

#endif
