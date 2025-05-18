#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include "err.h"
#include "my_readline.h"
#include "tree.h"

#define ALL_WORDS 370105
#define AVERAGE 1000

int is_number(char *data){
	for(size_t i = 0; i < strlen(data); i++){
		if(!isdigit(data[i])){
			return 0;
		}
	}
	return 1;
}

int main(int argc, char **argv){
	if(argc != 3 || is_number(argv[1]) == 0 || is_number(argv[2]) == 0){
		printf("Передайте 2 параметра: количество элементов в дереве и количество усреднений.");
		return 0;
	}
	int size = atoi(argv[1]);
	int iters = atoi(argv[2]);

	FILE *file = fopen("words_alpha.txt", "r");
	if(!file){
		printf("Неизвестный файл");
		return 0;
	}
	char **keys = (char **)calloc(ALL_WORDS, sizeof(char *));
	for(int i = 0; i < ALL_WORDS; i++){
		keys[i] = txt_readline(file);
	}
	fclose(file);

	Tree **data = (Tree **)calloc(iters, sizeof(Tree *));
	char *info = NULL;
	char ***cur_keys = (char ***)calloc(iters, sizeof(char **));

	// Заполнение дерева перед таймированием
	for(int j = 0; j < iters; j++){
		data[j] = create_tree();
		cur_keys[j] = (char **)calloc(size, sizeof(char *));
		for(int i = 0; i < size; i++){
			cur_keys[j][i] = keys[rand() % ALL_WORDS];
			insert_elem(data[j], cur_keys[j][i], cur_keys[j][i]);
		}
	}

	// вставка
	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);
	char *key = NULL;
	for(int j = 0; j < iters; j++){
		for(int i = 0; i < AVERAGE; i++){
			if(i%2 == 0){
				key = keys[rand() % ALL_WORDS];
			}
			else{
				key = cur_keys[j][rand() % size];
			}
			info = key;
			insert_elem(data[j], key, info);
		}
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ВСТАВКИ %d элементов в дерево составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	// поиск
	gettimeofday(&tv1, NULL);
	for(int j = 0; j < iters; j++){
		for(int i = 0; i < AVERAGE; i++){
			if(i%2 == 0){
				key = keys[rand() % ALL_WORDS];
			}
			else{
				key = cur_keys[j][rand() % size];
			}
			find(data[j], key);
		}
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ПОИСКА в дереве из %d элементов составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	// Удаление
	gettimeofday(&tv1, NULL);
	for(int j = 0; j < iters; j++){
		for(int i = 0; i < AVERAGE; i++){
			if(i%2 == 0){
				key = keys[rand() % ALL_WORDS];
			}
			else{
				key = cur_keys[j][rand() % size];
			}
			delete_elem(data[j], key);
		}
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время УДАЛЕНИЯ из деревв из %d элементов составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));


	gettimeofday(&tv1, NULL);
	for(int j = 0; j < iters; j++){
		traversal(data[j], NULL);
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ОБХОДА дерева составило %lf секунд.\n", iters, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));


	for(int j = 0; j < iters; j++){
		clear_tree(data[j]);
		free(data[j]);
	}
	free(data);
	for(int j = 0; j < iters; j++){
		free(cur_keys[j]);
	}

	free(cur_keys);
	for(int i = 0; i < ALL_WORDS; i++){
		free(keys[i]);
	}
	free(keys);
	return 0;
}
