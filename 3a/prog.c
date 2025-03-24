#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "err.h"
#include "table.h"

int main(){
	Table *table = create_table(5);
	char *elem = readline("ff");
	err flag = insert_elem(table, 2, elem);
	char *elem1 = readline("ff");
	flag = insert_elem(table, 6, elem1);
	delete_elem(table, 2, 0);

	show_table(table);
	free(elem);
	free(elem1);
	clear_table(table);
	free(table);

	return 0;
}
