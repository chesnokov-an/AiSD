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

int main(int argc, char **argv){
	if(argc != 3){
		printf("Передайте 2 параметра: количество элементов в дереве и количество усреднений.");
		return 0;
	}
	int size = atoi(argv[1]);
	int iters = atoi(argv[2]);

	FILE *file = fopen("words_alpha.txt", "r");
	char **keys = (char **)calloc(370105, sizeof(char *));
	for(int i = 0; i < 370105; i++){
		keys[i] = txt_readline(file);
	}
	fclose(file);

	Tree **data = (Tree **)calloc(iters, sizeof(Tree *));
	unsigned info = 0;
	char ***cur_keys = (char ***)calloc(iters, sizeof(char **));

	for(int j = 0; j < iters; j++){
		data[j] = create_tree();
		cur_keys[j] = (char **)calloc(size, sizeof(char *));
		for(int i = 0; i < size; i++){
			cur_keys[j][i] = keys[rand() % 370105];
			info = rand() % UINT_MAX;
			insert_elem(data[j], cur_keys[j][i], info);
		}
	}

	// вставка
	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);
	char *key = NULL;
	for(int j = 0; j < iters; j++){
		for(int i = 0; i < 1000; i++){
			if(i%2 == 0){
				key = keys[rand() % 370105];
			}
			else{
				key = cur_keys[j][rand() % size];
			}
			info = rand() % UINT_MAX;
			insert_elem(data[j], key, info);
		}
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ВСТАВКИ %d элементов в дерево составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	// поиск
	gettimeofday(&tv1, NULL);
	for(int j = 0; j < iters; j++){
		for(int i = 0; i < 1000; i++){
			if(i%2 == 0){
				key = keys[rand() % 370105];
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
		for(int i = 0; i < 1000; i++){
			if(i%2 == 0){
				key = keys[rand() % 370105];
			}
			else{
				key = cur_keys[j][rand() % size];
			}
			delete_elem(data[j], key);
		}
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время УДАЛЕНИЯ из деревв из %d элементов составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));


	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);
	char *key = NULL;
	for(int j = 0; j < iters; j++){
		traversal(data[j]);
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ОБХОДА дерева составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));


	for(int j = 0; j < iters; j++){
		clear_tree(data[j]);
		free(data[j]);
	}
	free(data);
	for(int j = 0; j < iters; j++){
		free(cur_keys[j]);
	}

	free(cur_keys);
	for(int i = 0; i < 370105; i++){
		free(keys[i]);
	}
	free(keys);
	return 0;
}
