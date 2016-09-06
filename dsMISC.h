#ifndef DS_MISC_H
#define DS_MISC_H

#include "dsp.h"
#include <math.h>

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


const float SamplingCoefficient[CHANNEL_CNT] = {
	0.01775622369512563915469277571393, 0.01775622369512563915469277571393,
	0.01775622369512563915469277571393, 0.00305185094759971922971282082583,
	0.00305185094759971922971282082583, 0.00305185094759971922971282082583
};
float SamplingCoefficientOffsetVolt[CHANNEL_CNT / 2];
float SamplingCoefficientOffsetAmp[CHANNEL_CNT / 2][3];

/*
E=f(U*I)
E = SamplingCoefficient[U]*SamplingCoefficient[I];

const float PowerCoefficient[CHANNEL_CNT/2]={
SamplingCoefficientU*SamplingCoefficientI,
SamplingCoefficientU*SamplingCoefficientI,
SamplingCoefficientU*SamplingCoefficientI};

#define PowerCoefficient 215/59056
*/
const float PowerCoefficient[CHANNEL_CNT / 2] = {
	5.4189348109761769935278712466597e-5,
	5.4189348109761769935278712466597e-5,
	5.4189348109761769935278712466597e-5
};
float PowerCoefficientOffset[CHANNEL_CNT / 2];



void IIRlpf_FRQ(struct DATA_struct *DATA) {
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

	/*IIR For Frequency*/
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		/*check if zero is betweet two cycle*/
		if (Pre_Last_data[channel] < 0 && OUT_BUFFER[0] >= 0) {
			if (Zero_CNT_Flag[channel] == 0) {
				Zero_CNT_Flag[channel] = 1;/*Start to count zero*/
				Zero_CNT[channel] = 0;
				Zero_TIM_tmp[channel] = 0;
				Zero_TIM[channel] = 0;
			} else {
				/*counting number of zero*/
				Zero_CNT[channel]++;
				Zero_TIM[channel] = ++Zero_TIM_tmp[channel];
			}
		} else {
			/*Add up the time unit*/
			Zero_TIM_tmp[channel]++;
		}

		/*store data for the next cycle*/
		Pre_Last_data[channel] = OUT_BUFFER[BUFFER_SIZE - 1];

		/*Check zero points in the buffer*/
		for (data = 1; data < BUFFER_SIZE; data++) {
			if (OUT_BUFFER[data] < 0 && OUT_BUFFER[data - 1] >= 0) {
				if (Zero_CNT_Flag[channel] == 0) {
					/*Start to count zero*/
					Zero_CNT_Flag[channel] = 1;
					Zero_CNT[channel] = 0;
					Zero_TIM[channel] = 0;
					Zero_TIM_tmp[channel] = 0;
				} else {
					/*counting number of zero*/
					Zero_CNT[channel]++;
					Zero_TIM[channel] = ++Zero_TIM_tmp[channel];
				}
			} else
				/*Add up the time unit*/
				Zero_TIM_tmp[channel]++;
		}

		if (Zero_CNT[channel] >= 50) {
			/*when count is enough at one peroid data,updata the result*/
			/* 1.0*Zero_CNT[channel]/((Zero_TIM[channel])*0.00025); //compute length function*/
			DATA->FRQ[channel] = 4000.0*Zero_CNT[channel] / (Zero_TIM[channel]);

			/*reset for next cycle*/
			Zero_CNT_Flag[channel] = 0;
		}
	}
}




