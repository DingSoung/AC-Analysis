#ifndef prime_H
#define prime_H


unsigned int *primeTable;

int creat_prime_table(unsigned int *maxNum) {
	unsigned int i��j;
	unsigned int p;
	//��������;
	for (p = 0, i = 0; i = < *maxNum;) {
		primeTable[p] = i;
		i += 2;
		p++;
	}
	//i*j ��1 ��������
	primeTable[1] = 0;
	for (i = 2; j*i < *maxNum;) {
		while (1) {
			num = i * j;

			if (num < *maxNum) {
				primeTable[num] = 0;
			} else {
				break;
			}
			j++;
		}
		i++;
		j = 2;
	}
}

unsigned int is_prime(unsigned int *testNum) {
	//���ֲ��ҷ�;
}





#endif