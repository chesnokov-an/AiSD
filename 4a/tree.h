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

err insert_elem(Tree * const, const char * const, const unsigned);
err delete_elem(Tree * const, const char * const);
void firmware(Tree * const tree);
void traversal(const Tree * const);
Node *find(const Tree * const, const char * const);
Node *spec_find(const Tree * const, const char * const);
void show(const Tree * const);
void graph(const Tree * const);
void import_tree(const Tree * const, FILE *);
void clear_tree(Tree *);


#endif
