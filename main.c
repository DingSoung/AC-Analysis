#include "dsRMS.h"
#include "stdlib.h"


int main(int argc, char* argv[]) {
	unsigned int signalLength = 0x400 * 16;
	unsigned short *signalBuffer = (unsigned short *)malloc(signalLength);
	if (signalBuffer == 0x00) return -1;
	unsigned int fs = 4000;
	float f = 50.0;
	

	for (unsigned int i = 0x00; i < signalLength; i++) {
		signalBuffer[i] = 220 * sqrt(2) * cos((2 * 3.1415926) / (fs / f) * i);
	}
	quasi_Init(fs, f);
	printf("\n signal RMS = %f", quasi_RMS(signalBuffer, signalLength, fs, f));
	quasi_finish();
	return 0;
}