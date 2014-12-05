#include <iostream>
using namespace std;
#include <math.h>
#include <windows.h>// for HANDLE  
#include <process.h>// for _beginthread()  

#define DIM 1024
#define DM1 (DIM-1)

#if 0
#include <cstdlib>
#define _sq(x) ((x)*(x)) // square
#define _cb(x) abs((x)*(x)*(x)) // absolute value of cube
#define _cr(x) (unsigned char)(pow((x),1.0/3.0)) // cube root
unsigned char RD(int i, int j) {
	// YOUR CODE HERE
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + _sq(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int((i + DIM)*s + y) % 2 + int((DIM * 2 - i)*s + y) % 2) * 127;
}
unsigned char GR(int i, int j) {
	// YOUR CODE HERE
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + _sq(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int(5 * ((i + DIM)*s + y)) % 2 + int(5 * ((DIM * 2 - i)*s + y)) % 2) * 127;
}
unsigned char BL(int i, int j) {
	// YOUR CODE HERE
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + _sq(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int(29 * ((i + DIM)*s + y)) % 2 + int(29 * ((DIM * 2 - i)*s + y)) % 2) * 127;
}
#endif
#if 0
unsigned char RD(int i, int j) {
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + _sq(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int((i + DIM)*s + y) % 2 + int((DIM * 2 - i)*s + y) % 2) * 127;
}
unsigned char GR(int i, int j) {
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + _sq(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int((i + DIM)*s + y) % 2 + int((DIM * 2 - i)*s + y) % 2) * 127;
}
unsigned char BL(int i, int j) {
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + _sq(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int((i + DIM)*s + y) % 2 + int((DIM * 2 - i)*s + y) % 2) * 127;
}
#endif
#if 0
unsigned char RD(int i, int j) {
	float s = 3. / (j + 99);
	return (int((i + DIM)*s + j*s) % 2 + int((DIM * 2 - i)*s + j*s) % 2) * 127;
}
unsigned char GR(int i, int j) {
	float s = 3. / (j + 99);
	return (int((i + DIM)*s + j*s) % 2 + int((DIM * 2 - i)*s + j*s) % 2) * 127;
}
unsigned char BL(int i, int j) {
	float s = 3. / (j + 99);
	return (int((i + DIM)*s + j*s) % 2 + int((DIM * 2 - i)*s + j*s) % 2) * 127;
}
#endif
#if 1
unsigned char RD(int i, int j) {
	float x = 0, y = 0; int k; for (k = 0; k++ < 256;) { float a = x*x - y*y + (i - 768.0) / 512; y = 2 * x*y + (j - 512.0) / 512; x = a; if (x*x + y*y>4)break; }return log(k) * 47;
}
unsigned char GR(int i, int j) {
	float x = 0, y = 0; int k; for (k = 0; k++ < 256;) { float a = x*x - y*y + (i - 768.0) / 512; y = 2 * x*y + (j - 512.0) / 512; x = a; if (x*x + y*y>4)break; }return log(k) * 47;
}
unsigned char BL(int i, int j) {
	float x = 0, y = 0; int k; for (k = 0; k++ < 256;) { float a = x*x - y*y + (i - 768.0) / 512; y = 2 * x*y + (j - 512.0) / 512; x = a; if (x*x + y*y>4)break; }return 128 - log(k) * 23;
}
#endif



FILE *fp;
void pixel_write(int i, int j) {
	static unsigned char color[3];
	color[0] = RD(i, j) & 255;
	color[1] = GR(i, j) & 255;
	color[2] = BL(i, j) & 255;
	fwrite(color, 1, 3, fp);
	//seek()
}
unsigned int __stdcall write_line(void *startLine) {
	for (int i = (unsigned int)startLine; i < (unsigned int)startLine + (DIM >> 2); i++) {
		for (int j = 0; j < DIM; j++)
			pixel_write(i, j);
	}
	return 0;
}
int main() {
	fp = fopen("/MathPic.ppm", "wb");
	fprintf(fp, "P6\n%d %d\n255\n", DIM, DIM);

	unsigned int startLine = 0;
	HANDLE handle[4];
	unsigned int *threadAddr[4];
	for (unsigned int i = 0; i < 4; i++) {
		handle[i] = (HANDLE)_beginthreadex(0, 1024, write_line, (void *)startLine, 0, threadAddr[i]);
		cout << "thread %d created \r\n", threadAddr[i];
		startLine += DIM >> 2;
	}

	WaitForSingleObject(handle[1], -1);
	WaitForMultipleObjects(sizeof(handle)>>2,handle,1,-1);

	fclose(fp);
	return 0;
}
