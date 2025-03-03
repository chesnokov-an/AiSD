#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include "err.h"
#include "stack.h"
#include "expression.h"

#define PROMPT "> "
/*
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
	return (strchr("+-*", data[0]) != NULL);
}

char *expression(char *operand1, char *operand2, char operator){
	char *res = calloc(strlen(operand1) + strlen(operand2) + 8, sizeof(char));
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
*/

int main(){
	rl_inhibit_completion = 1;
	char *input = readline(PROMPT);
	err flag = ERR_OK;
	while((input != NULL) && (flag == ERR_OK)){
		flag = show_infix(input);
		free(input);
		input = readline(PROMPT);
	}

	return 0;
}

