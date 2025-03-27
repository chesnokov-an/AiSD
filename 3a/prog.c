#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "err.h"
#include "table.h"

int main(){
	Table *table = create_table(5);
	//char *elem = readline("ff");
	FILE *file = fopen("test.txt", "r");
	err flag = load_from_txt(table, file);
	//char *elem1 = readline("ff");
	//flag = insert_elem(table, 6, elem1);
	//delete_elem(table, 2, 0);

	Table *nn = find_by_key(table, 1);
	show_table(nn);
	show_table(table);
	//free(elem);
	//free(elem1);
	//
	flag = reorganize_table(table);
	show_table(table);
	//
	clear_table(table);
	free(table);
	clear_table(nn);
	free(nn);
	fclose(file);


	return 0;
}
