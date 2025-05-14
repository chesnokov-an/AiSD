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
#define BLUE "\033[38;2;0;191;255m"
#define RESET "\033[0;0m"

#define left children[0]
#define middle children[1]
#define right children[2]


Tree *create_tree(){ return (Tree *)calloc(1, sizeof(Tree)); }
Node *create_node(){ return (Node *)calloc(1, sizeof(Node)); }

void clear_node(Node *node){
	if(node == NULL){ return; }
	for(int i = 0; i < node->size; i++){
		if(node->key[i] != NULL){
			free(node->key[i]);
		}
		if(node->info[i] != NULL){
			free(node->info[i]);
		}
	}
	free(node);
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

Node *max_node(Node * const node){
	if(node == NULL){ return NULL; }
	Node *res = node;
	while(res->middle != NULL){
		res = (res->right != NULL) ? (res->right) : (res->middle);
	}
	return res;
}

Node *min_node(Node * const node){
	if(node == NULL){ return NULL; }
	Node *res = node;
	while(res->left != NULL){
		res = res->left;
	}
	return res;
}

Node *spec_find(const Tree * const tree, const char * const key){
	if(tree == NULL || tree->root == NULL){ return NULL; }
	Node *r_node = max_node(tree->root);
	Node *l_node = min_node(tree->root);
	if(r_node->size == 2){
		return (str_diff(r_node->key[1], key) > str_diff(l_node->key[0], key)) ? (r_node) : (l_node);
	}
	return (str_diff(r_node->key[0], key) > str_diff(l_node->key[0], key)) ? (r_node) : (l_node);
}

int add_value(Node * const node, const char * const key, const char * const info){
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

void insert_key_info(Node * const node, char *key, char *info){
	int i = node->size - 1;
	while(i > -1 && cmp(node->key[i], key) > 0){
		node->key[i+1] = node->key[i];
		node->info[i+1] = node->info[i];
		i--;
	}
	node->key[i+1] = key;
	node->info[i+1] = info;
	node->size += 1;
}

void remove_key_info(Node * const node, int pos){
	if(pos == 0){
		node->key[0] = node->key[1];
		node->info[0] = node->info[1];
	}
	node->key[1] = NULL;
	node->info[1] = NULL;
	node->size -= 1;
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
		new_right->children[0] = node->children[2];
		new_right->children[1] = node->children[3];
		node->children[2] = NULL;
		node->children[3] = NULL;
	}

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

int index_in_parent(const Node * const node){
	if(node == NULL || node->parent == NULL){ return -1; }
	if(node == node->parent->left){ return 0; }
	else if(node == node->parent->middle){ return 1; }
	return 2;
}

Node *redistribute(Node *node){
	Node *par = node->parent;
	Node *le = par->left;
	Node *mi = par->middle;
	Node *ri = par->right;

	// в родителе 2 ключа, нет братьев с 2 ключами
	if((par->size == 2) && (le->size != 2) && (mi->size != 2) && (ri->size != 2)){
		if(node == par->left){
			par->left = par->middle;
			par->middle = par->right;
			par->right = NULL;
			insert_key_info(par->left, par->key[0], par->info[0]);
			remove_key_info(par, 0);
			par->left->right = par->left->middle;
			par->left->middle = par->left->left;

			if(node->left != NULL){
				par->left->left = node->left;
			}
			else if(node->middle != NULL){
				par->left->left = node->middle;
			}
			if(par->left->left != NULL){
				par->left->left->parent = par->left;
			}
			free(le);
		}
		else if(node == par->middle){
			insert_key_info(le, par->key[0], par->info[0]);
			remove_key_info(par, 0);
			if(node->left != NULL){
				le->right = node->left;
			}
			else if(node->middle != NULL){
				le->right = node->middle;
			}
			if(le->right != NULL){
				le->right->parent = le;
			}
			par->middle = par->right;
			par->right = NULL;
			free(mi);
		}
		else{
			insert_key_info(mi, par->key[1], par->info[1]);
			remove_key_info(par, 1);
			par->right = NULL;
			if(node->left != NULL){
				mi->right = node->left;
			}
			else if(node->middle != NULL){
				mi->right = node->middle;
			}
			if(mi->right != NULL){
				mi->right->parent = mi;
			}
			free(ri);
		}
	}

	// в родителе 2 ключа, есть братья с 2 ключами
	else if((par->size == 2) && ((le->size == 2) || (mi->size == 2) || (ri->size == 2))){
		if(node == par->left){
			if(node->left == NULL){
				node->left = node->middle;
				node->middle = NULL;
			}
			insert_key_info(le, par->key[0], par->info[0]);
			remove_key_info(par, 0);
			insert_key_info(par, mi->key[0], mi->info[0]);
			remove_key_info(mi, 0);
			if(mi->size == 1){	// 1, т.к. 1 уже забрали в parent
				le->middle = mi->left;
				if (le->middle != NULL){
					le->middle->parent = le;
				}
				mi->left = mi->middle;
				mi->middle = mi->right;
				mi->right = NULL;
			}
			else if(ri->size == 2){
				insert_key_info(mi, par->key[1], par->info[1]);
				remove_key_info(par, 1);
				insert_key_info(par, ri->key[0], ri->info[0]);
				remove_key_info(ri, 0);
				le->middle = mi->left;
				if (le->middle != NULL){
					le->middle->parent = le;
				}
				mi->left = mi->middle;
				mi->middle = ri->left;
				if (mi->middle != NULL){
					mi->middle->parent = mi;
				}
				ri->left = ri->middle;
				ri->middle = ri->right;
				ri->right = NULL;
			}
		}
		if(node == par->middle){
			if(ri->size == 2){
				if(node->left == NULL){
					node->left = node->middle;
					node->middle = NULL;
				}
				insert_key_info(mi, par->key[1], par->info[1]);
				remove_key_info(par, 1);
				insert_key_info(par, ri->key[0], ri->info[0]);
				remove_key_info(ri, 0);
				mi->middle = ri->left;
				if(mi->middle != NULL){
					mi->middle->parent = mi;
				}
				ri->left = ri->middle;
				ri->middle = ri->right;
				ri->right = NULL;
		    }
			else if(le->size == 2){
	            if(node->middle == NULL){
	                node->middle = node->left;
	                node->left = NULL;
				}
				insert_key_info(mi, par->key[0], par->info[0]);
				remove_key_info(par, 0);
				insert_key_info(par, le->key[1], le->info[1]);
				remove_key_info(le, 1);
				mi->left = le->right;
				if(mi->left != NULL){
					mi->left->parent = mi;
				}
				le->right = NULL;
			}
		}
		if(node == par->right){
			if(node->left != NULL){
				node->middle = node->left;
				node->left = NULL;
			}
			insert_key_info(node, par->key[1], par->info[1]);
			remove_key_info(par, 1);
            if(mi->size == 2){
				insert_key_info(par, mi->key[1], mi->info[1]);
				remove_key_info(mi, 1);
				node->left = mi->right;
				mi->right = NULL;
				if(node->left != NULL){
					node->left->parent = node;
				}
			}
			else if(le->size == 2){
				insert_key_info(par, mi->key[0], mi->info[0]);
				remove_key_info(mi, 0);
				node->left = mi->middle;
				mi->middle = mi->left;
				if(node->left != NULL){
					node->left->parent = node;
				}
				insert_key_info(mi, par->key[0], par->info[0]);
				remove_key_info(par, 0);
				insert_key_info(par, le->key[1], le->info[1]);
				remove_key_info(le, 1);
				mi->left = le->right;
				if(mi->left != NULL){
					mi->left->parent = mi;
				}
				le->right = NULL;
			}
		}
	}
	// В родителе 1 ключ и есть брат с 2 ключами
	else if(par->size == 1){
		insert_key_info(node, par->key[0], par->info[0]);
		remove_key_info(par, 0);

		if((node == par->left) && (mi->size == 2)){

			insert_key_info(par, mi->key[0], mi->info[0]);
			remove_key_info(mi, 0);

			if(node->left == NULL){
				node->left = node->middle;
			}
			node->middle = mi->left;
			mi->left = mi->middle;
			mi->middle = mi->right;
			mi->right = NULL;
			if(node->middle != NULL){
				node->middle->parent = node;
			}
		}
		else if((node == par->middle) && (le->size == 2)){

			insert_key_info(par, le->key[1], le->info[1]);
			remove_key_info(le, 1);

			if(node->middle == NULL){
				node->middle = node->left;
			}
			node->left = le->right;
			le->right = NULL;
			if(node->left != NULL){
				node->left->parent = node;
			}
		}
	}
	return par;
}

Node *merge(Node *node){
	Node *par = node->parent;
	if(par->left == node){
		insert_key_info(par->middle, par->key[0], par->info[0]);
		remove_key_info(par, 0);
		par->middle->right = par->middle->middle;
	    par->middle->middle = par->middle->left;
		if(node->left != NULL){
			par->middle->left = node->left;
		}
	    else if(node->middle != NULL){
			par->middle->left = node->middle;
		}
	    if(par->middle->left != NULL){
			par->middle->left->parent = par->middle;
		}
		free(par->left);
		par->left = NULL;
	}
	else if(par->middle == node){
		insert_key_info(par->left, par->key[0], par->info[0]);
		remove_key_info(par, 0);
        if(node->left != NULL){
			par->left->right = node->left;
		}
		else if(node->middle != NULL){
			par->left->right = node->middle;
		}
		if(par->left->right != NULL){
			par->left->right->parent = par->left;
		}
		free(par->middle);
		par->middle = NULL;
	}
	if(par->parent == NULL){
		Node *tmp = NULL;
		if(par->left != NULL){
			tmp = par->left;
		}
		else{
			tmp = par->middle;
		}
		tmp->parent = NULL;
		free(par);
		return tmp;
    }
	return par;
}

char is_redistributable(const Node * const node){
	if(node->parent->size == 2){ return 1; }
	if((node->parent->size == 1) && ((node->parent->left->size == 2) || (node->parent->middle->size == 2))){ return 1; }
	return 0;
}

Node *fix_after_delete(Node *node){
	if(node->size == 0 && node->parent == NULL){
		free(node);
		return NULL;
	}
	if(node->size != 0){
		if(node->parent != NULL){ return fix_after_delete(node->parent); }
		return node;
	}
	if(node->parent->left->size == 2 || node->parent->middle->size == 2 || node->parent->size == 2){
		node = redistribute(node);
	}
	else if(node->parent->size == 2 && node->parent->right->size == 2){ node = redistribute(node); }
	else{
		node = merge(node);
	}
	return fix_after_delete(node);
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
	int deleted = (cmp_val_1 == 0) ? (0) : (1);

	// Удаляем, если это корень с 1 ключом и 0 потомков
	if(node == tree->root && node->left == NULL && node->size == 1){
		clear_node(tree->root);
		tree->root = NULL;
		return ERR_OK;
	}

	// Если вершина - не лист, то находим подмену
	if(node->left != NULL){
		Node *min_right = (cmp_val_1 == 0) ? (min_node(node->middle)) : (min_node(node->right));
		swap_str(&node->key[deleted], &min_right->key[0]);
		swap_str(&node->info[deleted], &min_right->info[0]);
		node = min_right;
	}

	// Удаляем, если в листе 2 ключа
	if(node->size == 2){
		free(node->key[deleted]);
		free(node->info[deleted]);
		if(cmp_val_1 == 0){ remove_key_info(node, 0); }
		else{ remove_key_info(node, 1); }
		return ERR_OK;
	}

	// Удаляем, если требуется перераспределение или слияние узлов
	free(node->key[0]);
	free(node->info[0]);
	node->size = 0;
	node = fix_after_delete(node);
	while(node->parent != NULL){
		node = node->parent;
	}
	tree->root = node;
	return ERR_OK;
}

void show_node(const Node * const node, int level, int side){
	if(node == NULL){ return; }
	int i = level;
	while(i-- > 0){ printf("│    "); }
	if(side == 0){ printf("├─(R)"); }
	else if(side == 1){ printf("├─(M)"); }
	else if(side == 2){ printf("└─(L)"); }

	printf(GREEN"[\"%s\", \"%s\"]"RESET" : "BLUE"[\"%s\", \"%s\"]"RESET" : "RED"%d\n"RESET, node->key[0], node->key[1], node->info[0], node->info[1], node->size);

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

err import_tree(Tree *tree, FILE * const file){
	if(tree == NULL){ return ERR_NULL; }
	char *magic_word = txt_readline(file);
	if(magic_word == NULL){ return ERR_VAL; }
	if(strcmp("DWRF", magic_word) != 0){ return ERR_VAL; }
	free(magic_word);
	err flag = ERR_OK;
	while(flag == ERR_OK){
		char *key = txt_readline(file);
		if(key == NULL){ break; }
		char *info = txt_readline(file);
		if(info == NULL){
			free(key);
			break;
		}
		flag = insert_elem(tree, key, info);
		free(key);
		free(info);
	}
	if(flag == ERR_EOF){ flag = ERR_OK; }
	return flag;
}

void traversal_node(const Node * const node, const char * const key){
	if(node == NULL){ return; }
	for(int i = 0; i < node->size; i++){
		if(key == NULL || (key != NULL && strcmp(key, node->key[i]) < 0)){
			traversal_node(node->children[i], key);
			printf("\"%s\" : \"%s\"\n", node->key[i], node->info[i]);
		}
	}
	traversal_node(node->children[node->size], key);
}

err traversal(const Tree * const tree, const char * const key){
	if(tree == NULL){ return ERR_NULL; }
	if(tree->root == NULL){ return ERR_EMPTY; }
	if(key == NULL || strlen(key) == 0){
		traversal_node(tree->root, NULL);
	}
	else{
		traversal_node(tree->root, key);
	}
	return ERR_OK;
}


Agedge_t *make_edge(const Node * const n1, const Node * const n2, Agraph_t *graph){
	char *sub1_name = (char *)calloc(strlen(n1->key[0]) + 9, sizeof(char));
	strcpy(sub1_name, "cluster_");
	strcat(sub1_name, n1->key[0]);
	Agraph_t *sub_g1 = agsubg(graph, sub1_name, TRUE);

	char *sub2_name = (char *)calloc(strlen(n2->key[0]) + 9, sizeof(char));
	strcpy(sub2_name, "cluster_");
	strcat(sub2_name, n2->key[0]);
	Agraph_t *sub_g2 = agsubg(graph, sub2_name, TRUE);

	char *n1_name = (char *)calloc(strlen(n1->key[0]) + 7, sizeof(char));
	strcpy(n1_name, "NODE1_");
	strcat(n1_name, n1->key[0]);
	Agnode_t *node1 = agnode(graph, n1_name, TRUE);
	agsubnode(sub_g1, node1, TRUE);
	free(n1_name);

	char *n2_name = (char *)calloc(strlen(n2->key[0]) + 7, sizeof(char));
	strcpy(n2_name, "NODE2_");
	strcat(n2_name, n2->key[0]);
	Agnode_t *node2 = agnode(graph, n2_name, TRUE);
	agsubnode(sub_g2, node2, TRUE);
	free(n2_name);

	agsafeset(node1, "style", "invis", "");
	agsafeset(node1, "shape", "point", "");
	agsafeset(node1, "width", "0", "");
	agsafeset(node2, "style", "invis", "");
	agsafeset(node2, "shape", "point", "");
	agsafeset(node2, "width", "0", "");

	Agedge_t *edge = agedge(graph, node1, node2, "edge1", TRUE);

	agsafeset(edge, "lhead", sub2_name, "");
	agsafeset(edge, "ltail", sub1_name, "");

	free(sub1_name);
	free(sub2_name);

	return edge;
}

void draw_node(const Node * const node, Agraph_t *graph){
	if(node != NULL){
		char *sub_name = (char *)calloc(strlen(node->key[0]) + 9, sizeof(char));
		strcpy(sub_name, "cluster_");
		strcat(sub_name, node->key[0]);
		Agraph_t *sub_g = agsubg(graph, sub_name, TRUE);
		free(sub_name);

		Agnode_t __attribute__((unused)) *node1 = agnode(sub_g, node->key[0], TRUE);
		if(node->size == 2){
			Agnode_t __attribute__((unused)) *node2 = agnode(sub_g, node->key[1], TRUE);
		}

		if(node->left != NULL){
			make_edge(node, node->left, graph);
		}
		if(node->middle != NULL){
			make_edge(node, node->middle, graph);
		}
		if(node->right != NULL){
			make_edge(node, node->right, graph);
		}
		draw_node(node->left, graph);
		draw_node(node->middle, graph);
		draw_node(node->right, graph);
	}
}


void draw(const Tree * const tree, FILE * const file){
	if(tree == NULL){ return; }
	Agraph_t *graph = agopen("Tree", Agdirected, NULL);
	agsafeset(graph, "compound", "true", "");
	if(tree->root != NULL){
		draw_node(tree->root, graph);
	}
	GVC_t *gvc = gvContext();
	gvLayout(gvc, graph, "dot");
	gvRender(gvc, graph, "dot", file);
	gvFreeLayout(gvc, graph);
	agclose(graph);
	gvFreeContext(gvc);
}
