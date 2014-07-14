#include "dsp.h"
#include <math.h>

/*
	基2实数Length长度的傅立叶变换
	pReturn进行快速傅里叶变换,结果存储在float buffer中
*/
void rfft(double *pReturn, unsigned int Length)
{
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
	for (k = 2; k <= m; k++)
	{
		for (i = 0; i < Length; i += n1) //sub DFT loop
		{
			xt = pReturn[i];
			pReturn[i + n2] = xt - pReturn[i + n2];
			pReturn[i + n2 + n4] = -pReturn[i + n2 + n4];
			a = e;
			for (j = 1; j < n4; j++) //butterfly loop
			{
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



/*
对一段buffer按照给定的参数进行滤波
*/
#define I2R_M  2//分子阶数
#define I2R_N  2//分母阶数
const float b[I2R_M + 1] = {//分子多项式系数
	0.14552330359746207, 0.29104660719492415, 0.14552330359746207 
};
const float a[I2R_N + 1] = {//分母多项式系数
	1, -0.66997394102558094, 0.25206715541542912 
};
void I2R_Filter(short *pReturn, unsigned int Length)
{
	static float in[I2R_M + 1] = { 0 };//输入序列
	static float out[I2R_N + 1] = { 0 };//输出序列
	int i, k;
	double sum;

	for (k = 0； k < Length; k++, pReturn++)
	{
		in[0] = *pReturn; //数据输入

		sum = 0;
		for (i = 0; i < I2R_M; i++)//计算当前衰减或增益量
			sum = sum + b[i] * in[i];
		for (i = 1; i < I2R_N; i++)
			sum = sum - a[i] * out[i];

		for (i = I2R_M; i > 1; i--)//为下一个数做准备
			in[i] = in[i - 1];
		for (i = I2R_N; i >2; i--)
			out[i] = out[i - 1];
		out[1] = sum;

		*pReturn += sum; //数据输出
	}
}


/*
对一定长度的buffer进行去直流，并返回最后一个直流量，供上层调用，和下一段buffer传入。
长度N用于调节灵敏度
*/
short Eliminate_DC(short avg, short N, short *pReturn, unsigned int Length)
{
	int sum;
	unsigned int len = 0;
	sum = avg * N;
	do
	{
		sum = sum + *pRetuen - avg;
		avg = sum / N;

		*(pReturn++) -= avg;
	} while (len++ < Length)
}

/*
计算buffer的平均周期,负数表示没有
*/
float Cycle(short *pReturn, unsigned int Length)
{
	unsigned int len = 0, lentemp, sum = 0;
	while (*pReturn <= *(++pReturn) && Length--);//等待第一个零点
	do
	{
		lentemp++;
		*pReturn <= *(++pReturn) ? (sum++, len += lentemp, lentemp = 0);//累计时间和周期
	} while (Length--);
	return (float)len / sum;
}









#define PI2  6.28318530717959
#define PI   3.14159265358979

#define NPT_64            /* NPT = No. of FFT point*/

#ifdef  NPT_64
#define NPT 64
#define Fs  1280
#endif

#ifdef NPT_256
#define NPT 256
#define Fs 6400
#endif

#ifdef NPT_1024
#define NPT 1024
#define Fs 5120
#endif



struct DATA_struct DATA = { { { 0 }, { 0 } },
{ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
{ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
{ 0, 0, 0 }, { 0, 0, 0 } };
unsigned short int BUFFER_SIZE = Def_BUFFER_SIZE, BUFFER_SIZE_tmp = Def_BUFFER_SIZE;

const unsigned int COE_45Hz[BUFFER_SIZE_45Hz] = {
	1, 4, 10, 20, 35, 56, 84, 120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771,	2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495, 4960, 5456, 5984, 6545, 7140, 7770, 8436, 9139,
	9880, 10660, 11480, 12341, 13244, 14190, 15180, 16215, 17296, 18424, 19600, 20825, 22100,23426, 24804, 26235, 27720, 29260, 30856, 32509, 34220, 35990, 37820, 39711, 41664, 43680,
	45760, 47905, 50116, 52394, 54740, 57155, 59640, 62196, 64824, 67525, 70300, 73150, 76076,79079, 82160, 85320, 88560, 91881, 95284, 98770, 102340, 105995, 109736, 113564, 117480,
	121485, 125576, 129750, 134004, 138335, 142740, 147216, 151760, 156369, 161040, 165770,170556, 175395, 180284, 185220, 190200, 195221, 200280, 205374, 210500, 215655, 220836,226040, 231264, 236505, 241760, 247026, 252300, 257579, 262860, 268140, 273416, 278685,
	283944, 289190, 294420, 299631, 304820, 309984, 315120, 320225, 325296, 330330, 335324,340275, 345180, 350036, 354840, 359589, 364280, 368910, 373476, 377975, 382404, 386760,
	391040, 395241, 399360, 403394, 407340, 411195, 414956, 418620, 422184, 425645, 429000,	432246, 435380, 438399, 441300, 444080, 446736, 449265, 451664, 453930, 456060, 458051,	459900, 461604, 463160, 464565, 465816, 466910, 467844, 468615, 469220, 469656, 469920,
	470009, 469920, 469656, 469220, 468615, 467844, 466910, 465816, 464565, 463160, 461604,	459900, 458051, 456060, 453930, 451664, 449265, 446736, 444080, 441300, 438399, 435380,
	432246, 429000, 425645, 422184, 418620, 414956, 411195, 407340, 403394, 399360, 395241,
	391040, 386760, 382404, 377975, 373476, 368910, 364280, 359589, 354840, 350036, 345180,
	340275, 335324, 330330, 325296, 320225, 315120, 309984, 304820, 299631, 294420, 289190,
	283944, 278685, 273416, 268140, 262860, 257579, 252300, 247026, 241760, 236505, 231264,
	226040, 220836, 215655, 210500, 205374, 200280, 195221, 190200, 185220, 180284, 175395,
	170556, 165770, 161040, 156369, 151760, 147216, 142740, 138335, 134004, 129750, 125576,
	121485, 117480, 113564, 109736, 105995, 102340, 98770, 95284, 91881, 88560, 85320, 82160,
	79079, 76076, 73150, 70300, 67525, 64824, 62196, 59640, 57155, 54740, 52394, 50116, 47905,
	45760, 43680, 41664, 39711, 37820, 35990, 34220, 32509, 30856, 29260, 27720, 26235, 24804,
	23426, 22100, 20825, 19600, 18424, 17296, 16215, 15180, 14190, 13244, 12341, 11480, 10660,
	9880, 9139, 8436, 7770, 7140, 6545, 5984, 5456, 4960, 4495, 4060, 3654, 3276, 2925, 2600,
	2300, 2024, 1771, 1540, 1330, 1140, 969, 816, 680, 560, 455, 364, 286, 220, 165, 120, 84, 56,
	35, 20, 10, 4, 1
};
const unsigned int COE_50Hz[BUFFER_SIZE_50Hz] = {
	1, 4, 10, 20, 35, 56, 84, 120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771,
	2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495, 4960, 5456, 5984, 6545, 7140, 7770, 8436, 9139,
	9880, 10660, 11480, 12341, 13244, 14190, 15180, 16215, 17296, 18424, 19600, 20825, 22100,
	23426, 24804, 26235, 27720, 29260, 30856, 32509, 34220, 35990, 37820, 39711, 41664, 43680,
	45760, 47905, 50116, 52394, 54740, 57155, 59640, 62196, 64824, 67525, 70300, 73150, 76076,
	79079, 82160, 85320, 88560, 91877, 95268, 98730, 102260, 105855, 109512, 113228, 117000,
	120825, 124700, 128622, 132588, 136595, 140640, 144720, 148832, 152973, 157140, 161330,
	165540, 169767, 174008, 178260, 182520, 186785, 191052, 195318, 199580, 203835, 208080,
	212312, 216528, 220725, 224900, 229050, 233172, 237263, 241320, 245340, 249320, 253257,
	257148, 260990, 264780, 268515, 272192, 275808, 279360, 282845, 286260, 289602, 292868,
	296055, 299160, 302180, 305112, 307953, 310700, 313350, 315900, 318347, 320688, 322920,
	325040, 327045, 328932, 330698, 332340, 333855, 335240, 336492, 337608, 338585, 339420,
	340110, 340652, 341043, 341280, 341360, 341280, 341043, 340652, 340110, 339420, 338585,
	337608, 336492, 335240, 333855, 332340, 330698, 328932, 327045, 325040, 322920, 320688,
	318347, 315900, 313350, 310700, 307953, 305112, 302180, 299160, 296055, 292868, 289602,
	286260, 282845, 279360, 275808, 272192, 268515, 264780, 260990, 257148, 253257, 249320,
	245340, 241320, 237263, 233172, 229050, 224900, 220725, 216528, 212312, 208080, 203835,
	199580, 195318, 191052, 186785, 182520, 178260, 174008, 169767, 165540, 161330, 157140,
	152973, 148832, 144720, 140640, 136595, 132588, 128622, 124700, 120825, 117000, 113228,
	109512, 105855, 102260, 98730, 95268, 91877, 88560, 85320, 82160, 79079, 76076, 73150,
	70300, 67525, 64824, 62196, 59640, 57155, 54740, 52394, 50116, 47905, 45760, 43680, 41664,
	39711, 37820, 35990, 34220, 32509, 30856, 29260, 27720, 26235, 24804, 23426, 22100, 20825,
	19600, 18424, 17296, 16215, 15180, 14190, 13244, 12341, 11480, 10660, 9880, 9139, 8436,
	7770, 7140, 6545, 5984, 5456, 4960, 4495, 4060, 3654, 3276, 2925, 2600, 2300, 2024, 1771,
	1540, 1330, 1140, 969, 816, 680, 560, 455, 364, 286, 220, 165, 120, 84, 56, 35, 20, 10, 4, 1
};
const unsigned int COE_55Hz[BUFFER_SIZE_55Hz] = {
	1, 4, 10, 20, 35, 56, 84, 120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771,
	2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495, 4960, 5456, 5984, 6545, 7140, 7770, 8436,
	9139, 9880, 10660, 11480, 12341, 13244, 14190, 15180, 16215, 17296, 18424, 19600, 20825,
	22100, 23426, 24804, 26235, 27720, 29260, 30856, 32509, 34220, 35990, 37820, 39711, 41664,
	43680, 45760, 47905, 50116, 52394, 54740, 57155, 59640, 62196, 64824, 67525, 70296, 73134,
	76036, 78999, 82020, 85096, 88224, 91401, 94624, 97890, 101196, 104539, 107916, 111324,
	114760, 118221, 121704, 125206, 128724, 132255, 135796, 139344, 142896, 146449, 150000,
	153546, 157084, 160611, 164124, 167620, 171096, 174549, 177976, 181374, 184740, 188071,
	191364, 194616, 197824, 200985, 204096, 207154, 210156, 213099, 215980, 218796, 221544,
	224221, 226824, 229350, 231796, 234159, 236436, 238624, 240720, 242721, 244624, 246426,
	248124, 249715, 251196, 252564, 253816, 254949, 255960, 256846, 257604, 258231, 258724,
	259080, 259296, 259369, 259296, 259080, 258724, 258231, 257604, 256846, 255960, 254949,
	253816, 252564, 251196, 249715, 248124, 246426, 244624, 242721, 240720, 238624, 236436,
	234159, 231796, 229350, 226824, 224221, 221544, 218796, 215980, 213099, 210156, 207154,
	204096, 200985, 197824, 194616, 191364, 188071, 184740, 181374, 177976, 174549, 171096,
	167620, 164124, 160611, 157084, 153546, 150000, 146449, 142896, 139344, 135796, 132255,
	128724, 125206, 121704, 118221, 114760, 111324, 107916, 104539, 101196, 97890, 94624,
	91401, 88224, 85096, 82020, 78999, 76036, 73134, 70296, 67525, 64824, 62196, 59640, 57155,
	54740, 52394, 50116, 47905, 45760, 43680, 41664, 39711, 37820, 35990, 34220, 32509, 30856,
	29260, 27720, 26235, 24804, 23426, 22100, 20825, 19600, 18424, 17296, 16215, 15180, 14190,
	13244, 12341, 11480, 10660, 9880, 9139, 8436, 7770, 7140, 6545, 5984, 5456, 4960, 4495, 4060,
	3654, 3276, 2925, 2600, 2300, 2024, 1771, 1540, 1330, 1140, 969, 816, 680, 560, 455, 364, 286,
	220, 165, 120, 84, 56, 35, 20, 10, 4, 1
};
const unsigned int COE_60Hz[BUFFER_SIZE_60Hz] = {
	1, 4, 10, 20, 35, 56, 84, 120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771,
	2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495, 4960, 5456, 5984, 6545, 7140, 7770, 8436, 9139,
	9880, 10660, 11480, 12341, 13244, 14190, 15180, 16215, 17296, 18424, 19600, 20825, 22100,
	23426, 24804, 26235, 27720, 29260, 30856, 32509, 34220, 35990, 37820, 39711, 41664, 43680,
	45760, 47905, 50116, 52394, 54736, 57139, 59600, 62116, 64684, 67301, 69964, 72670, 75416,
	78199, 81016, 83864, 86740, 89641, 92564, 95506, 98464, 101435, 104416, 107404, 110396,
	113389, 116380, 119366, 122344, 125311, 128264, 131200, 134116, 137009, 139876, 142714,
	145520, 148291, 151024, 153716, 156364, 158965, 161516, 164014, 166456, 168839, 171160,
	173416, 175604, 177721, 179764, 181730, 183616, 185419, 187136, 188764, 190300, 191741,
	193084, 194326, 195464, 196495, 197416, 198224, 198916, 199489, 199940, 200266, 200464,
	200531, 200464, 200266, 199940, 199489, 198916, 198224, 197416, 196495, 195464, 194326,
	193084, 191741, 190300, 188764, 187136, 185419, 183616, 181730, 179764, 177721, 175604,
	173416, 171160, 168839, 166456, 164014, 161516, 158965, 156364, 153716, 151024, 148291,
	145520, 142714, 139876, 137009, 134116, 131200, 128264, 125311, 122344, 119366, 116380,
	113389, 110396, 107404, 104416, 101435, 98464, 95506, 92564, 89641, 86740, 83864, 81016,
	78199, 75416, 72670, 69964, 67301, 64684, 62116, 59600, 57139, 54736, 52394, 50116, 47905,
	45760, 43680, 41664, 39711, 37820, 35990, 34220, 32509, 30856, 29260, 27720, 26235, 24804,
	23426, 22100, 20825, 19600, 18424, 17296, 16215, 15180, 14190, 13244, 12341, 11480, 10660,
	9880, 9139, 8436, 7770, 7140, 6545, 5984, 5456, 4960, 4495, 4060, 3654, 3276, 2925, 2600,
	2300, 2024, 1771, 1540, 1330, 1140, 969, 816, 680, 560, 455, 364, 286, 220, 165, 120, 84, 56,
	35, 20, 10, 4, 1
};
const unsigned int COE_65Hz[BUFFER_SIZE_65Hz] = {
	1, 4, 10, 20, 35, 56, 84, 120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771,
	2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495, 4960, 5456, 5984, 6545, 7140, 7770, 8436,
	9139, 9880, 10660, 11480, 12341, 13244, 14190, 15180, 16215, 17296, 18424, 19600, 20825,
	22100, 23426, 24804, 26235, 27720, 29260, 30856, 32509, 34220, 35990, 37820, 39711, 41664,
	43676, 45744, 47865, 50036, 52254, 54516, 56819, 59160, 61536, 63944, 66381, 68844, 71330,
	73836, 76359, 78896, 81444, 84000, 86561, 89124, 91686, 94244, 96795, 99336, 101864,
	104376, 106869, 109340, 111786, 114204, 116591, 118944, 121260, 123536, 125769, 127956,
	130094, 132180, 134211, 136184, 138096, 139944, 141725, 143436, 145074, 146636, 148119,
	149520, 150836, 152064, 153201, 154244, 155190, 156036, 156779, 157416, 157944, 158360,
	158661, 158844, 158906, 158844, 158661, 158360, 157944, 157416, 156779, 156036, 155190,
	154244, 153201, 152064, 150836, 149520, 148119, 146636, 145074, 143436, 141725, 139944,
	138096, 136184, 134211, 132180, 130094, 127956, 125769, 123536, 121260, 118944, 116591,
	114204, 111786, 109340, 106869, 104376, 101864, 99336, 96795, 94244, 91686, 89124, 86561,
	84000, 81444, 78896, 76359, 73836, 71330, 68844, 66381, 63944, 61536, 59160, 56819, 54516,
	52254, 50036, 47865, 45744, 43676, 41664, 39711, 37820, 35990, 34220, 32509, 30856, 29260,
	27720, 26235, 24804, 23426, 22100, 20825, 19600, 18424, 17296, 16215, 15180, 14190, 13244,
	12341, 11480, 10660, 9880, 9139, 8436, 7770, 7140, 6545, 5984, 5456, 4960, 4495, 4060, 3654,
	3276, 2925, 2600, 2300, 2024, 1771, 1540, 1330, 1140, 969, 816, 680, 560, 455, 364, 286, 220,
	165, 120, 84, 56, 35, 20, 10, 4, 1
};
const unsigned int *QuasiSynchronousCoefficient = &COE_50Hz[0];//coefficient=COE_50Hz




/****Copy data and set flag***/
void CopyADBuffer(struct DATA_struct *DATA)
{
	unsigned short int data;

	for (data = 0; data < BUFFER_SIZE; data++)
	{
		DATA->BUFFER_Volt[data][0] = AD.BUFFER[data][0];
		DATA->BUFFER_Volt[data][1] = AD.BUFFER[data][2];
		DATA->BUFFER_Volt[data][2] = AD.BUFFER[data][4];

		DATA->BUFFER_Amp[data][0] = AD.BUFFER[data][1];
		DATA->BUFFER_Amp[data][1] = AD.BUFFER[data][3];
		DATA->BUFFER_Amp[data][2] = AD.BUFFER[data][5];
	}
	AD.FLAG = 0;//Buffer will not be used, clear  flag, important!

}

/**********************************************
*800mV:0x7FFF,
Vin:Vout=8000V:11V
Iin:Vout=125A:1V

*V =data/32767*0.8V/11*8000 (V)
*I =data/32767*0.8V/1*125 (A)
*
*const float SamplingCoefficient[CHANNEL_CNT]={
*  data/32767*0.8V/11*8000,data/32767*0.8V/11*8000,
*    data/32767*0.8V/11*8000,data/32767*0.8V/1*125,
*      data/32767*0.8V/1*125,data/32767*0.8V/1*125};
*
*************************************************/
const float SamplingCoefficient[CHANNEL_CNT] = {
	0.01775622369512563915469277571393, 0.01775622369512563915469277571393,
	0.01775622369512563915469277571393, 0.00305185094759971922971282082583,
	0.00305185094759971922971282082583, 0.00305185094759971922971282082583
};
float SamplingCoefficientOffsetVolt[CHANNEL_CNT / 2];
float SamplingCoefficientOffsetAmp[CHANNEL_CNT / 2][3];

/**********************************************
*E=f(U*I)
*E = SamplingCoefficient[U]*SamplingCoefficient[I];
*
*const float PowerCoefficient[CHANNEL_CNT/2]={
*SamplingCoefficientU*SamplingCoefficientI,
*  SamplingCoefficientU*SamplingCoefficientI,
*    SamplingCoefficientU*SamplingCoefficientI};
*
*#define PowerCoefficient 215/59056
*
**********************************************/
const float PowerCoefficient[CHANNEL_CNT / 2] = {
	5.4189348109761769935278712466597e-5,
	5.4189348109761769935278712466597e-5,
	5.4189348109761769935278712466597e-5
};
float PowerCoefficientOffset[CHANNEL_CNT / 2];












void IIRlpf_FRQ(struct DATA_struct *DATA)
{
	unsigned short int data = 0;
	unsigned char channel = 0;

	static float IIRlpf_FRQ_out[3], IIRlpf_FRQ_in[3];
	short int OUT_BUFFER[Def_BUFFER_SIZE];
	float IIRlpf_FRQ_tmp;
	unsigned char i;

	static unsigned char Zero_CNT_Flag[CHANNEL_CNT / 2];      /*flag when counting*/
	static unsigned char Zero_CNT[CHANNEL_CNT / 2];           /*count the number of zero*/
	static unsigned short int Zero_TIM[CHANNEL_CNT / 2];      /*count the number of Time*/
	static unsigned short int Zero_TIM_tmp[CHANNEL_CNT / 2];
	static short int Pre_Last_data[CHANNEL_CNT / 2];          /*store data betweet two cycle*/

	/******************************IIR For Frequency*****************************/
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		/*check if zero is betweet two cycle*/
		if (Pre_Last_data[channel] < 0 && OUT_BUFFER[0] >= 0)
		{
			if (Zero_CNT_Flag[channel] == 0)
			{
				Zero_CNT_Flag[channel] = 1;/*Start to count zero*/
				Zero_CNT[channel] = 0;
				Zero_TIM_tmp[channel] = 0;
				Zero_TIM[channel] = 0;
			}
			else
			{
				/*counting number of zero*/
				Zero_CNT[channel]++;
				Zero_TIM[channel] = ++Zero_TIM_tmp[channel];
			}
		}
		else
		{
			/*Add up the time unit*/
			Zero_TIM_tmp[channel]++;
		}

		/*store data for the next cycle*/
		Pre_Last_data[channel] = OUT_BUFFER[BUFFER_SIZE - 1];

		/*Check zero points in the buffer*/
		for (data = 1; data < BUFFER_SIZE; data++)
		{
			if (OUT_BUFFER[data] < 0 && OUT_BUFFER[data - 1] >= 0)
			{
				if (Zero_CNT_Flag[channel] == 0)
				{
					/*Start to count zero*/
					Zero_CNT_Flag[channel] = 1;
					Zero_CNT[channel] = 0;
					Zero_TIM[channel] = 0;
					Zero_TIM_tmp[channel] = 0;
				}
				else
				{
					/*counting number of zero*/
					Zero_CNT[channel]++;
					Zero_TIM[channel] = ++Zero_TIM_tmp[channel];
				}
			}
			else
				/*Add up the time unit*/
				Zero_TIM_tmp[channel]++;
		}

		if (Zero_CNT[channel] >= 50)
		{
			/*when count is enough at one peroid data,updata the result*/
			/* 1.0*Zero_CNT[channel]/((Zero_TIM[channel])*0.00025); //compute length function*/
			DATA->FRQ[channel] = 4000.0*Zero_CNT[channel] / (Zero_TIM[channel]);

			/*reset for next cycle*/
			Zero_CNT_Flag[channel] = 0;
		}
	}
}











/*******************************************************************************
* Function      :quasi-synchronous sampling for RMS of U and I
*               Normal
*                       RMS=Sqrt((DATA[i]^2)/M)
*               quasi-synchronous
*                       RMS=Sqrt(∑(DATA[i]^2*Coe[i])/pow(M,CYCLE_CNT))
* Author	: Michael Ting
*******************************************************************************/
void Quasi_RMS(struct DATA_struct *DATA)
{
	unsigned short int data = 0;
	unsigned char channel = 0;

	unsigned long long RMS_tmp[2];

	/*Voit*/
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		RMS_tmp[0] = 0;
		RMS_tmp[1] = 0;

		for (data = 0; data < BUFFER_SIZE; data++)
		{
			//DATA->BUFFER[data][channel]=50*1.414213562373095*cos(2*3.1415926/80*data);
			RMS_tmp[0] = DATA->BUFFER_Volt[data][channel] * DATA->BUFFER_Volt[data][channel];
			RMS_tmp[1] += RMS_tmp[0] * QuasiSynchronousCoefficient[data];
		}/*must  sepreat in two step or will overflow*/

		//DATA->RMS[channel]=sqrt(RMS_tmp/(pow(80,CYCLE_CNT)));
		//DATA->RMS[channel]=sqrt(RMS_tmp[1]/40960000);
		//DATA->RMS[channel]=sqrt(RMS_tmp[1])/6400.0;

		//DATA->RMS[channel]*=SamplingCoefficient[channel];

		DATA->RMS_Volt[channel] = sqrt(RMS_tmp[1] / 40960000.0)*SamplingCoefficientOffsetVolt[channel];
		//DATA->RMS[channel]=sqrt((float)RMS_tmp[1]/40960000.0)*SamplingCoefficientOffset[channel];
	}

	/*Amp*/
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		RMS_tmp[0] = 0;
		RMS_tmp[1] = 0;

		for (data = 0; data < BUFFER_SIZE; data++)
		{
			//DATA->BUFFER[data][channel]=50*1.414213562373095*cos(2*3.1415926/80*data);
			RMS_tmp[0] = DATA->BUFFER_Amp[data][channel] * DATA->BUFFER_Amp[data][channel];
			RMS_tmp[1] += RMS_tmp[0] * QuasiSynchronousCoefficient[data];
		}/*must  sepreat in two step or will overflow*/

		if (DATA->RMS_Amp[channel] < 1)
			DATA->RMS_Amp[channel] = sqrt(RMS_tmp[1] / 40960000.0)*SamplingCoefficientOffsetAmp[channel][0];
		else if (DATA->RMS_Amp[channel] < 10)
			DATA->RMS_Amp[channel] = sqrt(RMS_tmp[1] / 40960000.0)*SamplingCoefficientOffsetAmp[channel][1];
		else
			DATA->RMS_Amp[channel] = sqrt(RMS_tmp[1] / 40960000.0)*SamplingCoefficientOffsetAmp[channel][2];

		//DATA->RMS[channel]=sqrt((float)RMS_tmp[1]/40960000.0)*SamplingCoefficientOffset[channel];
	}
}




/*******************************************************************************
*
*M for numerator Order,and N for denominator Order
*
*******************************************************************************/
const unsigned char Power_IIRI2R_M = 2, Power_IIRI2R_N = 2;
const float Power_IIRI2R_Numerator[3] = { 0.0000098259168204820344,
0.000019651833640964069,
0.0000098259168204820344 };
const float Power_IIR_denominator[3] = { 1,
-1.9911142922016536,
0.99115359586893537 };
/*******************************************************************************
* Function      :Calculate Active and Reactive Power RMS IIR Low frequency pass
*                Filter and Plesiochronous calculations RMS
*               P=UI*Cosφ
*               Normal  有功功率----又叫平均功率
*                       P=∑(U(i)*I(i))/M,
*               quasi-synchronous
*                       P=∑(U[i]*I[i]*Coe[i])/pow(M,CYCLE_CNT)
* Author        : Michael Ting
*******************************************************************************/
/*too large,if in function may be out of stack when use other function as sqrt()*/
void IIRlpf_Quasi_Power(struct DATA_struct *DATA)
{
	unsigned short int data = 0;
	unsigned char channel = 0;

	//static float Power_IIRlpf_out[3][CHANNEL_CNT],Power_IIRlpf_in[3][CHANNEL_CNT];
	//float Power_IIR_tmp;
	//unsigned char i; 

	/*****************************Active Power***********************************/
	long long Power_tmp[2];
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		Power_tmp[0] = 0;
		Power_tmp[1] = 0;
		for (data = 0; data < BUFFER_SIZE; data++)
		{
			/*32bit 7FFF*7FFF=7FFF 0001<1 0000 0000*/
			Power_tmp[0] =
				DATA->BUFFER_Volt[data][channel] * DATA->BUFFER_Amp[data][channel];
			Power_tmp[1] += Power_tmp[0] * QuasiSynchronousCoefficient[data];
		}
		DATA->W[channel] = Power_tmp[1] / 40960000.0*PowerCoefficientOffset[channel];
	}


	/***************************Reactive Power***********************************/
	static unsigned char PhaseOffset = Typ_PHASE_OFFSET;
	static short int PreLastBuffer[MAX_PHASE_OFFSET][CHANNEL_CNT / 2];

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		Power_tmp[0] = 0;
		Power_tmp[1] = 0;

		/*Use the last  data of U the compute the I*/
		for (data = 0; data < PhaseOffset; data++)
		{
			Power_tmp[0] = PreLastBuffer[data][channel]
				* DATA->BUFFER_Amp[data][channel];
			Power_tmp[1] += Power_tmp[0] * QuasiSynchronousCoefficient[data];
		}

		for (data = PhaseOffset; data < BUFFER_SIZE; data++)
		{
			Power_tmp[0] = DATA->BUFFER_Volt[data - PhaseOffset][channel]
				* DATA->BUFFER_Amp[data][channel];
			Power_tmp[1] += Power_tmp[0] * QuasiSynchronousCoefficient[data];
		}

		/*store the latest data of U for the next cycle*/
		for (data = 0; data < PhaseOffset; data++)
		{
			PreLastBuffer[data][channel] = DATA->BUFFER_Volt[BUFFER_SIZE - PhaseOffset][channel];
		}

		DATA->VAR[channel] = Power_tmp[1] / 40960000.0*PowerCoefficientOffset[channel];
	}


	//#define USE_RMS
	//#define USE_Quasi_RMS
	//#define USE_Int_RMS
#ifdef USE_RMS  
	/*
	unsigned long long Power_tmp[2];
	for(channel=0;channel<CHANNEL_CNT/2;channel++)
	{
	Power_tmp[0]=0;
	Power_tmp[1]=0;
	for(data=0;data<BUFFER_SIZE;data++)
	{
	//DATA->BUFFER[data][channel]=50*1.414213562373095*cos(2*3.1415926/80*data);
	Power_tmp[0]=POWER_BUFFER[data][channel]*POWER_BUFFER[data][channel];
	Power_tmp[1]+=Power_tmp[0]*QuasiSynchronousCoefficient[data];
	}

	//DATA->W[channel]=sqrt(Power_tmp/(pow(80,CYCLE_CNT)));
	//DATA->W[channel]=sqrt(Power_tmp[1]/40960000);
	//DATA->W[channel]=sqrt(Power_tmp[1])/6400.0;

	//DATA->W[channel]*=SamplingCoefficient[channel];

	DATA->W[channel]=sqrt(Power_tmp[1]/40960000)*SamplingCoefficient[channel];
	}
	*/

	/*
	unsigned long long Power_tmpE;
	double Power_tmpF;
	for(channel=0;channel<CHANNEL_CNT/2;channel++)
	{
	Power_tmpF=0;
	Power_tmpE=0;
	for(data=0;data<BUFFER_SIZE;data++)
	{
	Power_tmpE=POWER_BUFFER[data][channel]*POWER_BUFFER[data][channel];
	Power_tmpF+=Power_tmpE;
	//printf("rn %f",Power_tmpF);
	}
	DATA->W[channel]=sqrt(Power_tmpF/BUFFER_SIZE);

	DATA->W[channel]*=PowerCoefficient[channel];

	//DATA->W[channel]=sqrt(Power_tmpF)/6400.0*PowerCoefficient[channel];
	}
	*/
	unsigned long long Power_tmpE;
	float Power_tmpF, Power_tmpG;

	for (channel = 0; channel < CHANNEL_CNT; channel++)
	{
		Power_tmpE = 0;
		Power_tmpF = 0;
		for (data = 0; data < BUFFER_SIZE; data++)
		{
			Power_tmpE = POWER_BUFFER[data][channel] * POWER_BUFFER[data][channel];
			//Power_tmpG=Power_tmpE*QuasiSynchronousCoefficient[data];
			Power_tmpF += Power_tmpE;
		}
		DATA->W[channel] = sqrt(Power_tmpF / BUFFER_SIZE)*PowerCoefficientOffset[channel];
	}

	/*IIR to  get Reactive  power
	for(channel=0;channel<CHANNEL_CNT/2;channel++)
	{
	for(data=0;data<BUFFER_SIZE;data++)
	{
	Power_IIRlpf_in[0][channel]=POWER_BUFFER[data][channel];

	Power_IIR_tmp=0;
	for(i=0;i<=Power_IIRI2R_M;i++)
	{
	Power_IIR_tmp+=Power_IIRI2R_Numerator[i]*Power_IIRlpf_in[i][channel];
	}
	for(i=1;i<=Power_IIRI2R_N;i++)
	{
	Power_IIR_tmp-=Power_IIR_denominator[i]*Power_IIRlpf_out[i][channel];
	}

	POWER_BUFFER[data][channel]=(short int)Power_IIR_tmp;//data updated

	for(i=Power_IIRI2R_M;i>=1;i--)
	{
	Power_IIRlpf_in[i][channel]=Power_IIRlpf_in[i-1][channel];
	}
	for(i=Power_IIRI2R_N;i>=2;i--)
	{
	Power_IIRlpf_out[i][channel]=Power_IIRlpf_out[i-1][channel];
	}//for the next

	Power_IIRlpf_out[1][channel]=POWER_BUFFER[data][channel];
	}
	}
	*/

	/*Calculate RMS of  reactive power*/

#elif defined USE_Quasi_RMS
	/*
	unsigned long long Power_tmpE;
	float Power_tmpF,Power_tmpG;

	for(channel=0;channel<CHANNEL_CNT;channel++)
	{
	Power_tmpE=0;
	Power_tmpF=0;
	for(data=0;data<BUFFER_SIZE;data++)
	{
	Power_tmpE=POWER_BUFFER[data][channel]*POWER_BUFFER[data][channel];
	Power_tmpG=Power_tmpE*QuasiSynchronousCoefficient[data];
	Power_tmpF+=Power_tmpG;
	}
	DATA->W[channel]=sqrt(Power_tmpF/40960000)*PowerCoefficient[channel];
	}
	*/

	unsigned long long Power_tmpE;
	float Power_tmpF, Power_tmpG;

	for (channel = 0; channel < CHANNEL_CNT; channel++)
	{
		Power_tmpE = 0;
		Power_tmpF = 0;
		for (data = 0; data < BUFFER_SIZE; data++)
		{
			Power_tmpE += POWER_BUFFER[data][channel];

			Power_tmpG = Power_tmpE*QuasiSynchronousCoefficient[data];
			Power_tmpF += Power_tmpG;
		}
		DATA->W[channel] = sqrt(Power_tmpF / 40960000)*PowerCoefficientOffset[channel];
	}

#elif defined USE_Int_RMS
	unsigned long long Power_tmp;
	unsigned long long Power_tmpH;
	unsigned long long Power_tmpL;
	double W_tmp;

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		//Power_tmp=0;/*result of (U*I)^2 */
		Power_tmpL = 0;/*low 32 bit of (U*I)^2*/
		Power_tmpH = 0;/*High 32 bit of (U*I)^2*/
		//W_tmp=0;/*High 32 bit*/

		for (data = 0; data < BUFFER_SIZE; data++)
		{
			Power_tmp = POWER_BUFFER[data][channel] * POWER_BUFFER[data][channel];
			/*(U*I)^2*Max_Ccoe_470009>2^64 ; sepreate in two part*/

			//Power_tmpL_tmp=Power_tmp&&0xFFFFFFFF;
			//Power_tmpH_tmp=Power_tmp>>32;
			//Power_tmpL+=Power_tmpL_tmp*QuasiSynchronousCoefficient[data];
			//Power_tmpH+=Power_tmpH_tmp*QuasiSynchronousCoefficient[data];

			//Power_tmpH=(Power_tmp>>32);
			//Power_tmpL=(Power_tmp&0xFFFFFFFF);

			Power_tmpH += (Power_tmp >> 32)*QuasiSynchronousCoefficient[data];
			Power_tmpL += (Power_tmp & 0xFFFFFFFF)*QuasiSynchronousCoefficient[data];

			//printf("rn %llx  %llx  %llx",Power_tmp,Power_tmpH,Power_tmpL);
		}
		/*Power_tmp=Power_tmpL/40960000+(Power_tmpH<<32)/40960000.0*/
		//W_tmp=(float)Power_tmpL/40960000.0+(float)Power_tmpH*104.8576;

		W_tmp = (float)(Power_tmpL / 40960000.0 + Power_tmpH*104.8576);

		//Power_tmp=Power_tmpL/40960000+(Power_tmpH/40960000)<<32;
		//Power_tmp=Power_tmpH*104+Power_tmpL/40960000;

		DATA->W[channel] = sqrt(W_tmp)*PowerCoefficientOffset[channel];
		//DATA->W[channel]=Power_tmp;
		//DATA->W[channel]=sqrt(100)*PowerCoefficient[channel];
	}
#endif 
}


