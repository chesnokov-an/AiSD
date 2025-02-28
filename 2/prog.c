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
		flag = pop(stack, &operand1);
		if(flag != ERR_OK){
			break;
		}
		flag = pop(stack, &operand2);
		if(flag != ERR_OK){
			break;
		}
		char *res = calloc(strlen(operand1) + strlen(operand2) + 4, sizeof(char));
		res[0] = '(';
		memcpy(res + 1, operand2, strlen(operand2) * sizeof(char));
		res[1 + strlen(operand2)] = data;
		memcpy(res + 2 + strlen(operand2), operand1, strlen(operand1) * sizeof(char));
		res[2 + strlen(operand1) + strlen(operand2)] = ')';
		free(operand1);
		free(operand2);
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



