#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include "tree.h"
#include "my_readline.h"
#include "err.h"

void prepare_line(char * const line){
	for(size_t i = 0; i < strlen(line); i++){
		if(ispunct(line[i])){
			line[i] = ' ';
		}
		line[i] = tolower(line[i]);
	}
}

void add_word(Tree * const tree, char *word, unsigned offset, unsigned line_num, char *file_name){
	if(find(tree, word) != NULL){ return; }
	char *buffer = calloc(strlen(word) + 25 + strlen(file_name), sizeof(char));

	char offset_str[11] = "";
	sprintf(offset_str, "%u", offset);
	char line_num_str[11] = "";
	sprintf(line_num_str, "%u", line_num);

	strcat(buffer, file_name);
	strcat(buffer, ":");
	strcat(buffer, line_num_str);
	strcat(buffer, ":");
	strcat(buffer, offset_str);
	insert_elem(tree, word, buffer);
	free(buffer);
}

Tree *parse_file(FILE *file, char *file_name){
	Tree *tree = create_tree();

	char *line = txt_readline(file);
	unsigned line_num = 1;
	unsigned offset = 0;
	while(line != NULL){
		offset = 0;
		prepare_line(line);
		char *word = strtok(line, "\t ");
		while(word != NULL){
			add_word(tree, word, offset, line_num, file_name);
			offset += strlen(word) + 1;
			word = strtok(NULL, "\t ");
		}
		free(line);
		line = txt_readline(file);
		line_num++;
	}
	fclose(file);
	return tree;
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("Введите название файла.\n");
		return 0;
	}
	FILE *file = fopen(argv[1], "r");
	if(!file){
		printf("Неизвестный файл.\n");
		return 0;
	}
	Tree *tree = parse_file(file, argv[1]);

	printf("!--- Все слова вводите в нижнем регистре ---!\n\n");

	char *word = readline("Введите слово: ");
	while(word != NULL){
		Node *node = find(tree, word);
		if(node != NULL){
			if(strcmp(word, node->key[0]) == 0){
				printf("\n%s\n\n", node->info[0]);
			}
			else{
				printf("\n%s\n\n", node->info[1]);
			}
		}
		else{
			printf("Нет такого слова.\n\n");
		}
		free(word);
		word = readline("Введите слово: ");
	}

	clear_tree(tree);
	free(tree);
	return 0;
}
