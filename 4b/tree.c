#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include "tree.h"
#include "my_readline.h"
#include "input.h"

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

#define RED "\033[38;2;255;0;0m"
#define GREEN "\033[38;2;0;255;0m"
#define RESET "\033[0;0m"

#define left children[0]
#define middle children[1]
#define right children[2]
#define parent children[4]

Tree *create_tree(){
	Tree *tree = (Tree *)calloc(1, sizeof(Tree));
	return tree;
}

Node *create_node(){
	Node *node = (Node *)calloc(1, sizeof(Node));
	return node;
}

void clear_node(Node *node){
	if(node == NULL){ return; }
	for(int i = 0; i < 3; i++){
		if(node->key[i] != NULL){
			free(node->key[i]);
		}
		if(node->info[i] != NULL){
			free(node->info[i]);
		}
	}
	free(node);
}

char cmp(const char * const key1, const char * const key2){
	return strcmp(key1, key2);
}

Node *find(const Tree * const tree, const char * const key){
	Node *node = tree->root;
	char cmp_val_1 = 0;
	char cmp_val_2 = 0;

	while(node != NULL){
		cmp_val_1 = cmp(node->key[0], key);
		if(cmp_val_1 == 0){ return node; }
		if(cmp_val_1 > 0){
			node = node->left;
			continue;
		}
		if(node->size == 1){
			node = node->middle;
			continue;
		}
		cmp_val_2 = cmp(node->key[1], key);
		if(cmp_val_2 == 0){ return node; }
		node = (cmp_val_2 > 0) ? (node->middle) : (node->right);
	}
	return node;
}

unsigned str_diff(const char * const key1, const char * const key2){
	unsigned diff = 0;
	size_t min_len = (strlen(key1) > strlen(key2)) ? strlen(key2) : strlen(key1);
	for(size_t i = 0; i < min_len; i++){
		diff += abs(key1[i] - key2[i]);
	}
	return diff;
}

Node *max_node(const Tree * const tree){
	if(tree == NULL || tree->root == NULL){ return NULL; }
	Node *node = tree->root;
	while(node->middle != NULL){
		node = (node->right == NULL) ? (node->middle) : (node->right);
	}
	return node;
}

Node *min_node(const Tree * const tree){
	if(tree == NULL || tree->root == NULL){ return NULL; }
	Node *node = tree->root;
	while(node->left != NULL){
		node = node->left;
	}
	return node;
}

Node *spec_find(const Tree * const tree, const char * const key){
	if(tree == NULL || tree->root == NULL){ return NULL; }
	Node *r_node = max_node(tree);
	Node *l_node = min_node(tree);
	if(r_node->size == 2){
		return (str_diff(r_node->key[1], key) > str_diff(l_node->key[0], key)) ? (r_node) : (l_node);
	}
	return (str_diff(r_node->key[0], key) > str_diff(l_node->key[0], key)) ? (r_node) : (l_node);
}

int add_value(Node * const node, const char *key, const char *info){
	int i = node->size - 1;
	while(i > -1 && cmp(node->key[i], key) > 0){
		node->key[i+1] = node->key[i];
		node->info[i+1] = node->info[i];
		node->children[i+2] = node->children[i+1];
		i--;
	}
	node->key[i+1] = strdup(key);
	node->info[i+1] = strdup(info);
	node->size += 1;
	return i+1;
}

