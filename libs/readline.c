#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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

char *bin_read_n_symbols(FILE* file_name, int n){
	char *res = NULL;
	res = calloc(n+1, sizeof(char));
	fread(res, sizeof(char), n, file_name);
	if(feof(file_name)){
		return NULL;
	}
	return res;
}

char *bin_readline(FILE* file_name){
	char *res = NULL;
	int count;
	err flag = bin_input_int(file_name, &count, 0, INT_MAX);
	if(flag != ERR_OK){
		return NULL;
	}
	res = bin_read_n_symbols(file_name, count);
	if(res == NULL){
		return NULL;
	}
	return res;
}
