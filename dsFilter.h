#ifndef DS_FILTER_H
#define DS_FILTER_H


/* ��һ��buffer���ո����Ĳ��������˲� */
#define I2R_M  2	/*���ӽ���*/
#define I2R_N  2	/*��ĸ����*/
const float b[I2R_M + 1] = {	/*���Ӷ���ʽϵ��*/
	0.14552330359746207, 0.29104660719492415, 0.14552330359746207
};
const float a[I2R_N + 1] = {	/*��ĸ����ʽϵ��*/
	1, -0.66997394102558094, 0.25206715541542912
};
static void I2R_Filter(short *pReturn, unsigned int Length) {
	static float in[I2R_M + 1] = { 0 };	/*��������*/
	static float out[I2R_N + 1] = { 0 };	/*�������*/
	int i, k;
	double sum;
	for (k = 0�� k < Length; k++, pReturn++) {
		in[0] = *pReturn;	/*��������*/
		sum = 0;
		for (i = 0; i < I2R_M; i++)	/*���㵱ǰ˥����������*/
			sum = sum + b[i] * in[i];
		for (i = 1; i < I2R_N; i++)
			sum = sum - a[i] * out[i];
		for (i = I2R_M; i > 1; i--)	/*Ϊ��һ������׼��*/
			in[i] = in[i - 1];
		for (i = I2R_N; i >2; i--)
			out[i] = out[i - 1];
		out[1] = sum;
		*pReturn += sum;	/*�������*/
	}
}

#endif