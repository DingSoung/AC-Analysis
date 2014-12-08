#include <iostream>
using namespace std;
#include <math.h>
#include <windows.h>// for HANDLE  
#include <process.h>// for _beginthread()  

#define DIM 1024

unsigned char RD(int i, int j) {
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + sqrt(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int((i + DIM)*s + y) % 2 + int((DIM * 2 - i)*s + y) % 2) * 127;
}
unsigned char GR(int i, int j) {
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + sqrt(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int(5 * ((i + DIM)*s + y)) % 2 + int(5 * ((DIM * 2 - i)*s + y)) % 2) * 127;
}
unsigned char BL(int i, int j) {
	float s = 3. / (j + 99);
	float y = (j + sin((i*i + sqrt(j - 700) * 5) / 100. / DIM) * 35)*s;
	return (int(29 * ((i + DIM)*s + y)) % 2 + int(29 * ((DIM * 2 - i)*s + y)) % 2) * 127;
}
FILE *fp;
HANDLE handleMutex;

inline void pixel_write(int i, int j) {
	static unsigned char color[3];
	color[0] = RD(i, j) & 255;
	color[1] = GR(i, j) & 255;
	color[2] = BL(i, j) & 255;
	WaitForSingleObject(handleMutex, -1);
	fwrite(color, 1, 3, fp);
	ReleaseMutex(handleMutex);
}
#if 0
unsigned int __stdcall write_line(void* startLine) {
#else
unsigned long WINAPI write_line(void* startLine) {
#endif
	unsigned int i = (unsigned int)startLine;
	for (; i < (unsigned int)startLine + (DIM >> 2); i++) {
		for (int j = 0; j < DIM; j++)
			pixel_write(j, i);
	}
	
	return 0;
}
int main() {
	fp = fopen("MathPic.ppm", "wb");
	fprintf(fp, "P6\n%d %d\n255\n", DIM, DIM);

	handleMutex = CreateMutex(NULL, FALSE, 0);
#if 0
	HANDLE handle[4];
	unsigned int *threadAddr[4];
	for (unsigned int i = 0; i < 4; i++) {
		handle[i] = (HANDLE)_beginthreadex(0, 1024, write_line, (void *)(i*DIM >> 2), 0, NULL);
		cout << "thread %d created \r\n", threadAddr[1];
		WaitForSingleObject(handle[i], -1);
	}
	WaitForSingleObject(handle[0], -1);
	WaitForSingleObject(handle[1], -1);
	WaitForSingleObject(handle[2], -1);
	WaitForSingleObject(handle[3], -1);
	
	//WaitForMultipleObjects(sizeof(handle) >> 2, handle, 1, -1);
#else
	void *handle[4];
	unsigned long threadID[4];
	for (unsigned int i = 0; i < 4; i++) {
		handle[i] = CreateThread(0, 1024, write_line, (void *)(i*DIM >> 2), 0, &threadID[i]);
		cout << "thread %d created \r\n", threadID[i];
	}
	WaitForSingleObject(handle[0], INFINITE);
	WaitForSingleObject(handle[1], INFINITE);
	WaitForSingleObject(handle[2], INFINITE);
	WaitForSingleObject(handle[3], INFINITE);
#endif
	fclose(fp);
	return 0;
}
