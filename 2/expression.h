#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "err.h"

int is_operand(char *);
int is_operator(char *);
char *post_to_inf(char *, char *, char);
err show_infix(const char *);

#endif
