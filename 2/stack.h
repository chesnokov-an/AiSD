#ifndef STACK_H
#define STACK_H

#include "err.h"

typedef struct Stack Stack;

Stack *stack_new();
void clear_stack(Stack *stack);
err push(Stack *stack, char *input);
char *pop(Stack *stack);
char *peek(Stack *stack);
void stack_print(const Stack *stack);
err is_empty(Stack *stack);
err is_full(Stack *stack);

#endif
