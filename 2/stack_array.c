#include <stdio.h>
#include <stdlib.h>
#include "err.h"
#include "stack.h"

typedef struct Stack{
	size_t top;
	size_t capacity;
	char **array;
} Stack;


Stack *stack_new(){
	Stack *stack = (Stack *)calloc(1, sizeof(Stack));
	stack->top = 0;
	stack->capacity = 100;
	stack->array = calloc(100, sizeof(char*));
	return stack;
}

void clear_stack(Stack *stack){

	if(!stack){
		return;
	}
	if(stack->array){
		for(size_t i = stack->top; i > 0; i--){
			if(stack->array[i-1] != NULL){
				free(stack->array[i-1]);
			}
		}
		free(stack->array);
	}
	free(stack);
}

char is_full(Stack *stack){
	return (stack->top == stack->capacity);
}

char is_empty(Stack *stack){
	return (stack->top == 0);
}

err push(Stack *stack, char *input){
	if(!stack || !stack->array){
		return ERR_NULL;
	}
	if(is_full(stack)){
		return ERR_VAL;
	}
	stack->array[stack->top] = input;
	stack->top += 1;
	return ERR_OK;
}

err peek(Stack *stack, char **output){
	if(!stack || !stack->array){
		return ERR_NULL;
	}
	if(is_empty(stack)){
		return ERR_VAL;
	}
	*output = stack->array[stack->top - 1];
	return ERR_OK;
}

err pop(Stack *stack, char **output){
	err flag = peek(stack, output);
	if(flag != ERR_OK){
		return flag;
	}
	stack->top -= 1;
	return ERR_OK;
}

void stack_print(const Stack *stack){
	if(!stack || !stack->array){
		return;
	}
	for(size_t i = stack->top; i > 0; i--){
		printf("%s ", stack->array[i-1]);
	}
	printf("\n");
}

