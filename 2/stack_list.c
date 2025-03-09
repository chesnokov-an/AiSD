#include <stdio.h>
#include <stdlib.h>
#include "err.h"
#include "stack.h"

typedef struct Item{
	char *data;
	struct Item *next;
} Item;

typedef struct Stack{
	Item *top;
} Stack;


Stack *stack_new(int capacity){
	return (Stack *) calloc(1, sizeof(Stack));
}

void clear_stack(Stack *stack){
	if(!stack){
		return;
	}
	Item *item = stack->top, *prev_item = NULL;
	while(item){
		prev_item = item;
		item = item->next;
		free(prev_item->data);
		free(prev_item);
	}
	free(stack);
}

err push(Stack *stack, char *input){
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

char *peek(Stack *stack){
	char *output = NULL;
	if(!stack || !stack->top){
		return NULL;
	}
	output = stack->top->data;
	return output;
}

char *pop(Stack *stack){
	char *output = peek(stack);
	if(!output){
		return NULL;
	}
	Item *item = stack->top;
	stack->top = item->next;
	free(item);
	return output;
}

void stack_print(const Stack *stack){
	if(!stack){
		return;
	}
	Item *item = stack->top;
	while(item){
		printf("%s", item->data);
		item = item->next;
	}
	printf("\n");
}