Node *split(Node *node){
	if(node == NULL){ return node; }
	if(node->size < 3){ return node; }

	if(node->parent == NULL){
		Node *new_parent = create_node();
		node->parent = new_parent;
	}

	Node *new_right = create_node();
	new_right->parent = node->parent;
	add_value(new_right, node->key[2], node->info[2]);
	free(node->key[2]);
	free(node->info[2]);
	node->key[2] = NULL;
	node->info[2] = NULL;
	if(node->children[2] != NULL && node->children[3] != NULL){
		node->children[2]->parent = new_right;
		node->children[3]->parent = new_right;
	}
	new_right->children[0] = node->children[2];
	new_right->children[1] = node->children[3];
	node->children[2] = NULL;
	node->children[3] = NULL;
	node->key[2] = NULL;
	node->info[2] = NULL;

	int p_index = add_value(node->parent, node->key[1], node->info[1]);
	free(node->key[1]);
	free(node->info[1]);
	node->key[1] = NULL;
	node->info[1] = NULL;
	node->parent->children[p_index] = node;
	node->parent->children[p_index + 1] = new_right;


	new_right->size = 1;
	node->size = 1;
	return split(node->parent);
}

err insert_elem(Tree * const tree, const char * const key, const char * const info){
	if(tree == NULL){ return ERR_NULL; }
	if(tree->root == NULL){
		tree->root = create_node();
		if(tree->root == NULL){ return ERR_MEM; }
		add_value(tree->root, key, info);
		return ERR_OK;
	}
	Node *node = tree->root;
	Node *pre_node = NULL;
	char cmp_val_1 = 0;
	char cmp_val_2 = 0;
	while(node != NULL){
		cmp_val_1 = cmp(node->key[0], key);
		if(cmp_val_1 == 0){ return ERR_VAL; }
		if(cmp_val_1 > 0){
			pre_node = node;
			node = node->left;
			continue;
		}
		if(node->size == 1){
			pre_node = node;
			node = node->middle;
			continue;
		}
		cmp_val_2 = cmp(node->key[1], key);
		if(cmp_val_2 == 0){ return ERR_VAL; }
		pre_node = node;
		node = (cmp_val_2 > 0) ? (node->middle) : (node->right);
	}
	add_value(pre_node, key, info);
	if(pre_node->size == 3){
		pre_node = split(pre_node);
	}
	while(pre_node->parent != NULL){
		pre_node = pre_node->parent;
	}
	tree->root = pre_node;
	return ERR_OK;
}





