#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <limits.h>
#include <ctype.h>
#include "err.h"
#include "input.h"
#include "table.h"
#include "dialog_table.h"
#include "my_readline.h"

void message(err flag){
	switch(flag){
	case ERR_MEM:
		printf("\nОшибка при выделении памяти.\n");
		break;
	case ERR_VAL:
		printf("\nНекорректное значение.\n");
		break;
	case ERR_NULL:
		printf("\nПередан нулевой указатель.\n");
		break;
	case ERR_FULL:
		printf("\nТаблица заполнена.\n");
		break;
	case ERR_EMPTY:
		printf("\nТаблица пуста.\n");
		break;
	case ERR_EOF:
		printf("\nЗавершение программы.\n");
		break;
	case ERR_OK:
		break;
	}
}

void Dialog(){
	err (*func_array[])(Table *) = {D_input, D_load, D_insert, D_delete, D_find, D_show, D_reorganize};

	unsigned msize = 0;
	printf("Введите количество ячеек памяти, которые нужно выделить под таблицу: ");
	err flag = input_uint(&msize, 0, UINT_MAX);
	if(flag == ERR_EOF){ goto end_program; }
	Table *table = create_table(msize);
	int option = -1;
	do{
		printf("\n\n---------- МЕНЮ ----------\nВозможные команды:\n\n");
		printf("0: Завершение программы.\n1: Ввод таблицы с клавиатуры.\n2: Импорт таблицы из файла.\n3: Вставка нового элемента.\n4: Удаление элемента.\n5: Поиск элемента.\n6: Вывод содержимого массива.\n7: Реорганизация таблицы.\n\n");
		printf("Выберите опцию: ");
		flag = input_int(&option, 0, 7);
		printf("\n");
		if(flag == ERR_EOF || option == 0){ goto end_program; }
		flag = func_array[option-1](table);

		if(flag != ERR_OK){ message(flag); }
	}while(option != 0);
	goto end_program;

end_program:
	clear_table(table);
	free(table);
	return;
}

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
	if(new_table == NULL){ return ERR_MEM; }
	unsigned msize = 0;
	printf("Введите максимальное число элементов: ");
	err flag = input_uint(&msize, 0, UINT_MAX);
	if(flag != ERR_OK || msize == 0){ goto clean_and_return; }
	unsigned csize = 0;
	printf("Введите число элементов: ");
	flag = input_uint(&csize, 0, msize);
	if(flag != ERR_OK){ goto clean_and_return; }
	new_table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if(new_table->ks == NULL){
		flag = ERR_MEM;
		goto clean_and_return;
	}
	new_table->msize = msize;
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

err D_load(Table *table){
	char *str_file = readline("Введите название файла: ");
	FILE *file = fopen(str_file, "r");
	free(str_file);
	err flag = ERR_OK;
	if(!file){
		printf("Unknown file\n");
		flag = ERR_VAL;
		return flag;
	}
	flag = load_from_txt(table, file);
	fclose(file);
	return flag;
}

err D_reorganize(Table *table){
	err flag = reorganize_table(table);
	show_table(table);
	return flag;
}

char str_isdigit(char *s){
	for(size_t i = 0; i < strlen(s); i++){
		if(!isdigit(s[i])){
			return 0;
		}
	}
	return 1;
}

err D_find(Table *table){
	err flag = ERR_OK;
	unsigned key = 0;
	printf("Введите ключ: ");
	flag = input_uint(&key, 1, 2);
	if(flag != ERR_OK){ return flag; }
	char *str_release = readline("Введите версию ( enter, если нужно получить все значения по ключу ): ");
	Table *new_table = NULL;
	if(strlen(str_release) == 0){
		new_table = find_by_key(table, key);
	}
	else if((str_isdigit(str_release)) && (strlen(str_release) <= 9)){
		unsigned release = strtoul(str_release, NULL, 0);
		if(flag != ERR_OK){
			free(str_release);
			return flag;
		}
		new_table = find_by_release(table, key, release);
	}
	else{
		free(str_release);
		flag = ERR_VAL;
		return flag;
	}
	free(str_release);
	show_table(new_table);
	if(new_table != NULL){
		clear_table(new_table);
		free(new_table);
	}
	return ERR_OK;
}
