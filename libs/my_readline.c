#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <readline/readline.h>
#include "err.h"
#include "input.h"

char *txt_readline(FILE* file_name){
	char buf[81] = {0};
	char *res = NULL;
	int len = 0;
	int flag = 0;

	do{
		flag = fscanf(file_name, "%80[^\n]", buf);
		if(flag < 0){
			if(!res){
				return NULL;
			}
		}
		if(flag > 0){
			int chunk_len = strlen(buf);
			int new_len = len + chunk_len;
			res = realloc(res, new_len + 1);
			memcpy(res + len, buf, chunk_len);
			len = new_len;
		}
		if(flag == 0){
			fscanf(file_name, "%*1[\n]");
		}
	}while(flag > 0);
	if(len > 0){
		res[len] = '\0';
	}
	else{
		res = calloc(1, sizeof(char));
	}
	return res;
}

char *bin_read_n_symbols(FILE* file_name, unsigned n){
	char *res = NULL;
	res = (char *)calloc(n+1, sizeof(char));
	if(res == NULL){
		return NULL;
	}
	fread(res, sizeof(char), n, file_name);
	if(feof(file_name)){
		return NULL;
	}
	return res;
}

char *bin_readline(FILE* file_name){
	unsigned count;
	err flag = bin_input_uint(file_name, &count, 0, UINT_MAX-1);
	if(flag != ERR_OK){
		return NULL;
	}
	char *res = bin_read_n_symbols(file_name, count);
	return res;
}


char *my_strip(char const * const s){
	if(s == NULL){
		return NULL;
	}
	size_t i = 0;
	while((i < strlen(s)) && ((s[i] == ' ') || (s[i] == '\t'))){
		i++;
	}
	size_t count = 0;
	while(((i + count) < strlen(s)) && ((s[i + count] != ' ') && (s[i + count] != '\t'))){
		count++;
	}
	char *new_s = (char *)calloc(count + 1, sizeof(char));
	if(!new_s){
		return NULL;
	}
	memcpy(new_s, s + i, count * sizeof(char));
	return new_s;
}

FILE *input_correct_file(){
	char *str_file = readline("Введите название файла: ");
	char *clear_s_file = my_strip(str_file);
	FILE *file = fopen(clear_s_file, "r");
	free(str_file);
	free(clear_s_file);
	if(!file){
		printf("Unknown file\n");
		return NULL;
	}
	return file;
}
