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
	err (*func_array[])(Graph *) = {D_insert_node, D_insert_edge, D_remove_node, D_remove_edge, D_modify_node, D_modify_edge, D_show, D_draw, D_import, D_generate, D_traversal, D_shortest_path, D_nearest_exit, D_additional_shortest_path};
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
		printf(ORANGE"9: импорт графа из текстового файла\n"RESET);
		printf(ORANGE"10: генерация рандомного графа\n"RESET);
		printf(YELLOW"\n11: проверка достижимости хотя бы одного из выходов из указанной точки входа\n"RESET);
		printf(YELLOW"12: поиск кратчайшего пути между указанным входом и указанным выходом\n"RESET);
		printf(YELLOW"13: определение ближайшего, к указанному входу, выхода и расстояния до него\n"RESET);
		printf(BLUE"\n14: отрисовка кратчайшего пути между указанным входом и указанным выходом\n"RESET);
		printf(RED"\n0: завершение программы\n\n"RESET);
		printf(MAGENTA"Выберите опцию: "RESET);
		flag = input_int(&option, 0, 14);
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
	unsigned length = 0;
	printf("Введите длинну ребра: ");
	err flag = input_uint(&length, 1, UINT_MAX);
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
	unsigned length = 0;
	printf("Введите длинну ребра: ");
	err flag = input_uint(&length, 1, UINT_MAX);
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

err D_import(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	FILE *file = input_correct_file();
	if(!file){
		printf("Unknown file\n");
		return ERR_VAL;
	}
	char *magic_word = txt_readline(file);
	if(magic_word == NULL){ return ERR_VAL; }
	if(strcmp("DWRF", magic_word) != 0){
		free(magic_word);
		return ERR_VAL;
	}
	free(magic_word);
	int node_count = 0;
	char *word = NULL;
	char *id = NULL;
	room_type room = PASS;
	err flag = txt_input_int(file, &node_count, 1, INT_MAX);
	char *line = NULL;
	if(flag != ERR_OK){
		fclose(file);
		return ERR_VAL;
	}
	for(int i = 0; i < node_count; i++){
		line = txt_readline(file);
		if(line == NULL){
			fclose(file);
			return ERR_VAL;
		}
		word = strtok(line, "\t ");
		id = strdup(word);
		word = strtok(NULL, "\t ");
		room = (room_type)atoi(word);
		insert_node(graph, id, room);
		free(line);
		free(id);
	}
	int edge_count = 0;
	flag = txt_input_int(file, &edge_count, 1, INT_MAX);
	if(flag != ERR_OK){
		fclose(file);
		return ERR_VAL;
	}
	char *id_from = NULL;
	char *id_to = NULL;
	unsigned length = 0;
	for(int i = 0; i < edge_count; i++){
		line = txt_readline(file);
		if(line == NULL){
			fclose(file);
			return ERR_VAL;
		}
		word = strtok(line, "\t ");
		id_from = strdup(word);
		word = strtok(NULL, "\t ");
		id_to = strdup(word);
		word = strtok(NULL, "\t ");
		length = atoi(word);
		insert_edge(graph, id_from, id_to, length);
		free(line);
		free(id_from);
		free(id_to);
	}
	fclose(file);
	return ERR_OK;
}

