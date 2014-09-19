#ifndef DS_CONV_H
#define DS_CONV_H

unsigned int convolutionLength(unsigned int ft, unsigned int gt) {
	return ft + gt - 1;
}
int convolutionBuffer(int *f, unsigned int ft, int *g, unsigned int gt, int *r) {
	unsigned int rt = ft + gt - 1;
	unsigned int i, j;
	int addOn; int lastCoe;
	for (i = 0; i < rt; i++) {
		j = (i - gt + 1 > 0) ? (i - gt + 1) : 0;
		ft = (ft - 1 < i) ? (ft - 1) : i;
		while (j < ft) {
			addOn = f[j] * g[i - j];
			lastCoe = r[i];
			r[i] = addOn + lastCoe;
			if ((r[i] - lastCoe) != addOn)
				return -1;
			j++;
		}
	}
	return 1;
	
}
#endif