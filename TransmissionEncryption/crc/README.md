###CRC32 CRC16 CRC8
use table

###Usage:
in c/c++ include the header file and use the functions directly. ex: main.c
in swift <...hold on, I'll try tonight ..>

	endian =
	0: msb,	RefIn = TRUE,	RefOut = TRUE	Mirror_Poly
	1: lsb,	RefIn = FALSE,	RefOut = FALSE	Normal_Poly

###Different criterion
refer [Wikipedia](http://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks) to add a rule ,for example CRC16 IBM below, uncommant it, and commant the last one. the some examples, if I miss any， pull request is welcome

	/* CRC-16 IBM
	#define endian16	0
	#define Poly16_Normal	0x8005
	#define Poly16_Mirror	0xA001
	#define Crc16_Init	0x0000
	#define Crc16_XorOut	0x0000
	*/

###References:
[Wikipedia](http://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks),
[HezhouW](https://github.com/HezhouW/CRC16_CCITT), 
[dtdns](http://www.macs123.dtdns.net/algo/cpp/cpp025.html),
[ocasas](https://github.com/ocasas/crc8),
[htmlxx](http://blog.csdn.net/htmlxx/article/details/17369105#comments),
[http://depa.usst.edu.cn/](http://depa.usst.edu.cn/chenjq/www2/software/crc/CRC_Javascript/CRCcalculation.htm),
[How to call c in swift](http://stackoverflow.com/questions/24004732/how-to-call-c-from-swift)
