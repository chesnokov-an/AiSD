#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <limits.h>
#include "err.h"
#include "input.h"
#include "table.h"
#include "dialog_table.h"
#include "my_readline.h"

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

err D_delete(Table *table){
	if(table == NULL){
		return ERR_NULL;
	}
	unsigned key = 0;
	printf("Введите ключ: ");
	err flag = input_uint(&key, 0, UINT_MAX);
	if(flag == ERR_EOF){
		return ERR_EOF;
	}
	unsigned release = 0;
	printf("Введите версию: ");
	flag = input_uint(&release, 0, UINT_MAX);
	if(flag == ERR_EOF){
		return ERR_EOF;
	}
	flag = delete_elem(table, key, release);
	return flag;
}

err D_show(Table *table){
	show_table(table);
	return ERR_OK;
}

err D_input(Table *table){
	Table *new_table = (Table *)calloc(1, sizeof(Table));
	unsigned msize = 0;
	printf("Введите максимальное число элементов: ");
	err flag = input_uint(&msize, 0, UINT_MAX);
	if(flag != ERR_OK || msize == 0){ goto clean_and_return; }
	unsigned csize = 0;
	printf("Введите число элементов: ");
	flag = input_uint(&csize, 0, msize);
	if(flag != ERR_OK){ goto clean_and_return; }
	new_table->msize = msize;
	new_table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if(new_table->ks == NULL){
		flag = ERR_MEM;
		goto clean_and_return;
	}
	for(unsigned i = 0; i < csize; i++){
		unsigned key = 0;
		printf("Введите ключ: ");
		flag = input_uint(&key, 0, UINT_MAX);
		if(flag != ERR_OK){ goto clean_and_return; }
		char *info = readline("Введите строку: ");
		if(info == NULL){
			flag = ERR_MEM;
			goto clean_and_return;
		}
		flag = insert_elem(new_table, key, info);
		free(info);
		if(flag != ERR_OK){ goto clean_and_return; }
	}
	clear_table(table);
	*table = *new_table;
	free(new_table);
	return ERR_OK;

clean_and_return:
	clear_table(new_table);
	free(new_table);
	return flag;
}

err D_reorganize(Table *table){
	err flag = reorganize_table(table);
	return flag;
}

err D_find(Table *table){
	err flag = ERR_OK;
	int option = 0;
	printf("Опции:\n\t1: поиск по ключу\n\t2: поиск по ключу и версии\nВведиет опцию: ");
	flag = input_int(&option, 1, 2);
	if(flag != ERR_OK){ return flag; }
	Table *new_table = NULL;
	if(option == 1){
		unsigned key = 0;
		flag = input_uint(&key, 0, UINT_MAX);
		if(flag != ERR_OK){ return flag; }
		new_table = find_by_key(table, key);
	}
	else if(option == 2){
		unsigned key = 0;
		flag = input_uint(&key, 0, UINT_MAX);
		if(flag != ERR_OK){ return flag; }
		unsigned release = 0;
		flag = input_uint(&release, 0, UINT_MAX);
		if(flag != ERR_OK){ return flag; }
		new_table = find_by_release(table, key, release);
	}
	show_table(new_table);
	if(new_table != NULL){
		clear_table(new_table);
		free(new_table);
	}
	return ERR_OK;
}
err D_clear(Table *table){
	clear_table(table);
	return ERR_OK;
}
