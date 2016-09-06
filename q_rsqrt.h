#ifndef q_rsqrt_H
#define q_rsqrt_H

//wiki http://en.wikipedia.org/wiki/Fast_inverse_square_root

static float q_rsqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;// evil floating point bit level hacking���Ը�������а��λ��hack��
	i = 0x5f3759df - (i >> 1);// what the fuck?�������������ô���£���
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration ����һ��ţ�ٵ�����
	//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed���ڶ��ε���������ɾ����
	return y;
}
#endif