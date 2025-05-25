#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <limits.h>
#include <ctype.h>
#include "err.h"
#include "input.h"
#include "graph.h"
#include "dialog_graph.h"
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

void Dialog(int capacity){
	err (*func_array[])(Graph *) = {D_insert_node, D_insert_edge, D_remove_node, D_remove_edge, D_modify_node, D_modify_edge, D_show, D_draw};
	Graph *graph = create_graph(capacity);
	int option = -1;
	err flag = ERR_OK;
	do{
		printf(MAGENTA"\n\n---------- МЕНЮ ----------\nВозможные команды:\n\n"RESET);
		printf(GREEN"1: добавление новой вершины\n"RESET);
		printf(GREEN"2: добавление нового ребра между двумя заданными вершинами\n"RESET);
		printf(RED"3: удаление заданной вершины\n"RESET);
		printf(RED"4: удаление заданного ребра\n"RESET);
		printf(YELLOW"5: изменение данных, ассоциированных с заданной вершиной\n"RESET);
		printf(YELLOW"6: изменение данных, ассоциированных с заданным ребром\n"RESET);
		printf(BLUE"7: вывод графа в виде списков смежности\n"RESET);
		printf(BLUE"8: графический вывод графа\n"RESET);
		printf(RED"\n0: Завершение программы\n\n"RESET);
		printf(MAGENTA"Выберите опцию: "RESET);
		flag = input_int(&option, 0, 8);
		printf("\n");
		if(flag == ERR_EOF || option == 0){ goto end_program; }
		flag = func_array[option-1](graph);

		if(flag != ERR_OK){ message(flag); }
	}while(option != 0);
	goto end_program;

end_program:
	free_graph(graph);
	return;
}

err D_insert_node(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	int room = 0;
	printf("Введите тип комнаты (Вход 1, Выход -1, Коридор 0): ");
	err flag = input_int(&room, -1, 1);
	if(flag != ERR_OK){ return flag; }

	char *id = readline("Введите название вершины: ");
	if(id == NULL){ return ERR_EOF; }
	flag = insert_node(graph, id, (room_type)room);
	free(id);
	return flag;
}

err D_insert_edge(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id_from = readline("Введите начало ребра: ");
	if(id_from == NULL){ return ERR_EOF; }
	char *id_to = readline("Введите конец ребра: ");
	if(id_to == NULL){
		free(id_from);
		return ERR_EOF;
	}
	int length = 0;
	printf("Введите длинну ребра: ");
	err flag = input_int(&length, 1, INT_MAX);
	if(flag != ERR_OK){
		free(id_from);
		free(id_to);
		return flag;
	}
	flag = insert_edge(graph, id_from, id_to, length);
	free(id_from);
	free(id_to);
	return flag;
}

err D_remove_node(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id = readline("Введите название вершины: ");
	if(id == NULL){ return ERR_EOF; }
	err flag = remove_node(graph, id);
	free(id);
	return flag;
}

err D_remove_edge(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id_from = readline("Введите начало ребра: ");
	if(id_from == NULL){ return ERR_EOF; }
	char *id_to = readline("Введите конец ребра: ");
	if(id_to == NULL){
		free(id_from);
		return ERR_EOF;
	}
	err flag = remove_edge(graph, id_from, id_to);
	free(id_from);
	free(id_to);
	return flag;
}

err D_modify_node(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id = readline("Введите название вершины: ");
	if(id == NULL){ return ERR_EOF; }
	int room = 0;
	printf("Введите новый тип комнаты (Вход 1, Выход -1, Коридор 0): ");
	err flag = input_int(&room, -1, 1);
	if(flag != ERR_OK){
		free(id);
		return flag;
	}
	flag = modify_node(graph, id, (room_type)room);
	free(id);
	return flag;
}

err D_modify_edge(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id_from = readline("Введите начало ребра: ");
	if(id_from == NULL){ return ERR_EOF; }
	char *id_to = readline("Введите конец ребра: ");
	if(id_to == NULL){
		free(id_from);
		return ERR_EOF;
	}
	int length = 0;
	printf("Введите длинну ребра: ");
	err flag = input_int(&length, 1, INT_MAX);
	if(flag != ERR_OK){
		free(id_from);
		free(id_to);
		return flag;
	}
	flag = modify_edge(graph, id_from, id_to, length);
	free(id_from);
	free(id_to);
	return flag;
}



/*
err D_spec_find(Graph *graph){
	printf("Введите значение: ");
	char *key = readline("Введите ключ: ");
	if(key == NULL){
		return ERR_EOF;
	}
	Node *node = spec_find(graph, key);
	free(key);
	if(node == NULL){
		return ERR_MEM;
	}

	printf("\n╔══════════════════════════╦═════════════════╗\n");
	printf("║           Ключ           ║     Значение    ║\n");
	printf("║══════════════════════════║═════════════════║\n");
	printf("║  %22s  ║  %14u ║\n", node->key, *node->info);
	printf("╚══════════════════════════╩═════════════════╝\n\n");
	return ERR_OK;
}
*/
err D_show(Graph *graph){
	show(graph);
	return ERR_OK;
}

err D_draw(Graph *graph){
	FILE *file = fopen("graph.dot", "wb");
	draw(graph, file);
	fclose(file);
	file = fopen("graph.dot", "rb");
	system("dot -Tx11 -Gdpi=500 -Gsize=100,100 graph.dot");
	fclose(file);
	return ERR_OK;
	return ERR_OK;
}
/*
err D_traversal(graph *graph){
	traversal(graph);
	return ERR_OK;
}

err D_input(graph *graph){
	FILE *file = input_correct_file();
	if(!file){
		return ERR_VAL;
	}
	if(!file){
		printf("Unknown file\n");
		return ERR_VAL;
	}
	err flag = import_graph(graph, file);
	fclose(file);
	return flag;
}*/
