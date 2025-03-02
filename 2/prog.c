#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "err.h"
#include "stack.h"

int is_operand(char data){
	return isalpha(data);
}

int is_operator(char data){
	return (strchr("+-*/", data) != NULL);
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
	res[5 + strlen(operand2) + strlen(operand2)] = ' ';
	res[6 + strlen(operand1) + strlen(operand2)] = ')';
	free(operand1);
	free(operand2);
	return res;
}

int main(){
	Stack *stack = stack_new();
	err flag = ERR_OK;
	char data = getchar();
	while((data != '\n') && (data != EOF)){
		if(data == ' '){
			data = getchar();
			continue;
		}
		if(!(is_operand(data) || is_operator(data))){
			flag = ERR_VAL;
			break;
		}
		if(is_operand(data)){
			char *operand = calloc(2, sizeof(char));
			operand[0] = data;
			flag = push(stack, operand);
			if(flag != ERR_OK){
				break;
			}
			data = getchar();
			continue;
		}
		char *operand1 = NULL, *operand2 = NULL;
		operand1 = pop(stack);
		if(!operand1){
			break;
		}
		operand2 = pop(stack);
		if(!operand2){
			free(operand1);
			break;
		}
		char *res = expression(operand1, operand2, data);
		if(!res){
			break;
		}
		flag = push(stack, res);
		if(flag != ERR_OK){
			break;
		}
		data = getchar();
	}
	stack_print(stack);
	clear_stack(stack);
	return (int)flag;
}



