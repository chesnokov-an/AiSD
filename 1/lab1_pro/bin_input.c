#include <stdio.h>
#include "err.h"

err bin_input_uint(FILE* file_name, unsigned int* data, unsigned int start, unsigned int end){
	fread(data, sizeof(unsigned int), 1, file_name);
	if(feof(file_name)){
		return ERR_EOF;
	}
	if((ferror(file_name)) || (*data < start) || (*data > end)){
		return ERR_VAL;
	}
	return ERR_OK;
}

err bin_input_int(FILE* file_name, int *data, int start, int end){
	fread(data, sizeof(int), 1, file_name);
	if(feof(file_name)){
		return ERR_EOF;
	}
	if((ferror(file_name)) || (*data < start) || (*data > end)){
		return ERR_VAL;
	}
	return ERR_OK;
}
