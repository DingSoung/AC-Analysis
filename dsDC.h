#ifndef DS_DC_H
#define DS_DC_H

/*
对一定长度的buffer进行去直流，并返回最后一个直流量，供上层调用，和下一段buffer传入。
长度N用于调节灵敏度
*/
short Eliminate_DC(short avg, short N, short *pReturn, unsigned int Length) {
	int sum;
	unsigned int len = 0;
	sum = avg * N;
	do {
		sum = sum + *pRetuen - avg;
		avg = sum / N;
		*(pReturn++) -= avg;
	} while (len++ < Length)
}


#endif