#ifndef DS_FREQUENCY_H
#define DS_FREQUENCY_H

/*
����buffer��ƽ������,������ʾû��
*/
static float Cycle(short *pReturn, unsigned int Length){
	unsigned int len = 0, lentemp, sum = 0;
	while (*pReturn <= *(++pReturn) && Length--);//�ȴ���һ�����
	do{
		lentemp++;
		*pReturn <= *(++pReturn) ? (sum++, len += lentemp, lentemp = 0);//�ۼ�ʱ�������
	} while (Length--);
	return (float)len / sum;
}


#endif
