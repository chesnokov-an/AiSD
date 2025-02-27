#include <stdio.h>
#include <stdlib.h>
#include "err.h"

typedef struct Item{
	int data;
	Item *next;
} Item;

typedef struct Stack{
	Item *top;
} Stack;


Stack *stack_new(){
	return (Stack *) calloc(1, sizeof(Stack));
}

void clear_stack(Stack *stack){
	if(!stack){
		return;
	}
	Item *item = stack->top, prev_item = NULL;
	while(item){
		prev_item = item;
		item = item->next;
		free(prev_item);
	}
	free(stack);
}

err stack_push(Stack *stack, int input){
	if(!stack){
		return ERR_NULL;
	}

	Item *item = calloc(1, sizeof(Item));
	if(!item){
		return ERR_MEM;
	}
	item->data = input;
	item->next = stack->top;
	stack->top = item;
	return ERR_OK;
}

err stack_pop(Stack *stack, int *output){
	if(!stack){
		return ERR_NULL;
	}
	if(!stack->top){
		return ERR_VAL;
	}
	*output = stack->top->data;
	Item *item = stack->top;
	stack->top = item->next;
	free(item);
	return ERR_OK;
}

void stack_print(const Stack *stack){
	if(!stack){
		return;
	}
	Item *item = stack->top;
	while(item){
		printf("%d ", item->data);
		item = item->next;
	}
	printf("\n");
}








