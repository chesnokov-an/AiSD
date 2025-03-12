#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "err.h"
#include "stack.h"

typedef struct Stack{
	size_t top;
	size_t capacity;
	char **array;
} Stack;


Stack *stack_new(int capacity){
	if(capacity <= 0){
		return NULL;
	}
	Stack *stack = (Stack *)calloc(1, sizeof(Stack));
	stack->top = 0;
	stack->capacity = capacity;
	stack->array = (char **)calloc(capacity, sizeof(char*));
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

err is_full(Stack *stack){
	return (stack->top == stack->capacity) ? ERR_EMPTY : ERR_OK;
}

err is_empty(Stack *stack){
	return (stack->top == 0) ? ERR_FULL : ERR_OK;
}

err push(Stack *stack, char *input){
	if(!stack || !stack->array){
		return ERR_NULL;
	}
	if(is_full(stack)){
		return ERR_FULL;
	}
	stack->array[stack->top] = input;
	stack->top += 1;
	return ERR_OK;
}

char *peek(Stack *stack){
	char *output = NULL;
	if(!stack || !stack->array || is_empty(stack)){
		return NULL;
	}
	output = stack->array[stack->top - 1];
	return output;
}

char *pop(Stack *stack){
	char *output = peek(stack);
	if(!output){
		return NULL;
	}
	stack->top -= 1;
	return output;
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
