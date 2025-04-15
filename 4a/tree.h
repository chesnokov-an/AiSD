#ifndef TREE_H
#define TREE_H

typedef char* keytype;
typedef unsigned infotype;

typedef struct Node{
	keytype key;
	struct Node **children;
	infotype *info;
}Node;

typedef struct Tree{
	Node *root;
}Tree;

err insert_elem(Tree * const, const keytype const, const infotype);
err delete_elem(Tree * const, const keytype const);
void traversal(const Tree * const);
Node *find(const Tree * const, const keytype const);
Node *spec_find(const Tree * const, const keytype const);
void format_tree(const Tree * const);
void graph_tree(const Tree * const);
void import_tree(const Tree * const, FILE *);

#endif
