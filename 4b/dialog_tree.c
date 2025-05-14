#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <limits.h>
#include <ctype.h>
#include "err.h"
#include "input.h"
#include "tree.h"
#include "dialog_tree.h"
#include "my_readline.h"

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
		break;
	case ERR_EMPTY:
		printf(RED"\nДерево пусто.\n"RESET);
		break;
	case ERR_EOF:
		printf(RED"\nЗавершение программы.\n"RESET);
		break;
	case ERR_NO_ELEM:
		printf(RED"\nНет такого элемента.\n"RESET);
		break;
	case ERR_OK:
		break;
	}
}

void Dialog(){
	err (*func_array[])(Tree *) = {D_insert, D_delete, D_find, D_spec_find, D_show, D_draw, D_traversal, D_input};
	Tree *tree = create_tree();
	int option = -1;
	err flag = ERR_OK;
	do{
		printf(MAGENTA"\n\n---------- МЕНЮ ----------\nВозможные команды:\n\n"RESET);
		printf("0: Завершение программы.\n"GREEN"1: Вставка нового элемента.\n"RED"2: Удаление элемента.\n"BLUE"3: Поиск элемента.\n"BLUE"4: Спец. поиск.\n"LIGHT"5: Вывод содержимого дерева.\n"LIGHT"6: Графический вывод дерева.\n\n"YELLOW"7: Обход дерева.\n"ORANGE"8: Импорт дерева из текстового файла.\n\n"RESET);
		printf(MAGENTA"Выберите опцию: "RESET);
		flag = input_int(&option, 0, 8);
		printf("\n");
		if(flag == ERR_EOF || option == 0){ goto end_program; }
		flag = func_array[option-1](tree);

		if(flag != ERR_OK){ message(flag); }
	}while(option != 0);
	goto end_program;

end_program:
	clear_tree(tree);
	free(tree);
	return;
}

err D_insert(Tree *tree){
	if(tree == NULL){
		return ERR_NULL;
	}
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	char *info = readline("Введите ключ: ");
	if(info == NULL){
		free(key);
		return ERR_EOF;
	}
	err flag = insert_elem(tree, key, info);
	free(key);
	free(info);
	return flag;
}

err D_delete(Tree *tree){
	if(tree == NULL){
		return ERR_NULL;
	}
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	err flag = delete_elem(tree, key);
	free(key);
	return flag;
}

err D_find(Tree *tree){
	if(tree == NULL){
		return ERR_NULL;
	}
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	Node *node = find(tree, key);
	free(key);
	if(node == NULL){
		return ERR_NO_ELEM;
	}
	printf("\nКлюч: [\"%s\", \"%s\"]\nЗначение: [\"%s\", \"%s\"]\n", node->key[0], node->key[1], node->info[0], node->info[1]);
	return ERR_OK;
}


err D_spec_find(Tree *tree){
	printf("Введите значение: ");
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	Node *node = spec_find(tree, key);
	free(key);
	if(node == NULL){
		return ERR_MEM;
	}

	printf("\nКлюч: [\"%s\", \"%s\"]\nЗначение: [\"%s\", \"%s\"]\n", node->key[0], node->key[1], node->info[0], node->info[1]);
	return ERR_OK;
}

err D_show(Tree *tree){
	show(tree);
	return ERR_OK;
}

err D_draw(Tree *tree){
	/*FILE *file = fopen("tree.dot", "wb");
	draw(tree, file);
	fclose(file);
	file = fopen("tree.dot", "rb");
	system("dot -Tx11 -Gdpi=500 -Gsize=100,100 tree.dot");
	fclose(file);
	return ERR_OK;*/
}

err D_traversal(Tree *tree){
	if(tree == NULL){
		return ERR_NULL;
	}
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	err flag = traversal(tree, key);
	free(key);
	return flag;
}

err D_input(Tree *tree){
	FILE *file = input_correct_file();
	if(!file){
		return ERR_VAL;
	}
	if(!file){
		printf("Unknown file\n");
		return ERR_VAL;
	}
	err flag = import_tree(tree, file);
	fclose(file);
	return flag;
}
