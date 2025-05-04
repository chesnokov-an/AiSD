#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include "err.h"
#include "my_readline.h"
#include "generate.h"
#include "tree.h"

int main(int argc, char **argv){
	if(argc != 3){
		printf("Передайте 2 параметра: количество элементов в дереве и количество усреднений.");
		return 0;
	}
	int size = atoi(argv[1]);
	int iters = atoi(argv[2]);

	FILE *file = fopen("russian_nouns.txt", "r");
	char **keys = (char **)calloc(51301, sizeof(char *));
	for(int i = 0; i < 51301; i++){
		keys[i] = txt_readline(file);
	}
	fclose(file);

	Tree *tree = create_tree();
	unsigned info = 0;
	char *key = NULL;
	for(int i = 0; i < size; i++){
		key = keys[rand() % 51301];
		info = rand() % UINT_MAX;
		insert_elem(tree, key, info);
	}

	// вставка
	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);
	for(int i = 0; i < iters; i++){
		key = keys[rand() % 51301];
		info = rand() % UINT_MAX;
		insert_elem(tree, key, info);

	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ВСТАВКИ %d элементов в дерево составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	// поиск
	gettimeofday(&tv1, NULL);
	for(int i = 0; i < iters; i++){
		key = keys[rand() % 51301];
		find(tree, key);
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время ПОИСКА в дереве из %d элементов составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	// Удаление
	gettimeofday(&tv1, NULL);
	for(int i = 0; i < iters; i++){
		key = keys[rand() % 51301];
		delete_elem(tree, key);
	}
	gettimeofday(&tv2, NULL);
	printf("Усреднённое за %d итераций время УДАЛЕНИЯ из деревв из %d элементов составило %lf секунд.\n", iters, size, (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

	clear_tree(tree);
	free(tree);
	for(int i = 0; i < 51301; i++){
		free(keys[i]);
	}
	free(keys);
	return 0;
}
