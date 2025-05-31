#ifndef GRAPH_H
#define GRAPH_H

#include "err.h"

typedef enum room_type{
	ENTRY = 1,
	EXIT = -1,
	PASS = 0,
}room_type;

typedef struct Edge Edge;

typedef struct Node Node;

typedef struct Graph Graph;

Graph *create_graph(const size_t capacity);
Node *create_node();
Edge *create_edge();

void clear_graph(Graph *graph);
void clear_node(Node *node);
void clear_edge(Edge *edge);

void free_graph(Graph *graph);
void free_node(Node *node);
void free_edge(Edge *edge);

size_t get_size(Graph *graph);
char *get_id(Node *node);
room_type get_room(Node *node);
unsigned get_length(Edge *edge);
char *get_id_to(Edge *edge);

err insert_node(Graph *graph, const char * const id, const room_type room);
err modify_node(Graph *graph, const char * const id, const room_type room);
err remove_node(Graph *graph, const char * const id);

err insert_edge(Graph *graph, const char * const id_from, const char * const id_to, const unsigned length);
err modify_edge(Graph *graph, const char * const id_from, const char * const id_to, const unsigned length);
err remove_edge(Graph *graph, const char * const id_from, const char * const id_to);

void show(Graph *graph);
void draw(Graph *graph, FILE * const file, char **path);

err traversal(Graph *graph, const char * const id_from);
char **shortest_path(Graph *graph, const char * const id_from, const char * const id_to, unsigned *path_length);
Node *nearest_exit(Graph *graph, const char * const id_from, unsigned *length);

#endif
