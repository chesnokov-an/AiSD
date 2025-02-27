#ifndef STACK_H
#define STACK_H

#include "err.h"

typedef struct Stack Stack;

Stack *stack_new();
void clear_stack(Stack *stack);
err stack_push(Stack *stack, int input);
err stack_pop(Stack *stack, int *output);
void stack_print(const Stack *stack);

#endif
