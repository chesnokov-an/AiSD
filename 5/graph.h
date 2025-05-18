#ifndef GRAPH_H
#define GRAPH_H

#include "err.h"

typedef enum room_type{
	entry = 1,
	exit = -1,
	pass = 0,
}room_type;

typedef struct Edge{
	const char * const * id;
	unsigned length;
	struct Edge *next;
}Edge;

typedef struct Node{
	char *id;
	room_type room;
	Edge *edges;
}Node;

typedef struct Graph{
	size_t capacity;
	size_t size;
	Node **array;
}Graph;

Graph *create_graph(const size_t capacity);
Node *create_node();
void clear_graph(Graph *graph);
void clear_node(Node *node);
void free_graph(Graph *graph);
void free_node(Node *node);
err insert_node(Graph *graph, const char * const id, const room_type room);
err modify_node(Graph *graph, const char * const id, const room_type room);
err remove_node(Graph *graph, const char * const id);
err insert_edge(Graph *graph, const char * const id1, const char * const id2, const unsigned edge);
err modify_edge(Graph *graph, const char * const id1, const char * const id2, const unsigned edge);
err remove_edge(Graph *graph, const char * const id1, const char * const id2);
void show(Graph *graph);
void draw(Graph *graph);

#endif
