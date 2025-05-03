#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include "err.h"
#include "generate.h"
#include "tree.h"

int main(int argc, char **argv){
	if(argc != 3){
		printf("Передайте 2 параметра: количество элементов в дереве и количество усреднений.");
		return 0;
	}
	int size = atoi(argv[1]);
	int iters = atoi(argv[2]);
	Tree **data = (Tree **)calloc(iters, sizeof(Tree *));

	// вставка
	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);
	for(int j = 0; j < iters; j++){
		data[j] = generate_tree(j, size);
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ВСТАВКИ %d элементов в дерево составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	// поиск
	gettimeofday(&tv1, NULL);
	char *key = NULL;
	for(int j = 0; j < iters; j++){
		key = generate_key();
		find(data[j], key);
		free(key);
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ПОИСКА в дереве из %d элементов составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	// Удаление
	gettimeofday(&tv1, NULL);
	for(int j = 0; j < iters; j++){
		key = generate_key();
		delete_elem(data[j], key);
		free(key);
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время УДАЛЕНИЯ из деревв из %d элементов составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));





	for(int j = 0; j < iters; j++){
		clear_tree(data[j]);
		free(data[j]);
	}
	free(data);


	return 0;
}
