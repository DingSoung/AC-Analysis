#ifndef DS_CONV_H
#define DS_CONV_H

#include "stdio.h"

unsigned int convolutionLength(unsigned int ft, unsigned int gt) {
	return ft + gt - 1;
}
int convolutionBuffer(int *f, unsigned int ft, int *g, unsigned int gt, int *r) {
	unsigned int rt = ft + gt - 1;
	unsigned int i, j, k;
	for (i = 0; i < rt; i++) {
		r[i] = 0x00;
		k = ((ft < i + 1) ? (ft - 1) : i);
		for (j = ((i + 1 > gt) ? (i + 1 - gt) : 0); j <= k; j++) {
			r[i] += f[j] * g[i - j];
		}
	}


	return 1;

}
#endif