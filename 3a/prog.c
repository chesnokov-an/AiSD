#include <stdio.h>
#include <stdlib.h>
#include "dialog_table.h"

int main(int argc, char **argv){
	unsigned capacity = 0;
	if(argc > 1){
		capacity = strtoul(argv[1], NULL, 0);
	}
	Dialog(capacity);

	return 0;
}
