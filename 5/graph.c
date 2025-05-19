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
	if(capacity == 0){ return graph; }
	graph->array = (Node **)calloc(capacity, sizeof(Node *));
	if(graph->array == NULL){
		free(graph);
		return NULL;
	}
	graph->capacity = capacity;
	return graph;
}

Node *create_node(){ return (Node *)calloc(1, sizeof(Node)); }
Edge *create_edge(){ return (Edge *)calloc(1, sizeof(Edge)); }

/*----------------CLEAR----------------*/
void clear_graph(Graph *graph){
	if(graph == NULL || graph->array == NULL){ return; }
	for(size_t i = 0; i < graph->size; i++){
		free_node(graph->array[i]);
	}
	graph->size = 0;
}

void clear_node(Node *node){
	if(node == NULL){ return; }
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
	if(edge == NULL){ return; }
	free(edge->node);
	edge->node = NULL;
	edge->length = 0;
	edge->next = NULL;
}

/*----------------FREE----------------*/
void free_graph(Graph *graph){
	if(graph == NULL){ return; }
	clear_graph(graph);
	if(graph->array != NULL){ free(graph->array); }
	free(graph);
}

void free_node(Node *node){
	if(node == NULL){ return; }
	clear_node(node);
	if(id != NULL){ free(node->id); }
	free(node);
}

void free_edge(Edge *edge){
	if(edge == NULL){ return; }
	clear_edge(edge);
	free(edge);
}

/*----------------MAIN FUNCTIONS----------------*/
err insert_node(Graph *graph, const char * const id, const room_type room){
	if(graph == NULL || graph->array == NULL || id == NULL){ return ERR_NULL; }
	if(find_node(graph, id) != NULL){ return ERR_VAL; }
	err flag = ERR_OK;
	if(graph->size == graph->capacity){
		flag = resize(graph);
		if(flag != ERR_OK){ return flag; }
	}
	Node *node = create_node();
	if(node == NULL){ return ERR_MEM; }
	node->id = strdup(id);
	node->room = room;
	graph->array[graph->size] = node;
	graph->size++;
	return ERR_OK;
}

err modify_node(Graph *graph, const char * const id, const room_type room){
	Node *node = find_node(graph, id);
	if(node == NULL){ return ERR_NO_ELEM; }
	node->room = room;
	return ERR_OK;
}

err remove_node(Graph *graph, const char * const id){
	// pass
}

err insert_edge(Graph *graph, const char * const id_from, const char * const id_to, const unsigned length){
	if(graph == NULL || graph->array == NULL || id_from == NULL || id_to == NULL){ return ERR_NULL; }
	if(find_edge(graph, id_from, id_to) != NULL){ return ERR_VAL; }

	Node *node_from = find_node(graph, id_from);
	Node *node_to = find_node(graph, id_to);
	Edge *edge = create_edge();
	edge->length = length;
	edge->node = node_to;
	edge->next = node_from->edges;
	node_from->edges = edge;
}

err modify_edge(Graph *graph, const char * const id_from, const char * const id_to, const unsigned length){
	Edge *edge = find_edge(graph, id_from, id_to);
	if(edge == NULL){ return ERR_NO_ELEM; }
	edge->length = length;
	return ERR_OK;
}

err remove_edge(Graph *graph, const char * const id_from, const char * const id_to){
	// pass
}

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

Node *find_node(Graph *graph, const char * const id){
	for(size_t i = 0; i < graph->size; i++){
		if(strcmp(graph->array[i]->id, id) == 0){
			return graph->array[i];
		}
	}
	return NULL;
}

Node *find_edge(Graph *graph, const char * const id_from, const char * const id_to){
	Node *node_from = find_node(graph, id_from);
	Edge *edge = node->edges;
	while(edge != NULL){
		if(strcmp(edge->node->id, id_to) == 0){
			return edge;
		}
		edge = edge->next;
	}
	return NULL;
}