/*******************************************************************************
* Function      :Calculate Apparent Power and Power Factor
*               S=Urms*Irms or S=sqrt(P^2+Q^2)  V*mA
*               COSφ=P/S                        0~1
* Author        : Michael Ting
*******************************************************************************/
void SecondaryParameter(struct DATA_struct *DATA)
{
	unsigned char channel;

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		/**Calculate Apparent Power S**/
#define USE_UI_S
#ifdef USE_UI_S
		DATA->VA[channel] = DATA->RMS_Volt[channel] * DATA->RMS_Amp[channel];
#elif defined USE_PQ_S
		DATA->VA[channel] =
			sqrt(DATA->W[channel] * DATA->W[channel]
			+ DATA->VAR[channel] * DATA->VAR[channel]);
#endif

		/**Calculate Power Factor PF**/
		DATA->PF[channel] = DATA->W[channel] / DATA->VA[channel];
	}
}




/*******************************************************************************
*
*E =dataU*0.8V/32767*(500K+820)/820 * dataI*800mV/32767*(1000/100) (V*mA=mW)
*       =SamplingCoefficient[U]*SamplingCoefficient[I];
*
*Sampling Frequency: 1000KHz
*T0=1/1000KHz = >>10 *(1/1000)
*
*U/32767*0.8V*(500K+820)/820 * I/32767*800*(1000/100) * (BUFFERSIZE-1)*T0
*
*
*******************************************************************************/
//const float ElectronicPowerCoefficient=>>10/1000.0
/*******************************************************************************
* Function      :Calculate Electronic Power
*               W=∑(Ui*Ii*t)=(∑(Ui*Ii))*T
*               Vr=∑(U(i-T/4)*Ii*t)=(∑(U(i-t/4)*Ii))*T
* Author        : Michael Ting
*******************************************************************************/
void ElectronicPower(struct DATA_struct *DATA)
{
	unsigned char channel;

#define USE_QUADRANTS
	//#define USE_PUIT_QUIT
	//#define USE_PT_QT

#ifdef USE_QUADRANTS
	unsigned short int data = 0;
	int ElectronicPower_tmp;
	unsigned int Positive_tmp;
	signed int Negative_tmp;
	//int Positive_tmp,Negative_tmp;
	unsigned short int Positive_TIM, Negative_TIM;

	static unsigned char PhaseOffset = Typ_PHASE_OFFSET;
	static short int PreLastBuffer[MAX_PHASE_OFFSET][CHANNEL_CNT / 2];

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		/*Add up P*/
		//ElectronicPower_tmp=0;
		Positive_tmp = 0; Negative_tmp = 0;
		Positive_TIM = 0; Negative_TIM = 0;
		for (data = 0; data < BUFFER_SIZE; data++)/*Can share the data in a nother function*/
		{
			ElectronicPower_tmp =
				DATA->BUFFER_Volt[data][channel] * DATA->BUFFER_Amp[data][channel];

			if (ElectronicPower_tmp)
			{
				Positive_tmp += ElectronicPower_tmp;
				Positive_TIM++;
			}
			else
			{
				Negative_tmp += ElectronicPower_tmp;
				Negative_TIM++;
			}
		}
		DATA->P_POWER[channel][0] += ((Positive_tmp*Positive_TIM) >> 10) / 1000.0;
		DATA->P_POWER[channel][1] += ((Negative_tmp*Negative_TIM) >> 10) / 1000.0;
		//DATA->P_POWER[channel][0]+=(Positive_tmp*Positive_TIM)/1000000.0;
		//DATA->P_POWER[channel][1]+=(Negative_tmp*Negative_TIM)/1000000.0;

		DATA->P[channel] =
			DATA->P_POWER[channel][0] + DATA->P_POWER[channel][1];


		/*Add up Q*/
		//ElectronicPower_tmp=0;
		Positive_tmp = 0; Negative_tmp = 0;
		Positive_TIM = 0; Negative_TIM = 0;
		/*Use the last  data of U and the I*/
		for (data = 0; data < PhaseOffset; data++)
		{
			ElectronicPower_tmp =
				PreLastBuffer[data][channel] * DATA->BUFFER_Amp[data][channel];

			if (ElectronicPower_tmp)
			{
				Positive_tmp += ElectronicPower_tmp;
				Positive_TIM++;
			}
			else
			{
				Negative_tmp += ElectronicPower_tmp;
				Negative_TIM++;
			}
		}

		for (data = PhaseOffset; data < BUFFER_SIZE; data++)
		{
			ElectronicPower_tmp =
				DATA->BUFFER_Volt[data - PhaseOffset][channel] * DATA->BUFFER_Amp[data][channel];

			if (ElectronicPower_tmp)
			{
				Positive_tmp += ElectronicPower_tmp;
				Positive_TIM++;
			}
			else
			{
				Negative_tmp += ElectronicPower_tmp;
				Negative_TIM++;
			}
		}

		/*store the latest data of U for the next cycle*/
		for (data = 0; data < PhaseOffset; data++)
		{
			PreLastBuffer[data][channel] = DATA->BUFFER_Volt[BUFFER_SIZE - PhaseOffset][channel];
		}

		DATA->Q_POWER[channel][0] += ((Positive_tmp*Positive_TIM) >> 10) / 1000.0;
		DATA->Q_POWER[channel][1] += ((Negative_tmp*Negative_TIM) >> 10) / 1000.0;

		DATA->Q[channel] =
			DATA->Q_POWER[channel][0] + DATA->Q_POWER[channel][1];
	}

