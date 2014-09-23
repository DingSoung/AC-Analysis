#ifndef DS_CONV_H
#define DS_CONV_H

#include "stdio.h"

unsigned int convolutionLength(unsigned int ft, unsigned int gt) {
	return ft + gt - 1;
}
/*Return Max value of array, it strongly suggest not bigger than 0xEFFFFFFF, new array start at *r */
int convolutionBuffer(int *f, unsigned int ft, int *g, unsigned int gt, int *r) {
	unsigned int rt = ft + gt - 1;
	int k = 0x00;
	for (unsigned int i = 0; i < rt; i++) {
		r[i] = 0x00;
		for (unsigned int j = ((i + 1 > gt) ? (i + 1 - gt) : 0); j <= ((ft < i + 1) ? (ft - 1) : i); j++) {
			r[i] += f[j] * g[i - j];
		}
		k = r[i] > k ? r[i] : k;
	}
	return k;
}
#endif