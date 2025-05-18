#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

#define GREEN "\033[38;2;0;255;0m"
#define RESET "\033[0;0m"

/*----------------CREATE----------------*/
Graph *create_graph(const size_t capacity){
	Graph *graph = (Graph *)calloc(1, sizeof(Graph));
	if(graph == NULL){ return NULL; }
	graph->array = (Node **)calloc(capacity, sizeof(Node *));
	if(graph->array == NULL){
		free(graph);
		return NULL;
	}
	graph->capacity = capacity;
	return graph;
}

Node *create_node(){ return (Node *)calloc(1, sizeof(Node)); }
Edge *create_node(){ return (Edge *)calloc(1, sizeof(Edge)); }

/*----------------CLEAR----------------*/
void clear_graph(Graph *graph){
	for(size_t i = 0; i < graph->size; i++){
		free_node(graph->array[i]);
	}
	graph->size = 0;
}

void clear_node(Node *node){
	Edge *edge = node->edges;
	Edge *pre_edge = NULL;
	while(edge != NULL){
		pre_edge = edge;
		edge = edge->next;
		free_edge(edge);
	}
	node->edge = NULL;
}

void clear_edge(Node *edge){
	free(edge->id);
	edge->id = NULL;
	edge->length = 0;
	edge->next = NULL;
}

/*----------------FREE----------------*/
void free_graph(Graph *graph){
	clear_graph(graph);
	free(graph->array);
	free(graph);
}

void free_node(Node *node){
	clear_node(node);
	free(node->id);
	free(node);
}

void free_edge(Edge *edge){
	clear_edge(edge);
	free(edge);
}

/*----------------MAIN FUNCTIONS----------------*/
err insert_node(Graph *graph, const char * const id, const room_type room){
	if(graph == NULL || graph->array == NULL || id == NULL){ return ERR_NULL; }
	err flag = ERR_OK;
	if(graph->size == graph->capacity){
		flag = resize(graph);
		if(flag != ERR_OK){ return flag; }
	}
	if(find_id(id) != NULL){ return ERR_VAL; }
	Node *node = create_node();
	if(node == NULL){ return ERR_MEM; }
	node->id = strdup(id);
	node->room = room;
	graph->array[graph->size] = node;
	graph->size++;
	return ERR_OK;
}

err modify_node(Graph *graph, const char * const id, const room_type room);
err remove_node(Graph *graph, const char * const id);

err insert_edge(Graph *graph, const char * const id1, const char * const id2, const unsigned edge);
err modify_edge(Graph *graph, const char * const id1, const char * const id2, const unsigned edge);
err remove_edge(Graph *graph, const char * const id1, const char * const id2);


/*----------------ADDITIONAL FUNCTIONS----------------*/
err resize(Graph *graph){
	if(graph == NULL || graph->array == NULL){ return ERR_NULL; }
	if(graph->size == graph->capacity){
		graph->array = (Node **)realloc(graph->capacity * 2, sizeof(Node *));
		graph->capacity *= 2;
	}
	if(graph->array == NULL){ return ERR_MEM; }
	return ERR_OK;
}

char *find_id(Graph *graph, const char * const id){
	for(size_t i = 0; i < graph->size; i++){
		if(strcmp(graph->array[i]->id, id) == 0){
			return graph->array[i]->id;
		}
	}
	return NULL;
}
