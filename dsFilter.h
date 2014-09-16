#ifndef DS_FILTER_H
#define DS_FILTER_H


/* 对一段buffer按照给定的参数进行滤波 */
#define I2R_M  2	/*分子阶数*/
#define I2R_N  2	/*分母阶数*/
const float b[I2R_M + 1] = {	/*分子多项式系数*/
	0.14552330359746207, 0.29104660719492415, 0.14552330359746207
};
const float a[I2R_N + 1] = {	/*分母多项式系数*/
	1, -0.66997394102558094, 0.25206715541542912
};
static void I2R_Filter(short *pReturn, unsigned int Length) {
	static float in[I2R_M + 1] = { 0 };	/*输入序列*/
	static float out[I2R_N + 1] = { 0 };	/*输出序列*/
	int i, k;
	double sum;
	for (k = 0； k < Length; k++, pReturn++) {
		in[0] = *pReturn;	/*数据输入*/
		sum = 0;
		for (i = 0; i < I2R_M; i++)	/*计算当前衰减或增益量*/
			sum = sum + b[i] * in[i];
		for (i = 1; i < I2R_N; i++)
			sum = sum - a[i] * out[i];
		for (i = I2R_M; i > 1; i--)	/*为下一个数做准备*/
			in[i] = in[i - 1];
		for (i = I2R_N; i >2; i--)
			out[i] = out[i - 1];
		out[1] = sum;
		*pReturn += sum;	/*数据输出*/
	}
}

#endif