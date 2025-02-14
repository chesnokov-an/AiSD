#ifndef MATRIX_H
#define MATRIX_H

#include "err.h"

typedef struct Meta{
	unsigned int m;
	unsigned int *n;
	unsigned int *offset;
} Meta;

err input_meta(FILE *, Meta *);
void output_meta(FILE *, Meta *);
err output_matrix(FILE *, FILE *, Meta *);
void clear_meta(Meta *);

#endif
