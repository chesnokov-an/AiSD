#ifndef MATRIX_H
#define MATRIX_H

#include "err.h"

typedef struct Line{
	int n;
	int *data;
} Line;

typedef struct Matrix{
	int m;
	Line *matr;
} Matrix;

err input_matrix(Matrix *);
void clear_line(Line *);
void clear_matrix(Matrix *);
void output_matrix(const char *, Matrix *);
Line *ind_task(Matrix *);
void output_line(const char *, Line *);

#endif
