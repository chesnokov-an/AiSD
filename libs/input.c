#include <stdio.h>
#include "err.h"

err input_uint(unsigned *data, unsigned start, unsigned end){
    char end_of_scanf = ' ';
    int flag_scanf = scanf("%9u%c", data, &end_of_scanf);

    while((flag_scanf != 2) || (end_of_scanf != '\n') || (*data < start) || (*data > end)){
        if(flag_scanf == EOF){
            return ERR_EOF;
        }
        if((flag_scanf != 2) || (end_of_scanf != '\n')){
            scanf("%*[^\n]");
        }
        printf("Повторите ввод: ");
        flag_scanf = scanf("%9u%c", data, &end_of_scanf);
    }
    return ERR_OK;
}

err txt_input_int(FILE* file_name, int *data, int start, int end){
    char end_of_scanf = ' ';
    int flag_scanf = fscanf(file_name, "%9d%c", data, &end_of_scanf);
	if(flag_scanf == EOF){
		return ERR_EOF;
	}
	if((flag_scanf != 2) || (end_of_scanf != '\n') || (*data < start) || (*data > end)){
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
