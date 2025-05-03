#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tree.h"

char *generate_key(){
	int key_len = rand() % 10 + 1;
	char *key = (char *)calloc(key_len + 1, sizeof(char));
	for(int j = 0; j < key_len; j++){
		key[j] = (rand() % 26) + 97;
	}
	return key;
}

Tree *generate_tree(int seed, int size){
	srand(time(NULL) + seed);
	Tree *tree = create_tree();
	char *key = NULL;
	unsigned info = 0;
	for(int i = 0; i < size; i++){
		key = generate_key();
		info = rand() % 10 + 1;
		insert_elem(tree, key, info);
		free(key);
	}
	return tree;
}
