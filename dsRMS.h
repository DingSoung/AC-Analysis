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

int quasi_init(unsigned int cycle, unsigned int bufferLength) {
	unsigned int *c = (unsigned int *)malloc(cycle * sizeof(unsigned int));
	if (c == 0x00) return -1;
	for (unsigned short i = 0; i < cycle; i++)
		c[i] = 1;

	coeLength = cycle;
	coeBuffer = (unsigned int *)malloc(coeLength * sizeof(unsigned int));
	if (coeBuffer == 0x00) {
		free(c);
		return -1;
	}
	for (unsigned short i = 0; i < coeLength; i++)
		coeBuffer[i] = c[i];

	while (convolutionLength(coeLength, cycle) <= bufferLength) {
		unsigned short coeLengthTemp = convolutionLength(coeLength, cycle);
		unsigned int *coeBufferTemp = (unsigned int *)malloc(coeLengthTemp * sizeof(unsigned int));
		if (coeBufferTemp == 0x00) break;
		if (convolutionBuffer(coeBuffer, coeLength, c, cycle, coeBufferTemp) > 0x0FFFFFFF) {
			free(coeBufferTemp);
			break;
		}
		free(coeBuffer);
		coeBuffer = coeBufferTemp;
		coeLength = coeLengthTemp;
	}
	free(c);

	for (unsigned int i = 0; i < coeLength;i++)
		printf("%d ", coeBuffer[i]);
	printf("%d", coeLength);

	return 1;
}
float quasi_rms(short *buffer, unsigned int bufferLength, unsigned int cycle) {
	unsigned long long temp1 = 0, temp2 = 0;
	for (unsigned int i = 0x00; i + coeLength <= bufferLength;) {
		for (unsigned int j = 0; j < coeLength; j++, i++) {
			temp1 = buffer[i] * buffer[i];
			temp2 += temp1 * coeBuffer[j];
		}
	}
	return (float)sqrt(temp2 / pow(cycle, bufferLength/cycle));
}
int quasi_finish() {
	free(coeBuffer);
	return 0;
}

#endif