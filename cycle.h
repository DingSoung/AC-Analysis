#ifndef DS_FREQUENCY_H
#define DS_FREQUENCY_H

// 计算buffer的平均周期,负数表示没有
static float cycle(short *pReturn, unsigned int Length) {
	unsigned int len = 0, lentemp, sum = 0;
	// 等待第一个零点
	while (*pReturn <= *(++pReturn) && Length--);	
	do {
		lentemp++;
		//累计时间和周期
		*pReturn <= *(++pReturn) ? (sum++, len += lentemp, lentemp = 0);	
	} while (Length--);
	return (float)len / sum;
}

#endif
