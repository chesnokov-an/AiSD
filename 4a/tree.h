#ifndef TREE_H
#define TREE_H

#include "err.h"

typedef struct Node{
	char *key;
	struct Node **children;
	unsigned *info;
	char is_thread;
}Node;

typedef struct Tree{
	Node *root;
}Tree;

Tree *create_tree();
err insert_elem(Tree * const, const char * const, const unsigned);
err delete_elem(Tree * const, const char * const);
void traversal(const Tree * const);
Node *find(const Tree * const, const char * const);
Node *spec_find(const Tree * const, const char * const);
void show(const Tree * const);
void draw(const Tree * const);
err import_tree(Tree *, FILE * const);
void clear_tree(Tree *);


#endif
