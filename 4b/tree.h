#ifndef TREE_H
#define TREE_H

#include "err.h"

typedef struct Node{
	int size;
	char *key[3];	// the last one is fictitious for operations
	char *info[3];
	struct Node *children[5];	// the last one is a pointer to the parent
}Node;

typedef struct Tree{
	Node *root;
}Tree;

Tree *create_tree();
err insert_elem(Tree * const, const char * const, const char * const);
err delete_elem(Tree * const, const char * const);
void traversal(const Tree * const);
Node *find(const Tree * const, const char * const);
Node *spec_find(const Tree * const, const char * const);
void show(const Tree * const);
void draw(const Tree * const, FILE * const);
void write_to_txt(const Tree * const, FILE * const);
err import_tree(Tree *, FILE * const);
void clear_tree(Tree *);


#endif