/*
quasi-synchronous sampling for RMS of U and I
Normal
RMS=Sqrt((DATA[i]^2)/M)
quasi-synchronous
RMS=Sqrt(∑(DATA[i]^2*Coe[i])/pow(M,CYCLE_CNT))
*/
void Quasi_RMS(struct DATA_struct *DATA) {
	unsigned short int data = 0;
	unsigned char channel = 0;

	unsigned long long RMS_tmp[2];

	/*Voit*/
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		RMS_tmp[0] = 0;
		RMS_tmp[1] = 0;

		for (data = 0; data < BUFFER_SIZE; data++) {
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
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		RMS_tmp[0] = 0;
		RMS_tmp[1] = 0;

		for (data = 0; data < BUFFER_SIZE; data++) {
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




/*M for numerator Order,and N for denominator Order*/
const unsigned char Power_IIRI2R_M = 2, Power_IIRI2R_N = 2;
const float Power_IIRI2R_Numerator[3] = { 0.0000098259168204820344,
0.000019651833640964069,
0.0000098259168204820344 };
const float Power_IIR_denominator[3] = { 1,
-1.9911142922016536,
0.99115359586893537 };
/*
Calculate Active and Reactive Power RMS IIR Low frequency pass
Filter and Plesiochronous calculations RMS
P=UI*Cosφ
Normal  有功功率----又叫平均功率
P=∑(U(i)*I(i))/M,
quasi-synchronous
P=∑(U[i]*I[i]*Coe[i])/pow(M,CYCLE_CNT)
*/
/*too large,if in function may be out of stack when use other function as sqrt()*/
void IIRlpf_Quasi_Power(struct DATA_struct *DATA) {
	unsigned short int data = 0;
	unsigned char channel = 0;

	//static float Power_IIRlpf_out[3][CHANNEL_CNT],Power_IIRlpf_in[3][CHANNEL_CNT];
	//float Power_IIR_tmp;
	//unsigned char i; 

	/*Active Power*/
	long long Power_tmp[2];
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		Power_tmp[0] = 0;
		Power_tmp[1] = 0;
		for (data = 0; data < BUFFER_SIZE; data++) {
			/*32bit 7FFF*7FFF=7FFF 0001<1 0000 0000*/
			Power_tmp[0] =
				DATA->BUFFER_Volt[data][channel] * DATA->BUFFER_Amp[data][channel];
			Power_tmp[1] += Power_tmp[0] * QuasiSynchronousCoefficient[data];
		}
		DATA->W[channel] = Power_tmp[1] / 40960000.0*PowerCoefficientOffset[channel];
	}


	/*Reactive Power*/
	static unsigned char PhaseOffset = Typ_PHASE_OFFSET;
	static short int PreLastBuffer[MAX_PHASE_OFFSET][CHANNEL_CNT / 2];

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		Power_tmp[0] = 0;
		Power_tmp[1] = 0;

		/*Use the last  data of U the compute the I*/
		for (data = 0; data < PhaseOffset; data++) {
			Power_tmp[0] = PreLastBuffer[data][channel]
				* DATA->BUFFER_Amp[data][channel];
			Power_tmp[1] += Power_tmp[0] * QuasiSynchronousCoefficient[data];
		}

		for (data = PhaseOffset; data < BUFFER_SIZE; data++) {
			Power_tmp[0] = DATA->BUFFER_Volt[data - PhaseOffset][channel]
				* DATA->BUFFER_Amp[data][channel];
			Power_tmp[1] += Power_tmp[0] * QuasiSynchronousCoefficient[data];
		}

		/*store the latest data of U for the next cycle*/
		for (data = 0; data < PhaseOffset; data++) {
			PreLastBuffer[data][channel] = DATA->BUFFER_Volt[BUFFER_SIZE - PhaseOffset][channel];
		}

		DATA->VAR[channel] = Power_tmp[1] / 40960000.0*PowerCoefficientOffset[channel];
	}


	//#define USE_RMS
	//#define USE_Quasi_RMS
	//#define USE_Int_RMS
#ifdef USE_RMS  
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

	unsigned long long Power_tmpE;
	float Power_tmpF, Power_tmpG;

	for (channel = 0; channel < CHANNEL_CNT; channel++) {
		Power_tmpE = 0;
		Power_tmpF = 0;
		for (data = 0; data < BUFFER_SIZE; data++) {
			Power_tmpE = POWER_BUFFER[data][channel] * POWER_BUFFER[data][channel];
			//Power_tmpG=Power_tmpE*QuasiSynchronousCoefficient[data];
			Power_tmpF += Power_tmpE;
		}
		DATA->W[channel] = sqrt(Power_tmpF / BUFFER_SIZE)*PowerCoefficientOffset[channel];
	}

	/*IIR to  get Reactive  power*/
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

	/*Calculate RMS of  reactive power*/

#elif defined USE_Quasi_RMS
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

	unsigned long long Power_tmpE;
	float Power_tmpF, Power_tmpG;

	for (channel = 0; channel < CHANNEL_CNT; channel++) {
		Power_tmpE = 0;
		Power_tmpF = 0;
		for (data = 0; data < BUFFER_SIZE; data++) {
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

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		//Power_tmp=0;/*result of (U*I)^2 */
		Power_tmpL = 0;/*low 32 bit of (U*I)^2*/
		Power_tmpH = 0;/*High 32 bit of (U*I)^2*/
		//W_tmp=0;/*High 32 bit*/

		for (data = 0; data < BUFFER_SIZE; data++) {
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


/*
Calculate Apparent Power and Power Factor
S=Urms*Irms or S=sqrt(P^2+Q^2)  V*mA
COSφ=P/S                        0~1
*/
void SecondaryParameter(struct DATA_struct *DATA) {
	unsigned char channel;

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
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




/*
E =dataU*0.8V/32767*(500K+820)/820 * dataI*800mV/32767*(1000/100) (V*mA=mW)
=SamplingCoefficient[U]*SamplingCoefficient[I];
Sampling Frequency: 1000KHz
T0=1/1000KHz = >>10 *(1/1000)
U/32767*0.8V*(500K+820)/820 * I/32767*800*(1000/100) * (BUFFERSIZE-1)*T0
*/
//const float ElectronicPowerCoefficient=>>10/1000.0
/*
Calculate Electronic Power
W=∑(Ui*Ii*t)=(∑(Ui*Ii))*T
Vr=∑(U(i-T/4)*Ii*t)=(∑(U(i-t/4)*Ii))*T
*/
void ElectronicPower(struct DATA_struct *DATA) {
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

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		/*Add up P*/
		//ElectronicPower_tmp=0;
		Positive_tmp = 0; Negative_tmp = 0;
		Positive_TIM = 0; Negative_TIM = 0;
		for (data = 0; data < BUFFER_SIZE; data++)/*Can share the data in a nother function*/
		{
			ElectronicPower_tmp =
				DATA->BUFFER_Volt[data][channel] * DATA->BUFFER_Amp[data][channel];

			if (ElectronicPower_tmp) {
				Positive_tmp += ElectronicPower_tmp;
				Positive_TIM++;
			} else {
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
		for (data = 0; data < PhaseOffset; data++) {
			ElectronicPower_tmp =
				PreLastBuffer[data][channel] * DATA->BUFFER_Amp[data][channel];

			if (ElectronicPower_tmp) {
				Positive_tmp += ElectronicPower_tmp;
				Positive_TIM++;
			} else {
				Negative_tmp += ElectronicPower_tmp;
				Negative_TIM++;
			}
		}

		for (data = PhaseOffset; data < BUFFER_SIZE; data++) {
			ElectronicPower_tmp =
				DATA->BUFFER_Volt[data - PhaseOffset][channel] * DATA->BUFFER_Amp[data][channel];

			if (ElectronicPower_tmp) {
				Positive_tmp += ElectronicPower_tmp;
				Positive_TIM++;
			} else {
				Negative_tmp += ElectronicPower_tmp;
				Negative_TIM++;
			}
		}

		/*store the latest data of U for the next cycle*/
		for (data = 0; data < PhaseOffset; data++) {
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

	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
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
		for (data = 0; data < PhaseOffset; data++) {
			ElectronicPower_tmp +=
				PreLastBuffer[data][channel] * DATA->BUFFER[data][channel + 3];
		}

		for (data = PhaseOffset; data < BUFFER_SIZE; data++) {
			ElectronicPower_tmp +=
				DATA->BUFFER[data - PhaseOffset][channel] * DATA->BUFFER[data][channel + 3];
		}

		/*store the latest data of U for the next cycle*/
		for (data = 0; data < PhaseOffset; data++) {
			PreLastBuffer[data][channel] = DATA->BUFFER[BUFFER_SIZE - PhaseOffset][channel];
		}

		DATA->Q_POWER[channel] += ((ElectronicPower_tmp*BUFFER_SIZE) >> 10) / 1000.0;
	}

#elif defined USE_PT_QT
	for (channel = 0; channel < CHANNEL_CNT / 2; channel++) {
		//DATA->P_POWER[channel]+=DATA->W[channel]*(BUFFER_SIZE-1)/1000000.0;
		DATA->P_POWER[channel] += DATA->W[channel] * 0.000316;

		//DATA->Q_POWER[channel]+=DATA->VAR[channel]*(BUFFER_SIZE-1)/1000000.0;
		DATA->Q_POWER[channel] += DATA->VAR[channel] * 0.000316;
	}
#endif
}


#ifdef USE_DSP_LIBRARY
void FFT_Configuration(double *pr, double *pi, int n, int k, double*fr, double*fi, int l, int il) {
}

void FFT_Harmonic(struct DATA_struct *DATA) {

}
#endif



void _I2R_Memcpy_fast(uint32_t * to, uint32_t * from, uint32_t count) {
	if (count == 0) {
		return;
	}
	uint32_t n = (count + 7) / 8;
	switch (count % 8) {
	case 0:
		do {
			*to++ = *from++;
	case 7: *to++ = *from++;
	case 6: *to++ = *from++;
	case 5: *to++ = *from++;
	case 4: *to++ = *from++;
	case 3: *to++ = *from++;
	case 2: *to++ = *from++;
	case 1: *to++ = *from++;
		} while (--n > 0);
	}
}
#include "data_prc.h"




#include <stm32f10x.h>
void DMA1_Channel2_IRQHandler() {
	if (DMA_GetITStatus(DMA1_IT_TC2)) {
		DMA_GetCurrDataCounter(DMA1_Channel2);//Can be used to count
		DMA_ClearITPendingBit(DMA1_IT_TC2);

		unsigned short int data;
		for (data = 0; data < BUFFER_SIZE; data++) {
			DATA.BUFFER[data][0] = AD.BUFFER[data][0];
			DATA.BUFFER[data][1] = AD.BUFFER[data][2];
			DATA.BUFFER[data][2] = AD.BUFFER[data][4];
			DATA.BUFFER[data][3] = AD.BUFFER[data][1];
			DATA.BUFFER[data][4] = AD.BUFFER[data][3];
			DATA.BUFFER[data][5] = AD.BUFFER[data][5];
		}

		//_I2R_Memcpy_fast(DATA.BUFFER, AD.BUFFER, BUFFER_SIZE*CHANNEL_CNT);
		AD.FLAG = SET;
	}
}



#endif