#ifndef DS_DC_H
#define DS_DC_H

/*
��һ�����ȵ�buffer����ȥֱ�������������һ��ֱ���������ϲ���ã�����һ��buffer���롣
����N���ڵ���������
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