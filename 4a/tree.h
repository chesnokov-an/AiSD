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

Node *find(const Tree * const, const keytype const);
err insert_elem(Tree * const, const keytype const, const infotype);
err delete_elem(Tree * const, const keytype const);

#endif