err D_generate(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	int node_count = 0;
	printf("Введите число вершин: ");
	err flag = input_int(&node_count, 0, INT_MAX);
	if(flag != ERR_OK){ return flag; }
	int edge_count = 0;
	printf("Введите число рёбер: ");
	flag = input_int(&edge_count, 0, node_count * node_count);
	if(flag != ERR_OK){ return flag; }

	char **id_array = (char **)calloc(node_count, sizeof(char *));
	int room = 0;
	for(int i = 0; i < node_count; i++){
		int id_len = rand() % 12 + 2;
		id_array[i] = (char *)calloc(id_len, sizeof(char));
		for(int j = 0; j < id_len - 1; j++){
			id_array[i][j] = (rand() % 26) + 97;
		}
		room = (rand() % 3) - 1;
		insert_node(graph, id_array[i], (room_type)room);
	}
	char *id_from = NULL;
	char *id_to = NULL;
	unsigned length = 0;
	for(int i = 0; i < edge_count; i++){
		id_from = id_array[rand() % node_count];
		id_to = id_array[rand() % node_count];
		length = (rand() % (1000 - 1)) + 1;
		insert_edge(graph, id_from, id_to, length);
	}
	for(int i = 0; i < node_count; i++){
		free(id_array[i]);
	}
	free(id_array);
	return ERR_OK;
}

err D_show(Graph *graph){
	show(graph);
	return ERR_OK;
}

err D_draw(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	FILE *file = fopen("graph.dot", "wb");
	draw(graph, file, NULL);
	fclose(file);
	file = fopen("graph.dot", "rb");
	system("dot -Tx11 -Gdpi=500 -Gsize=100,100 graph.dot");
	fclose(file);
	return ERR_OK;
}

err D_traversal(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id_from = readline("Введите вход (type 1): ");
	if(id_from == NULL){ return ERR_EOF; }
	err flag = traversal(graph, id_from);
	free(id_from);
	if(flag == ERR_OK){
		printf(GREEN"\nЕсть достижимые выходы!\n"RESET);
	}
	return flag;
}

err D_shortest_path(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id_from = readline("Введите вход (type 1): ");
	if(id_from == NULL){ return ERR_EOF; }
	char *id_to = readline("Введите выход (type -1): ");
	if(id_to == NULL){
		free(id_from);
		return ERR_EOF;
	}
	unsigned length = 0;
	char **path = shortest_path(graph, id_from, id_to, &length);
	free(id_from);
	free(id_to);
	if(path == NULL){ return ERR_NO_ELEM; }
	printf(GREEN"Длина пути: %u\n"RESET, length);
	printf(GREEN"Путь: "RESET);
	size_t size = get_size(graph);
	for(size_t i = 0; i < size; i++){
		if(path[i] == NULL){ break; }
		printf("%s", path[i]);
		if((i < (size - 1)) && (path[i + 1] != NULL)){ printf(" <- "); }
		free(path[i]);
	}
	free(path);
	return ERR_OK;
}

err D_nearest_exit(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id_from = readline("Введите вход (type 1): ");
	if(id_from == NULL){ return ERR_EOF; }
	unsigned length = 0;
	Node *exit = nearest_exit(graph, id_from, &length);
	free(id_from);
	if(exit == NULL){ return ERR_NO_ELEM; }
	char *exit_id = get_id(exit);
	printf(GREEN"\nБлижайший выход: %s\nРасстояние до него: %d\n"RESET, exit_id, length);
	free(exit_id);
	return ERR_OK;
}

err D_additional_shortest_path(Graph *graph){
	if(graph == NULL){ return ERR_NULL; }
	char *id_from = readline("Введите вход (type 1): ");
	if(id_from == NULL){ return ERR_EOF; }
	char *id_to = readline("Введите выход (type -1): ");
	if(id_to == NULL){
		free(id_from);
		return ERR_EOF;
	}
	unsigned length = 0;
	char **path = shortest_path(graph, id_from, id_to, &length);
	free(id_from);
	free(id_to);
	if(path == NULL){ return ERR_NO_ELEM; }

	FILE *file = fopen("graph.dot", "wb");
	draw(graph, file, path);
	fclose(file);
	file = fopen("graph.dot", "rb");
	system("dot -Tx11 -Gdpi=500 -Gsize=100,100 graph.dot");
	fclose(file);
	size_t size = get_size(graph);
	for(size_t i = 0; i < size; i++){
		if(path[i] == NULL){ break; }
		free(path[i]);
	}
	free(path);
	return ERR_OK;
}
