#ifndef DS_FREQUENCY_H
#define DS_FREQUENCY_H

/*
计算buffer的平均周期,负数表示没有
*/
static float Cycle(short *pReturn, unsigned int Length){
	unsigned int len = 0, lentemp, sum = 0;
	while (*pReturn <= *(++pReturn) && Length--);//等待第一个零点
	do{
		lentemp++;
		*pReturn <= *(++pReturn) ? (sum++, len += lentemp, lentemp = 0);//累计时间和周期
	} while (Length--);
	return (float)len / sum;
}


#endif
