#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include "tree.h"
#include "my_readline.h"
#include "input.h"

/*
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "\nDEBUG: %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif
*/

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

Node **spec_find(const Tree * const tree, const unsigned info, unsigned *count){
	if(tree == NULL || tree->root == NULL){ return NULL; }
	Node *node = tree->root;
	while(node->right != NULL){
		node = node->right;
	}
	unsigned diff = 0;
	unsigned cur_diff = 0;
	*count = 0;
	while(node != NULL){
		cur_diff = (*node->info > info) ? (*node->info - info) : (info - *node->info);
		if(diff == cur_diff){
			*count += 1;
		}
		else if(diff < cur_diff){
			*count = 1;
			diff = cur_diff;
		}
		node = node->next;
	}
	Node **arr_nodes = (Node **)calloc(*count, sizeof(Node *));
	if(arr_nodes == NULL){ return NULL; }
	node = tree->root;
	while(node->right != NULL){
		node = node->right;
	}
	*count = 0;
	while(node != NULL){
		cur_diff = (*node->info > info) ? (*node->info - info) : (info - *node->info);
		if(cur_diff == diff){
			arr_nodes[*count] = node;
			*count += 1;
		}
		node = node->next;
	}
	return arr_nodes;
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

void show_node(const Node * const node, const Node * const pre_node, int offset, int level, char left_flag){
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
		if(left_flag == 0){
			printf("┌──");
		}
		else if(left_flag == 1){
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

void draw_node(const Node * const node, Agraph_t *graph, unsigned *i){
	if(node != NULL){
		if(node->left != NULL){
			Agnode_t *node1 = agnode(graph, node->key, TRUE);
			Agnode_t *node2 = agnode(graph, node->left->key, TRUE);
			Agedge_t __attribute__((unused)) *edge = agedge(graph, node1, node2,"edge1",TRUE);
		}
		else{
			Agnode_t *node1 = agnode(graph, node->key, TRUE);
			char index [14] = "";
			sprintf(index, "none%u", *i);
			Agnode_t *node2 = agnode(graph, index, TRUE);
			agsafeset(node2, "style", "invis", "");
			Agedge_t *edge = agedge(graph, node1, node2,"edge1",TRUE);
			agsafeset(edge, "style", "invis", "");
			*i += 1;
		}
		if(node->right != NULL){
			Agnode_t *node1 = agnode(graph, node->key, TRUE);
			Agnode_t *node2 = agnode(graph, node->right->key, TRUE);
			Agedge_t __attribute__((unused)) *edge = agedge(graph, node1, node2,"edge1",TRUE);
		}
		else{
			Agnode_t *node1 = agnode(graph, node->key, TRUE);
			char index [14] = "";
			sprintf(index, "none%u", *i);
			Agnode_t *node2 = agnode(graph, index, TRUE);
			agsafeset(node2, "style", "invis", "");
			Agedge_t *edge = agedge(graph, node1, node2,"edge1",TRUE);
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

		Node *node = tree->root;
		while(node->right != NULL){
			node = node->right;
		}
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
	clear_tree(tree);
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
