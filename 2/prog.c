#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include "err.h"
#include "stack.h"

#define PROMPT "> "

int is_operand(char *data){
	for(size_t i = 0; i < strlen(data); i++){
		if(!isalpha(data[i])){
			return 0;
		}
	}
	return 1;
}

int is_operator(char *data){
	if(strlen(data) != 1){
		return 0;
	}
	return (strchr("+-*/", data[0]) != NULL);
}

char *post_to_inf(char *operand1, char *operand2, char operator){
	char *res = calloc(strlen(operand1) + strlen(operand2) + 8, sizeof(char));
	if(!res){
		return NULL;
	}
	res[0] = '(';
	res[1] = ' ';
	memcpy(res + 2, operand2, strlen(operand2) * sizeof(char));
	res[2 + strlen(operand2)] = ' ';
	res[3 + strlen(operand2)] = operator;
	res[4 + strlen(operand2)] = ' ';
	memcpy(res + 5 + strlen(operand2), operand1, strlen(operand1) * sizeof(char));
	res[5 + strlen(operand1) + strlen(operand2)] = ' ';
	res[6 + strlen(operand1) + strlen(operand2)] = ')';
	free(operand1);
	free(operand2);
	return res;
}

err show_infix(const char *input, int capacity){
	int count_operand = 0;
	int count_operator = 0;
	err flag = ERR_OK;
	Stack *stack = stack_new(capacity);
	char *s = strdup(input);
	char *data = strtok(s, "\t ");
	while(data != NULL){
		if(!(is_operand(data) || is_operator(data))){
			flag = ERR_VAL;
			printf("Некорректное выражение\n");
			break;
		}
		if(is_operand(data)){
			count_operand += 1;
			char *operand = calloc(strlen(data) + 1, sizeof(char));
			memcpy(operand, data, strlen(data) * sizeof(char));
			flag = push(stack, operand);
			if(flag != ERR_OK){
				printf("Ошибка памяти\n");
				free(operand);
				break;
			}
			data = strtok(NULL, "\t ");
			continue;
		}
		count_operator += 1;
		char *operand1 = NULL, *operand2 = NULL;
		operand1 = pop(stack);
		if(!operand1){
			flag = ERR_MEM;
			printf("Ошибка памяти\n");
			break;
		}
		operand2 = pop(stack);
		if(!operand2){
			flag = ERR_MEM;
			printf("Ошибка памяти\n");
			free(operand1);
			break;
		}
		char *res = post_to_inf(operand1, operand2, data[0]);
		if(!res){
			flag = ERR_MEM;
			printf("Ошибка памяти\n");
			break;
		}
		flag = push(stack, res);
		if(flag != ERR_OK){
			printf("Ошибка памяти\n");
			break;
		}
		data = strtok(NULL, "\t ");
	}
	free(s);
	if(flag == ERR_OK && !(((count_operator + 1) == count_operand) || ((count_operator == 0) && (count_operand == 0)))){
		printf("Некорректное выражение\n");
		flag = ERR_VAL;
	}
	if(flag == ERR_OK){
		stack_print(stack);
	}
	clear_stack(stack);
	return flag;
}

int main(int argc, char **argv){
	int capacity = 0;
	if(argc > 1){
		capacity = atoi(argv[1]);
	}
	rl_inhibit_completion = 1;
	char *input = readline(PROMPT);
	err flag = ERR_OK;
	while(input != NULL){
		flag = show_infix(input, capacity);
		free(input);
		input = readline(PROMPT);
	}
	if(flag != ERR_OK){
		free(input);
	}

	return 0;
}
