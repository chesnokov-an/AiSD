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

void clear_node(Node *node){
	if(node == NULL){ return; }
	if(node->info != NULL){ free(node->info); }
	if(node->children != NULL){ free(node->children); }
	if(node->key != NULL){ free(node->key); }
	free(node);
}

char cmp(keytype key1, keytype key2){
	return strcmp(key1, key2);
}

Node *find(const Tree * const tree, const keytype const key){
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
	}
	else{
		pre_node->right = create_node(key, info);
		if(pre_node->right == NULL){ return ERR_MEM; }
	}
	firmware(tree);
	return ERR_OK;
}

char is_possible_to_delete(const Node * const node){
	if(node->left == NULL || node->right){
		return 1;
	}
	return 0;
}

err delete_elem(Tree * const tree, const keytype const key){
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
		if(left_flag == -1){ tree->root = NULL; }
		goto end_of_insert;
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
		goto end_of_insert;
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
		goto end_of_insert;
		return ERR_OK;
	}
	// Удаляем, если два потомка
	Node *min_right = node->right;
	Node *pre_min = NULL;
	while(node->left != NULL){
		pre_min = node;
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
		goto end_of_insert;
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
	goto end_of_insert;

end_of_insert:
	clear_node(node);
	free(node);
	firmware(tree);
	return ERR_OK;
}

void firmware(Tree * const tree) {
	Node *prev = NULL;
	firmware_by_node(tree->root, prev);
}

void firmware_by_node(Node * const node, Node * const prev){
	if(node == NULL){ return; }

	firmware_by_node(node->left, prev);
	firmware_by_node(node->right, prev);

	node->next = prev;
	prev = node;
}
