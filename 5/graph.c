#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include "graph.h"

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

#define GREEN "\033[38;2;0;255;0m"
#define BLUE "\033[38;2;0;191;255m"
#define YELLOW "\033[38;2;255;255;0m"
#define RESET "\033[0;0m"


typedef struct Edge{
	Node **node;
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
		free_edge(pre_edge);
	}
	node->edges = NULL;
}

void clear_edge(Edge *edge){
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
	if(node->id != NULL){ free(node->id); }
	free(node);
}

void free_edge(Edge *edge){
	if(edge == NULL){ return; }
	clear_edge(edge);
	free(edge);
}

/*----------------ADDITIONAL FUNCTIONS----------------*/
err resize(Graph *graph){
	if(graph == NULL || graph->array == NULL){ return ERR_NULL; }
	if(graph->size == graph->capacity){
		graph->array = (Node **)realloc(graph->array, graph->capacity * 2 * sizeof(Node *));
		graph->capacity *= 2;
	}
	if(graph->array == NULL){ return ERR_MEM; }
	return ERR_OK;
}

Node *find_node(const Graph *const graph, const char * const id){
	for(size_t i = 0; i < graph->size; i++){
		if(strcmp(graph->array[i]->id, id) == 0){
			return graph->array[i];
		}
	}
	return NULL;
}

Edge *find_edge(const Graph * const graph, const char * const id_from, const char * const id_to){
	Node *node_from = find_node(graph, id_from);
	if(node_from == NULL){ return NULL; }
	Edge *edge = node_from->edges;
	while(edge != NULL){
		if(strcmp((*edge->node)->id, id_to) == 0){
			return edge;
		}
		edge = edge->next;
	}
	return NULL;
}

int index_in_graph(const Graph * const graph, const char * const id){
	for(size_t i = 0; i < graph->size; i++){
		if(strcmp(graph->array[i]->id, id) == 0){
			return (int)i;
		}
	}
	return -1;
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
	if(graph == NULL || id == NULL){ return ERR_NULL; }
	if(graph->size == 0){ return ERR_EMPTY; }
	int index = index_in_graph(graph, id);
	if(index == -1){ return ERR_NO_ELEM; }
	free_node(graph->array[index]);
	graph->array[index] = graph->array[graph->size-1];
	graph->array[graph->size-1] = NULL;
	graph->size -= 1;
	return ERR_OK;
}

err insert_edge(Graph *graph, const char * const id_from, const char * const id_to, const unsigned length){
	if(graph == NULL || graph->array == NULL || id_from == NULL || id_to == NULL){ return ERR_NULL; }
	if(find_edge(graph, id_from, id_to) != NULL){ return ERR_VAL; }
	Node *from = find_node(graph, id_from);
	Node *to = find_node(graph, id_to);
	if(from == NULL || to == NULL){ return ERR_NO_ELEM; }
	Edge *edge = create_edge();
	if(edge == NULL){ return ERR_MEM; }
	edge->length = length;
	edge->node = (Node **)calloc(1, sizeof(Node *));
	*edge->node = to;
	edge->next = from->edges;
	from->edges = edge;
	return ERR_OK;
}

err modify_edge(Graph *graph, const char * const id_from, const char * const id_to, const unsigned length){
	Edge *edge = find_edge(graph, id_from, id_to);
	if(edge == NULL){ return ERR_NO_ELEM; }
	edge->length = length;
	return ERR_OK;
}

err remove_edge(Graph *graph, const char * const id_from, const char * const id_to){
	if(graph == NULL || id_from == NULL || id_to == NULL){ return ERR_NULL; }
	if(graph->size == 0){ return ERR_EMPTY; }
	Node *from = find_node(graph, id_from);
	Node *to = find_node(graph, id_to);
	if(from == NULL || to == NULL){ return ERR_NO_ELEM; }
	Edge *prev_edge = NULL;
	Edge *edge = from->edges;
	while(edge != NULL){
		if(*edge->node == to){ break; }
		prev_edge = edge;
		edge = edge->next;
	}
	if(edge == NULL){ return ERR_NO_ELEM; }
	prev_edge->next = edge->next;
	free_edge(edge);
	return ERR_OK;
}

void show(Graph *graph){
	if(graph == NULL || graph->array == NULL){ return; }
	Node *node = NULL;
	Edge *edge = NULL;
	for(size_t i = 0; i < graph->size; i++){
		node = graph->array[i];
		printf(GREEN"\n\"%s\""RESET","BLUE" type: %d"RESET, node->id, node->room);
		printf((node->edges != NULL) ? (", edges:\n") : ("\n"));
		edge = node->edges;
		while(edge != NULL){
			printf(GREEN"\t\"%s\""RESET", "YELLOW"length: %d\n"RESET, (*edge->node)->id, edge->length);
			edge = edge->next;
		}
	}
}

void draw_node(Agraph_t *G, const Node * const node){
	if(node == NULL){ return; }
	Agnode_t *vertex = agnode(G, node->id, TRUE);
	if(node->room == ENTRY){ agsafeset(vertex, "shape", "square", ""); }
	else if(node->room == EXIT){ agsafeset(vertex, "shape", "star", ""); }
	else{ agsafeset(vertex, "shape", "circle", ""); }
}

void draw_edge(Agraph_t *G, const Node * const node){
	if(node == NULL){ return; }
	Agnode_t *vertex1 = agnode(G, node->id, FALSE);
	Agnode_t *vertex2 = NULL;
	Edge *edge = node->edges;
	while(edge != NULL){
		vertex2 = agnode(G, (*edge->node)->id, FALSE);
		char *name = (char *)calloc(strlen(node->id) + strlen((*edge->node)->id) + 1, sizeof(char));
		strcat(name, node->id);
		strcat(name, (*edge->node)->id);
		Agedge_t *edge_for_draw = agedge(G, vertex1, vertex2, name, TRUE);
		char length[11] = "";
		sprintf(length, "%d", edge->length);
		agsafeset(edge_for_draw, "label", length , "");
		edge = edge->next;
	}
}

void draw(Graph *graph, FILE * const file){
	if(graph == NULL){ return; }
	Agraph_t *G = agopen("G", Agdirected, NULL);

	agsafeset(G, "layout", "neato", "");
    agsafeset(G, "K", "0.5", "");
    agsafeset(G, "overlap", "false", "");
    agsafeset(G, "sep", "+10", "");

	if(graph->array != NULL){
		for(size_t i = 0; i < graph->size; i++){
			draw_node(G, graph->array[i]);
		}
		for(size_t i = 0; i < graph->size; i++){
			draw_edge(G, graph->array[i]);
		}
	}
	GVC_t *gvc = gvContext();
	gvLayout(gvc, G, "neato");
	gvRender(gvc, G, "dot", file);
	gvFreeLayout(gvc, G);
	agclose(G);
	gvFreeContext(gvc);
}
