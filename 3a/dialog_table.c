#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <limits.h>
#include "err.h"
#include "input_uint.h"
#include "table.h"
#include "dialog_table.h"

err D_insert(Table *table){
	if(table == NULL){
		return ERR_NULL;
	}
	unsigned key = 0;
	printf("Введите ключ: ");
	err flag = input_uint(&key, 0, UINT_MAX);
	if(flag == ERR_EOF){
		return ERR_EOF;
	}
	char *elem = readline("Введите элемент: ");
	if(elem == NULL){
		return ERR_EOF;
	}
	flag = insert_elem(table, key, elem);
	free(elem);
	return flag;
}