#elif defined USE_PUIT_QUIT
	unsigned short int data = 0;
	int ElectronicPower_tmp;

	static unsigned char PhaseOffset = Typ_PHASE_OFFSET;
	static short int PreLastBuffer[MAX_PHASE_OFFSET][CHANNEL_CNT / 2];

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		/*Add up P*/
		ElectronicPower_tmp = 0;
		for (data = 0; data < BUFFER_SIZE; data++)/*Can share the data in a nother function*/
		{
			ElectronicPower_tmp +=
				DATA->BUFFER[data][channel] * DATA->BUFFER[data][channel + 3];
		}
		DATA->P_POWER[channel] += ((ElectronicPower_tmp*BUFFER_SIZE) >> 10) / 1000.0;


		/*Add Code to Add up Q*/
		ElectronicPower_tmp = 0;

		/*Use the last  data of U and the I*/
		for (data = 0; data < PhaseOffset; data++)
		{
			ElectronicPower_tmp +=
				PreLastBuffer[data][channel] * DATA->BUFFER[data][channel + 3];
		}

		for (data = PhaseOffset; data < BUFFER_SIZE; data++)
		{
			ElectronicPower_tmp +=
				DATA->BUFFER[data - PhaseOffset][channel] * DATA->BUFFER[data][channel + 3];
		}

		/*store the latest data of U for the next cycle*/
		for (data = 0; data < PhaseOffset; data++)
		{
			PreLastBuffer[data][channel] = DATA->BUFFER[BUFFER_SIZE - PhaseOffset][channel];
		}

		DATA->Q_POWER[channel] += ((ElectronicPower_tmp*BUFFER_SIZE) >> 10) / 1000.0;
	}

