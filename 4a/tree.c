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

#define GREEN "\033[38;2;0;255;0m"
#define RESET "\033[0;0m"

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
	node->info = (unsigned *)calloc(1, sizeof(unsigned));
	if(node->info == NULL){
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
		node = (cmp_val > 0) ? (node->left) : (node->right);
	}
	return node;
}

unsigned str_diff(const char * const key1, const char * const key2){
	unsigned diff = 0;
	size_t min_len = (strlen(key1) > strlen(key2)) ? strlen(key2) : strlen(key1);
	for(size_t i = 0; i < min_len; i++){
		diff += abs(key1[i] - key2[i]);
		DEBUG_PRINT("%d", key1[i]);
	}
	return diff;
}

Node *max_node(const Tree * const tree){
	if(tree == NULL || tree->root == NULL){ return NULL; }
	Node *node = tree->root;
	while(node->right != NULL){
		node = node->right;
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
	return (str_diff(r_node->key, key) > str_diff(l_node->key, key)) ? (r_node) : (l_node);
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


void show_node(const Node * const node, const Node * const pre_node, int offset, int level, char side_flag){
	int i = offset;
	int j = level;
	int max_len = 0;
	if(pre_node != NULL){
		if(pre_node->right != NULL && pre_node->left != NULL){
			max_len = (strlen(pre_node->right->key) > strlen(pre_node->left->key)) ? (strlen(pre_node->right->key)) : (strlen(pre_node->left->key));
		}
		else if(pre_node->right != NULL){ max_len = strlen(pre_node->right->key); }
		else if(pre_node->left != NULL){ max_len = strlen(pre_node->left->key); }
	}
	else if(node != NULL){
		max_len = strlen(node->key);
	}
	if(node != NULL){
		show_node(node->right, node, offset + strlen(node->key) + 2, max_len - strlen(node->key), 0);
		while(i-- > 0){
			printf(" ");
		}
		if(side_flag == 0){
			printf("┌──");
		}
		else if(side_flag == 1){
			printf("└──");
		}
		while(j-- > 0){
			printf("─");
		}
		printf("%s\n", node->key);
		show_node(node->left, node, offset + strlen(node->key) + 2, max_len - strlen(node->key), 1);
	}
}

void show(const Tree * const tree){
	show_node(tree->root, NULL, -2, 0, -1);
}
/*
void draw_node(const Node * const node, FILE * const file, unsigned *i){
	if(node != NULL){
		if(node->left != NULL){
			fprintf(file, "\"%s\" -> \"%s\" [dir=none];\n", node->key, node->left->key);
		}
		else{
			fprintf(file, "none%u [style=invis]\n", *i);
			fprintf(file, "\"%s\" -> none%u [style=invis];\n", node->key, *i);
			*i += 1;
		}
		if(node->right != NULL){
			fprintf(file, "\"%s\" -> \"%s\" [dir=none];\n", node->key, node->right->key);
		}
		else{
			fprintf(file, "none%u [style=invis]\n", *i);
			fprintf(file, "\"%s\" -> none%u [style=invis];\n", node->key, *i);
			*i += 1;
		}
		draw_node(node->left, file, i);
		draw_node(node->right, file, i);
	}
}

void draw(const Tree * const tree, FILE * const file){
	fprintf(file, "digraph BST {\n");
	if(tree->root != NULL){
		unsigned i = 0;
		draw_node(tree->root, file, &i);
	}
	fprintf(file, "}");
}
*/

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
}
