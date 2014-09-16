#ifndef DS_RMS_H
#define DS_RMS_H

#include "stdlib.h"

unsigned int *coeBuffer;
unsigned short coeLength;

/* ¾í»ý¼ÆËã */
unsigned int convolutionLength(unsigned int ft, unsigned int gt) {
	return ft + gt - 1;
}
void convolutionBuffer(int *f, unsigned int ft, int *g, unsigned int gt, int *r) {
	int rt = ft + gt - 1;
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
	unsigned int coeLength = (unsigned int)(fs/f);
	while (coeLength < 10000){
		coeLength = convolutionLength(coeLength,(unsigned int)(fs/f));
		coeBuffer = (unsigned int *)malloc(coeLength);
		if (coeBuffer == 0x00) return -1;
		
		
		
		
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
float Quasi_RMS(short *pBuffer, unsigned int length) {
	unsigned int i = 0;
	unsigned long long j = 0,k = 0;
	
	for (i = 0; i < length; i++) {
		j = pBuffer[i] * pBuffer[i];
		k += j * coeBuffer[i % coeLength];
	}
	return squr(k / length)
		DATA->RMS_Volt[channel] = sqrt(RMS_tmp[1] / 40960000.0)*SamplingCoefficientOffsetVolt[channel];
}

int quasi_finish() {
	free(coeBuffer);
}



#endif