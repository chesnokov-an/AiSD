#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define left children[0]
#define right children[1]


Tree *create_tree(){
	Tree *tree = (Tree *)calloc(1, sizeof(Tree));
	return tree;
}

Node *create_node(const keytype const key, const infotype info){
	Node *node = (Node *)calloc(1, sizeof(Node));
	if(node == NULL){ return NULL; }
	node->children = (Node **)calloc(1, sizeof(Node *));
	if(node->children == NULL){
		free(node);
		return NULL;
	}
	node->info = (infotype *)calloc(1, sizeof(infotype));
	if(node->info == NULL){
		free(node->children);
		free(node);
		return NULL;
	}
	node->key = strdup(key);
	*(node->info) = info;
	return node;
}

char cmp(keytype key1, keytype key2){
	return strcmp(key1, key2);
}

Node *find(const Tree * const tree, const keytype const key){
	Node *node = tree->root;
	char cmp_val = 0;
	while(node != NULL){
		cmp_val = cmp(node->key, key);
		if(cmp_val == 1){
			return node;
		}
		if(cmp_val > 0){
			node = node->left;
		}
		else{
			node = node->right;
		}
	}
	return node;
}

err insert_elem(Tree * const tree, const keytype const key, const infotype info){
	if(tree == NULL){ return ERR_NULL; }
	if(tree->root == NULL){
		tree->root = create_node(key, info);
		if(tree->root == NULL){ return ERR_MEM; }
	}
	Node *node = tree->root;
	Node *pre_node = NULL;
	char cmp_val = 0;
	while(node != NULL){
		cmp_val = cmp(node->key, key);
		if(cmp_val == 1){
			return ERR_VAL;
		}
		pre_node = node;
		if(cmp_val > 0){
			node = node->left;
		}
		else{
			node = node->right;
		}
	}
	if(cmp_val > 0){
		pre_node->left = create_node(key, info);
		if(pre_node->left == NULL){ return ERR_MEM; }
	}
	else{
		pre_node->right = create_node(key, info);
		if(pre_node->right == NULL){ return ERR_MEM; }
	}
	return ERR_OK;
}
/*
err delete_elem(Tree * const tree, const keytype const key){
	if(tree == NULL){ return ERR_NULL; }
	if(tree->root == NULL){
		return ERR_EMPTY;
	}


}*/
