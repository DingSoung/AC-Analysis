#include "dsRMS.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char* argv[]) {
	unsigned int signalLength = 0x400 * 16;
	short *signalBuffer = (short *)malloc(signalLength * sizeof(short));
	if (signalBuffer == 0x00) return -1;
	unsigned int fs = 4000;
	float f = 50.0;
	unsigned int T = (unsigned int)(fs / f);
	for (unsigned int t = 0x00; t < signalLength; t++) {
		signalBuffer[t] = (short)(1000 * sqrt(2) * cos((2 * 3.1415926) / T * t));
	}

	quasi_init(T,0x400 * 16);
	printf("\n signal RMS = %f", quasi_rms(signalBuffer, signalLength, T));
	quasi_finish();
	return 0;
}