#ifndef DS_FREQUENCY_H
#define DS_FREQUENCY_H

// ����buffer��ƽ������,������ʾû��
static float cycle(short *pReturn, unsigned int Length) {
	unsigned int len = 0, lentemp, sum = 0;
	// �ȴ���һ�����
	while (*pReturn <= *(++pReturn) && Length--);	
	do {
		lentemp++;
		//�ۼ�ʱ�������
		*pReturn <= *(++pReturn) ? (sum++, len += lentemp, lentemp = 0);	
	} while (Length--);
	return (float)len / sum;
}

#endif
