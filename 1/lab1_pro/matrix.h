#ifndef MATRIX_H
#define MATRIX_H

#include "err.h"

typedef struct Meta{
	unsigned int m;
	unsigned int *n;
	unsigned int *offset;
} Meta;

err input_meta(Meta *);
void output_meta(const char *, Meta *);
void output_matrix(const char *, Meta *);
void clear_meta(Meta *);

#endif
