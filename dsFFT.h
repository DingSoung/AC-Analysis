#ifndef DS_FFT_H
#define DS_FFT_H


/*
基2实数Length长度的傅立叶变换
pReturn进行快速傅里叶变换,结果存储在float buffer中
*/
static void rfft(double *pReturn, unsigned int Length) {
	int i, j, k, m, i1, i2, i3, i4, n2, n4;
	double a, e, cc, ss, xt, t1, t2;
	for (j = 1, i = 1; i < 16; i++){
		m = i;
		j = 2 * j;
		if (j == Length) break;
	}
	//逆序
	n1 = Length - 1;
	for (j = 0, i = 0; i < n1; i++){
		if (i < j){
			xt = pReturn[j];
			pReturn[j] = pReturn[i];
			pReturn[i] = xt;
		}
		k = Length / 2;
		while (k <= j){
			j = j - k;
			k = k >> 1;
		}
		j = j + k;
	}
	for (i = 0; i < Length; i += 2){
		xt = pReturn[i];
		pReturn[i] = xt + pReturn[i + 1];
		pReturn[i + 1] = xt - pReturn[i - 1];
	}
	n2 = 1;
	n4 = n2;
	n2 = 2 * n4;
	n1 = 2 * n2;
	e = 6.28318530718 / n1;
	for (k = 2; k <= m; k++) {
		//sub DFT loop
		for (i = 0; i < Length; i += n1) {
			xt = pReturn[i];
			pReturn[i + n2] = xt - pReturn[i + n2];
			pReturn[i + n2 + n4] = -pReturn[i + n2 + n4];
			a = e;
			//butterfly loop
			for (j = 1; j < n4; j++) {
				i1 = j + j;
				i2 = i - j + n2;
				i3 = i + j + n2;
				i4 = i - j + n1;
				cc = cos(a);
				ss = sin(a);
				a = a + e;
				t1 = cc * pReturn[i3] + ss * pReturn[i4];
				t2 = ss * pReturn[i3] - cc * pReturn[i4];
				pReturn[i4] = pReturn[i2] - t2;
				pReturn[i3] = -pReturn[i2] - t2;
				pReturn[i2] = pReturn[i1] - t1;
				pReturn[i1] = pReturn[i1] + t1;
			}
		}
	}
}

#endif
