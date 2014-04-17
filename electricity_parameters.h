#ifndef __ELECTRICITY_PARAMETERS_H
#define __ELECTRICITY_PARAMETERS_H

#include "stdio.h"

typedef BufferType  (short)

struct DATA_struct
{
	//int16_t BUFFER[BUFFER_SIZE_50Hz][CHANNEL_CNT];//AD data for data processing  Warming   this is the problem!
	short int BUFFER_Volt[Def_BUFFER_SIZE][CHANNEL_CNT/2]; /**UA UB UC IA IB IC*/
	short int BUFFER_Amp[Def_BUFFER_SIZE][CHANNEL_CNT/2]; /**UA UB UC IA IB IC*/

	short int DC_Volt[CHANNEL_CNT/2];        /*DC value of each channel*/
	short int DC_Amp[CHANNEL_CNT/2];        /*DC value of each channel*/

	float FRQ[CHANNEL_CNT/2];       /*AC frequency of each value*/

	float RMS_Volt[CHANNEL_CNT/2];    /*AC RMS of each channel*/
	float RMS_Amp[CHANNEL_CNT/2];    /*AC RMS of each channel*/

	float	W[CHANNEL_CNT/2];       /*each channel active power*/
	float	VAR[CHANNEL_CNT/2];     /*each channel Reactive power*/
	float VA[CHANNEL_CNT/2];      /*Apparent Power*/
	float PF[CHANNEL_CNT/2];      /*Power Factor*/

	float P_POWER[CHANNEL_CNT/2][2];/*Postive and negtive active power*/
	float Q_POWER[CHANNEL_CNT/2][2];/*Postive and negtive reactive power*/

	float P[CHANNEL_CNT/2];        /*Active Electronic  power*/
	float Q[CHANNEL_CNT/2];        /*Reactive Electronic  power*/
};


extern struct DATA_struct DATA;

extern unsigned short int BUFFER_SIZE,BUFFER_SIZE_tmp;
extern const unsigned int *QuasiSynchronousCoefficient,\
COE_45Hz[BUFFER_SIZE_45Hz],COE_50Hz[BUFFER_SIZE_50Hz],\
COE_55Hz[BUFFER_SIZE_55Hz],COE_60Hz[BUFFER_SIZE_60Hz],\
COE_65Hz[BUFFER_SIZE_65Hz];


extern const float SamplingCoefficient[CHANNEL_CNT];

extern float SamplingCoefficientOffsetVolt[CHANNEL_CNT/2];
extern float SamplingCoefficientOffsetAmp[CHANNEL_CNT/2][3];

extern const float PowerCoefficient[CHANNEL_CNT/2];
extern float PowerCoefficientOffset[CHANNEL_CNT/2];


void CopyADBuffer(struct DATA_struct *DATA);

bool Eliminate_DC(struct DATA_struct *DATA);
void IIRlpf_FRQ(struct DATA_struct *DATA);
void Quasi_RMS(struct DATA_struct *DATA);
void IIRlpf_Quasi_Power(struct DATA_struct *DATA);
void SecondaryParameter(struct DATA_struct *DATA);
void ElectronicPower(struct DATA_struct *DATA);

void FFT_Harmonic(struct DATA_struct *DATA);



#endif