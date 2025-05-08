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

void add_word(Tree * const tree, char *word){
	Node *node = find(tree, word);
	if(node == NULL){
		insert_elem(tree, word, 0);
		node = find(tree, word);
	}
	*node->info += 1;
}

int main(){
	Tree *tree = create_tree();
	FILE *file = input_correct_file();
	while(!file){
		file = input_correct_file();
	}

	char *line = txt_readline(file);
	while(line != NULL){
		prepare_line(line);
		char *word = strtok(line, "\t ");
		while(word != NULL){
			add_word(tree, word);
			word = strtok(NULL, "\t ");
		}
		free(line);
		line = txt_readline(file);
	}

	fclose(file);
	file = fopen("res.txt", "w");
	write_to_txt(tree, file);
	fclose(file);
	goto end_program;

end_program:
	clear_tree(tree);
	free(tree);
	return 0;
}
