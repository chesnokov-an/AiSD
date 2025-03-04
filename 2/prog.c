#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include "err.h"
#include "stack.h"
#include "expression.h"

#define PROMPT "> "

int main(){
	rl_inhibit_completion = 1;
	char *input = readline(PROMPT);
	err flag = ERR_OK;
	while(input != NULL){
		flag = show_infix(input);
		free(input);
		input = readline(PROMPT);
	}
	if(flag != ERR_OK){
		free(input);
	}

	return 0;
}

