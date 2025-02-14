#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "matrix.h"

int main(){
	rl_inhibit_completion = 1;

	Meta *meta = NULL;
	meta = calloc(1, sizeof(Meta));

	if(!meta){
		fprintf(stderr, "Ошибка выделения памяти.\n");
		return (int)ERR_MEM;
	}

	char *s_input_file = readline("Введите входной файл: ");
	FILE *input_file = fopen(s_input_file, "rb");
	free(s_input_file);

	if(!input_file){
		fprintf(stderr, "Неизвестный файл.\n");
		free(meta);
		return 0;
	}

	err flag = input_meta(input_file, meta);

	if(flag != ERR_OK){
		if(flag == ERR_MEM){
			fprintf(stderr, "Ошибка выделения памяти.\n");
		}
		free(meta);
		fclose(input_file);
		return (int)flag;
	}
	
	char *s_output_file = readline("Введите выходной файл: ");
	FILE *output_file = fopen(s_output_file, "wb");

	flag = output_matrix(input_file, output_file, meta);
	free(s_output_file);
	if(flag != ERR_OK){
		if(flag == ERR_MEM){
			fprintf(stderr, "Не удалось выделить память.");
		}
		else{
			fprintf(stderr, "Некорректное значение в файле.");
		}
		clear_meta(meta);
		free(meta);
		fclose(input_file);
		fclose(output_file);
		return (int)flag;
	}

	clear_meta(meta);
	free(meta);

	fclose(input_file);
	fclose(output_file);

	return 0;
}
