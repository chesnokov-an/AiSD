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

#define START_CAPACITY 7
#define GREEN "\033[38;2;0;255;0m"
#define RED "\033[38;2;255;0;0m"
#define BLUE "\033[38;2;0;191;255m"
#define LIGHT "\033[38;2;180;180;180m"
#define ORANGE "\033[38;2;255;165;0m"
#define MAGENTA "\033[38;2;255;20;147m"
#define YELLOW "\033[38;2;255;255;0m"
#define RESET "\033[0;0m"

void message(err flag){
	switch(flag){
	case ERR_MEM:
		printf(RED"\nОшибка при выделении памяти.\n"RESET);
		break;
	case ERR_VAL:
		printf(RED"\nНекорректное значение.\n"RESET);
		break;
	case ERR_NULL:
		printf(RED"\nПередан нулевой указатель.\n"RESET);
		break;
	case ERR_FULL:
		printf(RED"\nТаблица заполнена.\n"RESET);
		break;
	case ERR_EMPTY:
		printf(RED"\nТаблица пуста.\n"RESET);
		break;
	case ERR_EOF:
		printf(RED"\nЗавершение программы.\n"RESET);
		break;
	case ERR_OK:
		break;
	}
}

void Dialog(){
	err (*func_array[])(Table *) = {D_insert, D_delete, D_find, D_show, D_input, D_output};
	Table *table = create_table(START_CAPACITY);
	int option = -1;
	err flag = ERR_OK;
	do{
		printf(MAGENTA"\n\n---------- МЕНЮ ----------\nВозможные команды:\n\n"RESET);
		printf("0: Завершение программы.\n"GREEN"1: Вставка нового элемента.\n"RED"2: Удаление элемента.\n"BLUE"3: Поиск элемента.\n"LIGHT"4: Вывод содержимого таблицы.\n"YELLOW"5: Импорт таблицы из бинарного файла.\n"ORANGE"6: Экспорт таблицы в бинарный файл.\n\n"RESET);
		printf(MAGENTA"Выберите опцию: "RESET);
		flag = input_int(&option, 0, 6);
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
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	char *info = readline("Введите элемент: ");
	if(info == NULL){
		free(key);
		return ERR_EOF;
	}
	err flag = insert_elem(table, key, info);
	free(key);
	free(info);
	return flag;
}

err D_delete(Table *table){
	if(table == NULL){
		return ERR_NULL;
	}
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	err flag = delete_elem(table, key);
	free(key);
	return flag;
}

err D_find(Table *table){
	if(table == NULL){
		return ERR_NULL;
	}
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	Table *new_table = find(table, key);
	free(key);
	if(new_table == NULL){
		return ERR_VAL;
	}
	show_table(new_table);
	clear_table(new_table);
	free(new_table);
	return ERR_OK;
}

err D_show(Table *table){
	show_table(table);
	return ERR_OK;
}

char *my_strip(char const * const s){
	if(s == NULL){
		return NULL;
	}
	size_t i = 0;
	while((i < strlen(s)) && ((s[i] == ' ') || (s[i] == '\t'))){
		i++;
	}
	size_t count = 0;
	while(((i + count) < strlen(s)) && ((s[i + count] != ' ') && (s[i + count] != '\t'))){
		count++;
	}
	char *new_s = (char *)calloc(count + 1, sizeof(char));
	if(!new_s){
		return NULL;
	}
	memcpy(new_s, s + i, count * sizeof(char));
	return new_s;
}

FILE *input_correct_file(){
	char *str_file = readline("Введите название файла: ");
	char *clear_s_file = my_strip(str_file);
	FILE *file = fopen(clear_s_file, "r");
	free(str_file);
	free(clear_s_file);
	if(!file){
		printf("Unknown file\n");
		return NULL;
	}
	return file;
}

err D_input(Table *table){
	char *str_file = readline("Введите название файла: ");
	if(str_file == NULL){ return ERR_VAL; }
	char *clear_s_file = my_strip(str_file);
	FILE *file = fopen(clear_s_file, "rb");
	if(!file){
		free(str_file);
		free(clear_s_file);
		return ERR_VAL;
	}
	free(str_file);
	free(clear_s_file);
	if(!file){
		printf("Unknown file\n");
		return ERR_VAL;
	}
	err flag = input_bin(table, file);
	fclose(file);
	return flag;
}

err D_output(Table *table){
	char *str_file = readline("Введите название файла: ");
	if(str_file == NULL){ return ERR_VAL; }
	char *clear_s_file = my_strip(str_file);
	FILE *file = fopen(clear_s_file, "wb");
	if(!file){
		free(str_file);
		free(clear_s_file);
		return ERR_VAL;
	}
	free(str_file);
	free(clear_s_file);
	err flag = output_bin(table, file);
	fclose(file);
	return flag;
}
/*
err D_reorganize(Table *table){
	err flag = reorganize_table(table);
	show_table(table);
	return flag;
}

char str_isunsigned(char *s){
	if(strlen(s) == 0 || strlen(s) > 9){
		return 0;
	}
	for(size_t i = 0; i < strlen(s); i++){
		if(!isdigit(s[i])){
			return 0;
		}
	}
	return 1;
}
*/
