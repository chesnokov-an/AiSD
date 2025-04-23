#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tree.h"
#include "my_readline.h"
#include "input.h"

#define left children[0]
#define right children[1]
#define prev children[2]
#define next children[3]

Tree *create_tree(){
	Tree *tree = (Tree *)calloc(1, sizeof(Tree));
	return tree;
}

Node *create_node(const char * const key, const unsigned info){
	Node *node = (Node *)calloc(1, sizeof(Node));
	if(node == NULL){ return NULL; }
	node->children = (Node **)calloc(4, sizeof(Node *));
	if(node->children == NULL){
		free(node);
		return NULL;
	}
	node->info = (unsigned *)calloc(1, sizeof(unsigned));
	if(node->info == NULL){
		free(node->children);
		free(node);
		return NULL;
	}
	node->key = strdup(key);
	*(node->info) = info;
	node->is_thread = 0;
	return node;
}

void clear_node(Node *node){
	if(node == NULL){ return; }
	if(node->info != NULL){ free(node->info); }
	if(node->children != NULL){ free(node->children); }
	if(node->key != NULL){ free(node->key); }
	free(node);
}

char cmp(const char * const key1, const char * const key2){
	return strcmp(key1, key2);
}

Node *find(const Tree * const tree, const char * const key){
	Node *node = tree->root;
	char cmp_val = 0;
	while(node != NULL){
		cmp_val = cmp(node->key, key);
		if(cmp_val == 0){
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

err insert_elem(Tree * const tree, const char * const key, const unsigned info){
	if(tree == NULL){ return ERR_NULL; }
	if(tree->root == NULL){
		tree->root = create_node(key, info);
		if(tree->root == NULL){ return ERR_MEM; }
		return ERR_OK;
	}
	Node *node = tree->root;
	Node *pre_node = NULL;
	char cmp_val = 0;
	while(node != NULL){
		cmp_val = cmp(node->key, key);
		if(cmp_val == 0){
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
		pre_node->left->next = pre_node->next;
		pre_node->left->prev = pre_node;
		if(pre_node->next != NULL){
			pre_node->next->prev = pre_node->left;
		}
		pre_node->next = pre_node->left;
	}
	else{
		pre_node->right = create_node(key, info);
		if(pre_node->right == NULL){ return ERR_MEM; }
		pre_node->right->next = pre_node;
		pre_node->right->prev = pre_node->prev;
		if(pre_node->prev != NULL){
			pre_node->prev->next = pre_node->right;
		}
		pre_node->prev = pre_node->right;
	}
	return ERR_OK;
}

err delete_elem(Tree * const tree, const char * const key){
	if(tree == NULL){ return ERR_NULL; }
	if(tree->root == NULL){
		return ERR_EMPTY;
	}
	// Находим элемент с таким ключом и его родителя
	Node *node = tree->root;
	Node *pre_node = NULL;
	char left_flag = -1;
	char cmp_val = 0;
	while(node != NULL){
		cmp_val = cmp(node->key, key);
		if(cmp_val == 0){
			break;
		}
		pre_node = node;
		if(cmp_val > 0){
			node = node->left;
			left_flag = 1;
		}
		else{
			node = node->right;
			left_flag = 0;
		}
	}
	if(node == NULL){
		return ERR_NO_ELEM;
	}
	// Удаляем, если нет потомков
	if((node->left == NULL) && (node->right == NULL)){
		if(pre_node == NULL){
			tree->root = NULL;
			goto end_of_delete;
		}
		if(left_flag == 1){
			pre_node->left = NULL;
		}
		else{
			pre_node->right = NULL;
		}
		goto end_of_delete;
		return ERR_OK;
	}
	// Удаляем, если один потомок
	if(node->left == NULL){
		if(left_flag == -1){
			tree->root = node->right;
		}
		else if(left_flag == 1){
			pre_node->left = node->right;
		}
		else{
			pre_node->right = node->right;
		}
		goto end_of_delete;
		return ERR_OK;
	}
	if(node->right == NULL){
		if(left_flag == -1){
			tree->root = node->left;
		}
		else if(left_flag == 1){
			pre_node->left = node->left;
		}
		else{
			pre_node->right = node->left;
		}
		goto end_of_delete;
		return ERR_OK;
	}
	// Удаляем, если два потомка
	Node *min_right = node->right;
	Node *pre_min = NULL;
	while(min_right->left != NULL){
		pre_min = min_right;
		min_right = min_right->left;
	}
	if(pre_min == NULL){
		if(left_flag == -1){
			tree->root = min_right;
		}
		else if(left_flag == 1){
			pre_node->left = min_right;
		}
		else{
			pre_node->right = min_right;
		}
		min_right->left = node->left;
		goto end_of_delete;
		return ERR_OK;
	}
	if(left_flag == -1){
		tree->root = min_right;
	}
	else if(left_flag == 1){
		pre_node->left = min_right;
	}
	else{
		pre_node->right = min_right;
	}
	pre_min->left = min_right->right;
	min_right->right = node->right;
	min_right->left = node->left;
	goto end_of_delete;

end_of_delete:
	if(node->prev != NULL){
		node->prev->next = node->next;
	}
	if(node->next != NULL){
		node->next->prev = node->prev;
	}
	clear_node(node);
	return ERR_OK;
}

void traversal(const Tree * const tree){
	if(tree == NULL || tree->root == NULL){ return; }
	Node *node = tree->root;
	while(node->right != NULL){
		node = node->right;
	}
	while(node != NULL){
		printf("%s %u\n", node->key, *node->info);
		node = node->next;
	}
}

void show_node(const Node * const node, int level){
	int i = level;
	if(node != NULL){
		show_node(node->right, level + strlen(node->key));
		while(i-- > 0){
			printf("  ");
		}
		printf("%s\n", node->key);
		show_node(node->left, level + strlen(node->key));
	}
}

void show(const Tree * const tree){
	show_node(tree->root, 0);
}

void clear_tree_node(Node *node){
	if(node != NULL){
		clear_tree_node(node->right);
		clear_tree_node(node->left);
		clear_node(node);
	}
}

void clear_tree(Tree *tree){
	clear_tree_node(tree->root);
}

err import_tree(Tree *tree, FILE * const file){
	clear_tree(tree);
	if(tree == NULL){ return ERR_MEM; }
	err flag = ERR_OK;
	unsigned info = 0;
	while(flag == ERR_OK){
		char *key = txt_readline(file);
		if(key == NULL){ break; }
		flag = txt_input_uint(file, &info, 0, UINT_MAX);
		if(flag != ERR_OK){
			free(key);
			break;
		}
		flag = insert_elem(tree, key, info);
		free(key);
	}
	if(flag == ERR_EOF){ flag = ERR_VAL; }
	return flag;
}
