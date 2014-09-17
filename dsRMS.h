#ifndef DS_RMS_H
#define DS_RMS_H

#include "stdlib.h"
#include "math.h"

unsigned int *coeBuffer;
unsigned short coeLength;

/* ¾í»ý¼ÆËã */
unsigned int convolutionLength(unsigned int ft, unsigned int gt) {
	return ft + gt - 1;
}
void convolutionBuffer(int *f, unsigned int ft, int *g, unsigned int gt, int *r) {
	unsigned int rt = ft + gt - 1;
	unsigned int i, j;
	for (i = 0; i < rt; i++) {
		j = (i - gt + 1 > 0) ? (i - gt + 1) : 0;
		ft = (ft - 1 < i) ? (ft - 1) : i;
		while (j < ft) {
			r[i] += f[j] * g[i - j];
			j++;
		}
	}
}

int quasi_Init(unsigned int fs, float f) {
	/* check frequency */
	if (fs / f < 2) return -1;
	/*auto get coe length, coe > 2^32 when length > xxx*/
	unsigned short m = (unsigned int)(fs / f);
	unsigned int *c = (unsigned int *)malloc(m * sizeof(int));
	if (c == 0x00) return -1;
	for (unsigned short i = 0x00; i < m; i++)
		c[i] = i;

	coeLength = m;
	coeBuffer = (unsigned int *)malloc(coeLength * sizeof(int));
	if (coeBuffer == 0x00) return -1;
	for (unsigned short i = 0x00; i < coeLength; i++)
		coeBuffer[i] = c[i];

	for (unsigned short i = 0x00; i < 0x03; i++) {
		unsigned short coeLengthTemp = coeLength;
		unsigned int *coeBufferTemp = coeBuffer;

		coeLength = convolutionLength(coeLength, m);
		coeBuffer = (unsigned int *)malloc(coeLength * sizeof(int));
		if (coeBuffer == 0x00) return -1;
		convolutionBuffer((int *)coeBufferTemp, coeLengthTemp, (int *)c, m, (int *)coeBuffer);
		free(coeBufferTemp);
	}
	return 1;
}

/*
quasi-synchronous sampling for RMS of U and I
Normal
RMS=Sqrt((DATA[i]^2)/M)
quasi-synchronous
RMS=Sqrt(¡Æ(DATA[i]^2*Coe[i])/pow(M,CYCLE_CNT))
*/
float quasi_RMS(short *pBuffer, unsigned int length, unsigned int fs, float f) {
	unsigned int i = 0;
	unsigned long long j = 0, k = 0;

	for (i = 0; i < length; i++) {
		j = pBuffer[i] * pBuffer[i];
		k += j * coeBuffer[i % coeLength];
	}
	return sqrt(k / pow((fs / f), 4));
}

int quasi_finish() {
	free(coeBuffer);
	return 0;
}

#endif