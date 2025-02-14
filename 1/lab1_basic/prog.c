#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(){
	Matrix *matrix = NULL;
	matrix = calloc(1, sizeof(Matrix));

	err flag = input_matrix(matrix);

	if(flag != ERR_OK){
		if(flag == ERR_MEM){
			fprintf(stderr, "Ошибка выделения памяти.\n");
		}
		return (int)ERR_EOF;
	}
	
	Line *line = ind_task(matrix);

	output_matrix("\nМатрица:\n\n", matrix);
	output_line("\nВектор индивидуального задания: \n\n", line);
	clear_matrix(matrix);
	free(matrix);
	clear_line(line);
	free(line);

	return 0;
}
