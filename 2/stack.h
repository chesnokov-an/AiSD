#ifndef STACK_H
#define STACK_H

#include "err.h"

typedef struct Stack Stack;

Stack *stack_new();
void clear_stack(Stack *stack);
err push(Stack *stack, char *input);
err pop(Stack *stack, char **output);
err peek(Stack *stack, char **output);
void stack_print(const Stack *stack);
char is_empty(Stack *stack);
char is_full(Stack *stack);

#endif
