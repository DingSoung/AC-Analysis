#ifndef DS_RMS_H
#define DS_RMS_H

#include "stdlib.h"
#include "math.h"

#include "convolution.h"

double rms(short *buffer, unsigned int length) {
	double sum = 0x00;
	for (unsigned int i = 0x00; i < length; i++) {
		unsigned int temp = buffer[i] * buffer[i];
		sum += temp;
	}
	return sqrt(sum / length);
}

unsigned int *coeBuffer;
unsigned short coeLength;

int quasi_init(unsigned int T) {
	/*auto get coeBuffer length, > 2^32 when length > xxx*/
	unsigned int *c = (unsigned int *)malloc(T * sizeof(int));
	if (c == 0x00) return -1;
	for (unsigned short i = 1; i < T; i++)
		c[i-1] = i;

	coeLength = T;
	coeBuffer = (unsigned int *)malloc(coeLength * sizeof(int));
	if (coeBuffer == 0x00) {
		free(c);
		return -1;
	}
	for (unsigned short i = 0; i < coeLength; i++)
		coeBuffer[i] = c[i];


	while (1) {
		unsigned short coeLengthTemp = coeLength;
		unsigned int *coeBufferTemp = coeBuffer;

		coeLength = convolutionLength(coeLengthTemp, T);
		coeBuffer = (unsigned int *)malloc(coeLength * sizeof(int));
		if (coeBuffer == 0x00) {
			free(c);
			free(coeBufferTemp);
			return -1;
		}
		int convStatus = convolutionBuffer(coeBufferTemp, coeLengthTemp, c, T, coeBuffer);
		if (!convStatus) {
			free(c);
			free(coeBuffer);
			coeBuffer = coeBufferTemp;
			coeLength = coeLengthTemp;
			
			return 1;
		}
		free(coeBufferTemp);
	}

}
float quasi_rms(short *buffer, unsigned int length, unsigned int T) {
	unsigned int i = 0;
	unsigned long long j = 0, k = 0;

	for (i = 0; i < length; i++) {
		j = buffer[i] * buffer[i];
		k += j * coeBuffer[i % coeLength];
	}
	return (float)sqrt(k / pow(T, 4));
}
int quasi_finish() {
	free(coeBuffer);
	return 0;
}

#endif