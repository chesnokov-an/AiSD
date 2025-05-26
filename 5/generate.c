#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "err.h"
#include "graph.h"

#define NODE_COUNT 1000
#define EDGE_COUNT 1000
#define MAX_LENGTH 1000
int main(){
	FILE *file = fopen("test.txt", "w");
	fprintf(file, "DWRF\n");
	fprintf(file, "%d\n", NODE_COUNT);
	char **id_array = (char **)calloc(NODE_COUNT, sizeof(char *));
	int room = 0;
	for(int i = 0; i < NODE_COUNT; i++){
		int id_len = rand() % 12 + 2;
		id_array[i] = (char *)calloc(id_len, sizeof(char));
		for(int j = 0; j < id_len - 1; j++){
			id_array[i][j] = (rand() % 26) + 97;
		}
		room = (rand() % 3) - 1;
		fprintf(file, "%s %d\n", id_array[i], room);
	}
	fprintf(file, "%d\n", EDGE_COUNT);
	char *id_from = NULL;
	char *id_to = NULL;
	int length = 0;
	for(int i = 0; i < EDGE_COUNT; i++){
		id_from = id_array[rand() % NODE_COUNT];
		id_to = id_array[rand() % NODE_COUNT];
		length = (rand() % (MAX_LENGTH - 1)) + 1;
		fprintf(file, "%s %s %d\n", id_from, id_to, length);
	}
	fclose(file);

	for(int i = 0; i < NODE_COUNT; i++){
		free(id_array[i]);
	}
	free(id_array);
	return 0;
}