void swap_str(char **s1, char **s2){
	char *tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

err delete_elem(Tree * const tree, const char * const key){
	if(tree == NULL){ return ERR_NULL; }
	if(tree->root == NULL){
		return ERR_EMPTY;
	}

	// Находим элемент с таким ключом
	Node *node = tree->root;
	char cmp_val_1 = 0;
	char cmp_val_2 = 0;
	while(node != NULL){
		cmp_val_1 = cmp(node->key[0], key);
		if(cmp_val_1 == 0){ break; }
		if(cmp_val_1 > 0){
			node = node->left;
			continue;
		}
		if(node->size == 1){
			node = node->middle;
			continue;
		}
		cmp_val_2 = cmp(node->key[1], key);
		if(cmp_val_2 == 0){ break; }
		node = (cmp_val_2 > 0) ? (node->middle) : (node->right);
	}
	if(node == NULL){
		return ERR_NO_ELEM;
	}

	// Удаляем, если это корень с 1 ключом и 0 потомков
	if(node == tree->root && node->left == NULL){
		clear_node(tree->root);
		tree->root = NULL;
		return ERR_OK;
	}


	int deleted = (cmp_val_1 == 0) ? (0) : (1);

	// Если вершина - не лист, то находим подмену
	if(node->left != NULL){
		Node *min_right = (node->right != NULL) ? (node->right) : (node->middle);
		while(min_right->left != NULL){
			min_right = min_right->left;
		}
		swap_str(&node->key[deleted], &min_right->key[0]);
		swap_str(&node->info[deleted], &min_right->info[0]);
		node = min_right;
	}

	// Удаляем, если это лист с 2 ключами
	if(node->left == NULL && node->size == 2){
		free(node->key[deleted]);
		free(node->info[deleted]);
		if(cmp_val_1 == 0){
			node->key[0] = node->key[1];
			node->info[0] = node->info[1];
		}
		node->key[1] = NULL;
		node->info[1] = NULL;
		node->size = 1;
		return ERR_OK;
	}

	// Удаляем, если это лист с одной вершиной, который имеет брата с 2 ключами

}






void show_node(const Node * const node, int level, int side){
	if(node == NULL){ return; }
	int i = level;
	while(i-- > 0){
		printf("│    ");
	}
	if(side == 0){
		printf("├─(R)");
	}
	else if(side == 1){
		printf("├─(M)");
	}
	else if(side == 2){
		printf("└─(L)");
	}

	printf(GREEN"[\"%s\", \"%s\", \"%s\"]\n"RESET, node->key[0], node->key[1], node->key[2]);

	show_node(node->right, level + 1, 0);
	show_node(node->middle, level + 1, 1);
	show_node(node->left, level + 1, 2);
}

void show(const Tree * const tree){
	if(tree == NULL){ return; }
	if(tree->root == NULL){
		printf("\nДерево пусто.\n");
	}
	show_node(tree->root, 0, -1);
}

void clear_tree_node(Node *node){
	if(node != NULL){
		clear_tree_node(node->right);
		clear_tree_node(node->middle);
		clear_tree_node(node->left);
		clear_node(node);
	}
}

void clear_tree(Tree *tree){
	clear_tree_node(tree->root);
	tree->root = NULL;
}
















/*
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
		if(cmp_val == 0){ return ERR_VAL; }
		pre_node = node;
		node = (cmp_val > 0) ? (node->left) : (node->right);
	}

	int new_index = (cmp_val > 0) ? 0 : 1;
	int new_thread1 = (cmp_val > 0) ? 3 : 2;
	int new_thread2 = (cmp_val > 0) ? 2 : 3;
	pre_node->children[new_index] = create_node(key, info);
	if(pre_node->children[new_index] == NULL){ return ERR_MEM; }
	pre_node->children[new_index]->children[new_thread1] = pre_node->children[new_thread1];
	pre_node->children[new_index]->children[new_thread2] = pre_node;
	if(pre_node->children[new_thread1] != NULL){
		pre_node->children[new_thread1]->children[new_thread2] = pre_node->children[new_index];
	}
	pre_node->children[new_thread1] = pre_node->children[new_index];
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
	int side_flag = -1;
	char cmp_val = 0;
	while(node != NULL){
		cmp_val = cmp(node->key, key);
		if(cmp_val == 0){ break; }
		pre_node = node;
		node = (cmp_val > 0) ? node->left : node->right;
		side_flag = (cmp_val > 0) ? 0 : 1;
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
		pre_node->children[side_flag] = NULL;
		goto end_of_delete;
	}
	// Удаляем, если один потомок
	if(node->left == NULL || node->right == NULL){
		int next_side = (node->left == NULL) ? 1 : 0;
		if(side_flag == -1){
			tree->root = node->left;
		}
		else{
			pre_node->children[side_flag] = node->children[next_side];
		}
		goto end_of_delete;
	}
	// Удаляем, если два потомка
	Node *min_right = node->right;
	Node *pre_min = NULL;
	while(min_right->left != NULL){
		pre_min = min_right;
		min_right = min_right->left;
	}
	if(side_flag == -1){
		tree->root = min_right;
	}
	else{
		pre_node->children[side_flag] = min_right;
	}
	min_right->left = node->left;
	if(pre_min == NULL){
		goto end_of_delete;
	}
	pre_min->left = min_right->right;
	min_right->right = node->right;
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
	Node *node = max_node(tree);
	printf("\n╔══════════════════════════╦═════════════════╗\n");
	printf("║           Ключ           ║     Значение    ║\n");
	printf("║══════════════════════════║═════════════════║\n");
	while(node != NULL){
		printf("║  %22s  ║  %14u ║\n", node->key, *node->info);
		node = node->next;
	}
	printf("╚══════════════════════════╩═════════════════╝\n\n");

}

void write_to_txt(const Tree * const tree, FILE * const file){
	if(tree == NULL || tree->root == NULL){ return; }
	Node *node = max_node(tree);
	fprintf(file, "Ключ : Значение\n");
	while(node != NULL){
		fprintf(file, "%s : %u\n", node->key, *node->info);
		node = node->next;
	}
}



Agedge_t *make_edge(char *key1, char *key2, Agraph_t *graph){
	Agnode_t *node1 = agnode(graph, key1, TRUE);
	Agnode_t *node2 = agnode(graph, key2, TRUE);
	Agedge_t __attribute__((unused)) *edge = agedge(graph, node1, node2,"edge1",TRUE);
	return edge;
}

void draw_node(const Node * const node, Agraph_t *graph, unsigned *i){
	if(node != NULL){
		if(node->left != NULL){
			make_edge(node->key, node->left->key, graph);
		}
		else{
			char index [14] = "";
			sprintf(index, "none%u", *i);
			Agnode_t *node2 = agnode(graph, index, TRUE);
			agsafeset(node2, "style", "invis", "");
			Agedge_t *edge = make_edge(node->key, index, graph);
			agsafeset(edge, "style", "invis", "");
			*i += 1;
		}
		if(node->right != NULL){
			make_edge(node->key, node->right->key, graph);
		}
		else{
			char index [14] = "";
			sprintf(index, "none%u", *i);
			Agnode_t *node2 = agnode(graph, index, TRUE);
			agsafeset(node2, "style", "invis", "");
			Agedge_t *edge = make_edge(node->key, index, graph);
			agsafeset(edge, "style", "invis", "");
			*i += 1;
		}
		draw_node(node->left, graph, i);
		draw_node(node->right, graph, i);
	}
}


void draw(const Tree * const tree, FILE * const file){
	if(tree == NULL){ return; }
	Agraph_t *graph;
	graph = agopen("Tree", Agundirected, NULL);
	if(tree->root != NULL){
		unsigned i = 0;
		draw_node(tree->root, graph, &i);

		Node *node = max_node(tree);
		Node *pre_node = node;
		node = node->next;
		Agnode_t *node1 = NULL;
		Agnode_t *node2 = NULL;
		Agedge_t *edge1 = NULL;
		Agedge_t *edge2 = NULL;
		while(node != NULL){
			node1 = agnode(graph, pre_node->key, FALSE);
			node2 = agnode(graph, node->key, FALSE);
			edge1 = agedge(graph, node1, node2,"edge2",TRUE);
			agsafeset(edge1, "constraint", "false", "");
			agsafeset(edge1, "color", "green", "");
			agsafeset(edge1, "dir", "forward", "");
			agsafeset(edge1, "arrowhead", "normal", "");
			if(pre_node->prev == NULL){
				agsafeset(node1, "color", "green", "");
			}
			edge2 = agedge(graph, node2, node1,"edge3",TRUE);
			agsafeset(edge2, "color", "red", "");
			agsafeset(edge2, "constraint", "false", "");
			agsafeset(edge2, "dir", "forward", "");
			agsafeset(edge2, "arrowhead", "normal", "");
			pre_node = node;
			node = node->next;
			if(node == NULL){
				agsafeset(node2, "color", "red", "");
			}
		}
	}
	GVC_t *gvc = gvContext();
	gvLayout(gvc, graph, "dot");
	gvRender(gvc, graph, "dot", file);
	gvFreeLayout(gvc, graph);
	agclose(graph);
	gvFreeContext(gvc);
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
	tree->root = NULL;
}

err import_tree(Tree *tree, FILE * const file){
	if(tree == NULL){ return ERR_NULL; }
	char *magic_word = txt_readline(file);
	if(magic_word == NULL){ return ERR_VAL; }
	if(strcmp("DWRF", magic_word) != 0){ return ERR_VAL; }
	free(magic_word);
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
	if(flag == ERR_EOF){ flag = ERR_OK; }
	return flag;
}*/