#elif defined USE_PT_QT
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++)
	{
		//DATA->P_POWER[channel]+=DATA->W[channel]*(BUFFER_SIZE-1)/1000000.0;
		DATA->P_POWER[channel] += DATA->W[channel] * 0.000316;

		//DATA->Q_POWER[channel]+=DATA->VAR[channel]*(BUFFER_SIZE-1)/1000000.0;
		DATA->Q_POWER[channel] += DATA->VAR[channel] * 0.000316;
	}
#endif
}





/*******************************************************************************
* Function   :FFT Harmonic Process
*
* Author     : Michael Ting
*******************************************************************************/
#ifdef USE_DSP_LIBRARY
void FFT_Configuration(double *pr, double *pi, int n, int k, double*fr, double*fi, int l, int il)
{

}

void FFT_Harmonic(struct DATA_struct *DATA)
{

}
#endif




/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : Copy buff from AD to DATA
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
void _I2R_Memcpy_fast( uint32_t * to, uint32_t * from, uint32_t count)
{
if(count==0)
{
return ;
}
uint32_t n = (count + 7 ) / 8 ;
switch (count % 8 )
{
case 0 :
do { * to ++ = * from ++ ;
case 7 : * to ++ = * from ++ ;
case 6 : * to ++ = * from ++ ;
case 5 : * to ++ = * from ++ ;
case 4 : * to ++ = * from ++ ;
case 3 : * to ++ = * from ++ ;
case 2 : * to ++ = * from ++ ;
case 1 : * to ++ = * from ++ ;
} while ( -- n > 0 );
}
}
#include "data_prc.h"
*/



/*
#include <stm32f10x.h>
void DMA1_Channel2_IRQHandler()
{
if(DMA_GetITStatus(DMA1_IT_TC2))
{
DMA_GetCurrDataCounter(DMA1_Channel2);//Can be used to count
DMA_ClearITPendingBit(DMA1_IT_TC2);

unsigned short int data;
for(data=0;data<BUFFER_SIZE;data++)
{
DATA.BUFFER[data][0]=AD.BUFFER[data][0];
DATA.BUFFER[data][1]=AD.BUFFER[data][2];
DATA.BUFFER[data][2]=AD.BUFFER[data][4];
DATA.BUFFER[data][3]=AD.BUFFER[data][1];
DATA.BUFFER[data][4]=AD.BUFFER[data][3];
DATA.BUFFER[data][5]=AD.BUFFER[data][5];
}

//_I2R_Memcpy_fast(DATA.BUFFER, AD.BUFFER, BUFFER_SIZE*CHANNEL_CNT);
AD.FLAG=SET;
}
}
*/
