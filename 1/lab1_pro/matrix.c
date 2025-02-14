#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "matrix.h"
#include "bin_input.h"
#include "err.h"

err input_meta(Meta *meta){
	err flag = bin_input_uint(&meta->m, 0, UINT_MAX);
	if(flag == ERR_EOF){ goto clear_and_return; }
	if(!matrix->m){ return ERR_OK; }

	meta->n = calloc(mate->m, sizeof(unsigned int));
	if(!meta->n){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	meta->offset = calloc(mate->m, sizeof(unsigned int));
	if(!meta->offset){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	for(int i = 0; i < meta->m; i++){
		flag = bin_input_uint(&(meta->n + i), 0, UINT_MAX);
		if(flag == ERR_EOF){ goto clear_and_return; }
		flag = bin_input_uint(&(meta->offset + i), 0, UINT_MAX);
		if(flag == ERR_EOF){ goto clear_and_return; }

	return ERR_OK;

clear_and_return:
	clear_meta(meta);
	free(meta);
	return flag;
}

void clear_meta(Meta *meta){
	if(!meta){
		return;
	}
	free(meta->n);
	free(meta->offset);
}


























/*
err input_matrix(Matrix *matrix){
	printf("Введите количество строк матрицы: ");
	err flag = input_int(&matrix->m, 0, INT_MAX);
	if(flag == ERR_EOF){ goto clear_and_return; }
	if(!matrix->m){ return ERR_OK; }

	matrix->matr = calloc(matrix->m, sizeof(Line));
	if(!matrix->matr){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	for(int i = 0; i < matrix->m; i++){
		printf("Введите количество элементов строки: ");
		flag = input_int(&(matrix->matr + i)->n, INT_MIN, INT_MAX);
		if(flag == ERR_EOF){ goto clear_and_return; }
		if(!((matrix->matr + i)->n)){ continue; }
		(matrix->matr + i)->data = calloc((matrix->matr + i)->n, sizeof(int));
		if(!(matrix->matr + i)->data){
			flag = ERR_MEM;
			goto clear_and_return;
		}
		
		printf("Введите элементы строки через Enter: ");
		for(int j = 0; j < (matrix->matr + i)->n; j++){
			flag = input_int((matrix->matr + i)->data + j, INT_MIN, INT_MAX);
			if(flag == ERR_EOF){ goto clear_and_return; }
		}
	}
	return ERR_OK;

clear_and_return:
	clear_matrix(matrix);
	free(matrix);
	return flag;
}

void clear_line(Line *line){
	if(!line){
		return;
	}
	free(line->data);
}

void clear_matrix(Matrix *matrix){
	if(!matrix){
		return;
	}
	for(int i = 0; i < matrix->m; i++){
		clear_line(matrix->matr + i);
	}
	free(matrix->matr);
}

void output_matrix(const char *prompt, Matrix *matrix){
	printf("%s", prompt);
	for(int i = 0; i < matrix->m; i++){
		for(int j = 0; j < (matrix->matr + i)->n; j++){
			printf("%d ", *((matrix->matr + i)->data + j));
		}
		printf("\n");
	}
}

int sum_of_digits(int a){
    int res = 0;
	while(a > 0){
	    res += a%10;
	    a /= 10;
	}
	return res;
}

int ind_sum(Line *line){
	if(!line || !line->data){
		return 0;
	}
	int first_sum = sum_of_digits(*(line->data));
	int res = 0;
	for(int j = 0; j < line->n; j++){
		if(sum_of_digits(*(line->data + j)) == first_sum){
			res += *(line->data + j);
		}
	}
	return res;
}

Line *ind_task(Matrix *matrix){
	Line *res = NULL;
	res = calloc(1, sizeof(Line));
	res->n = matrix->m;

	if(!res){
		return NULL;
	}
	res->data = calloc(res->n, sizeof(int));
	if(!res->data){
		return NULL;
	}

	for(int i = 0; i < res->n; i++){
		*(res->data + i) = ind_sum(matrix->matr + i);
	}
	return res;
}

void output_line(const char *prompt, Line *line){
	printf("%s", prompt);
	for(int j = 0; j < line->n; j++){
		printf("%d ", *(line->data + j));
	}
	printf("\n");
}
*/
