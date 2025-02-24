#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "matrix.h"
#include "bin_input.h"
#include "err.h"

#define MAGIC_WORD "DWRF"

err check_magic_word(FILE* file){
	char *magic_word = NULL;
	magic_word = calloc(strlen(MAGIC_WORD)+1, sizeof(char));
	fread(magic_word, sizeof(char), strlen(MAGIC_WORD), file);
	if(feof(file)){
		free(magic_word);
		return ERR_VAL;
	}
	if(strcmp(magic_word, MAGIC_WORD) != 0){
		free(magic_word);
		return ERR_VAL;
	}
	free(magic_word);
	return ERR_OK;
}

err input_meta(FILE *file, Meta *meta){
	err flag = check_magic_word(file);
	if(flag != ERR_OK){ goto clear_and_return; }
	flag = bin_input_uint(file, &meta->m, 0, UINT_MAX);
	if(flag == ERR_EOF){ goto clear_and_return; }
	if(!meta->m){ return ERR_OK; }

	meta->n = calloc(meta->m, sizeof(unsigned int));
	if(!meta->n){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	meta->offset = calloc(meta->m, sizeof(unsigned int));
	if(!meta->offset){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	for(unsigned int i = 0; i < meta->m; i++){
		flag = bin_input_uint(file, meta->n + i, 0, UINT_MAX);
		if(flag == ERR_EOF){ goto clear_and_return; }
		flag = bin_input_uint(file, meta->offset + i, 0, UINT_MAX);
		if(flag == ERR_EOF){ goto clear_and_return; }
	}

	return ERR_OK;

clear_and_return:
	clear_meta(meta);
	free(meta);
	return flag;
}

void clear_meta(Meta *meta){
	if(!meta || (!meta->n && !meta->offset)){
		return;
	}
	if(!meta->offset){
		free(meta->n);
		return;
	}
	if(!meta->n){
		free(meta->offset);
		return;
	}
	free(meta->n);
	free(meta->offset);
}

void output_meta(FILE *file, Meta *meta){
	char magic_word[4] = MAGIC_WORD;
	fwrite(magic_word, 4, sizeof(char), file);
	fwrite(&meta->m, 1, sizeof(unsigned int), file);
	for(unsigned int i = 0; i < meta->m; i++){
		fwrite(meta->n + i, 1, sizeof(unsigned int), file);
		fwrite(meta->offset + i, 1, sizeof(unsigned int), file);
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

err get_elem(FILE *file, unsigned int offset, int *data){
	fseek(file, offset, SEEK_SET);
	err flag = bin_input_int(file, data, INT_MIN, INT_MAX);
	return flag;
}

void out_elem(FILE *file, unsigned int offset, int *data){
	fseek(file, offset, SEEK_SET);
	fwrite(data, 1, sizeof(int), file);
}

err output_matrix(FILE *input_file, FILE *output_file, Meta *meta){
	err flag = ERR_OK;

	Meta *res = NULL;
	res = calloc(1, sizeof(Meta));
	if(!res){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	res->m = 1;
	res->n = calloc(1, sizeof(unsigned int));
	if(!res->n){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	*(res->n) = meta->m;

	res->offset = calloc(1, sizeof(unsigned int));
	if(!res->offset){
		flag = ERR_MEM;
		goto clear_and_return;
	}

	*(res->offset) = 3 * sizeof(unsigned int) + strlen(MAGIC_WORD) * sizeof(char);

	output_meta(output_file, res);

	for(unsigned int i = 0; i < meta->m; i++){
		int first_elem = 0;
		flag = get_elem(input_file, *(meta->offset + i), &first_elem);
		if(flag != ERR_OK){ goto clear_and_return; }

		int first_sum = sum_of_digits(first_elem);
		int res_sum = 0;
		for(unsigned int j = 0; j < *(meta->n + i); j++){
			int current = 0;
			flag = get_elem(input_file, *(meta->offset + i) + j*sizeof(int), &current);
			if(flag != ERR_OK){ goto clear_and_return; }

			if(sum_of_digits(current) == first_sum){
				res_sum += current;
			}
			out_elem(output_file, *(res->offset) + i*sizeof(int), &res_sum);
		}
	}
	goto clear_and_return;

clear_and_return:
	if(flag == ERR_MEM){
		fprintf(stderr, "Не удалось выделить память.");
	}
	else if(flag == ERR_VAL){
		fprintf(stderr, "Некорректное значение в файле.");
	}
	clear_meta(res);
	free(res);
	return flag;
}
