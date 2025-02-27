#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "err.h"

typedef struct Stack{
	unsigned top;
	unsigned capacity;
	int *array;
} Stack;


Stack *stack_new(){
	return (Stack *) calloc(1, sizeof(Stack));
}

void clear_stack(Stack *stack){
	if(!stack){
		return;
	}
	if(stack->array){
		free(stack->array);
	}
	free(stack);
}

bool is_full(Stack *stack){
	return (stack->top == stack->capacity);
}

bool is_empty(Stack *stack){
	return (stack->top == 0);
}

err stack_push(Stack *stack, int input){
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

err stack_pop(Stack *stack, int *output){
	if(!stack || !stack->array){
		return ERR_NULL;
	}
	if(is_empty(stack)){
		return ERR_VAL;
	}
	stack->top -= 1;
	*output = stack->array[stack->top];
	return ERR_OK;

}

void stack_print(const Stack *stack){
	for(unsigned i = stack->top - 1; i > 0; --i){
		printf("%d ", stack->array[stack->top]);
	}
	printf("%d\n", stack->array[stack->top]);
}








