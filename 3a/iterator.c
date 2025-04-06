#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

typedef struct Iterator{
	Table *table;
	unsigned ptr;
} Iterator;


Iterator *create_iterator(Table * const table, unsigned ptr){
	Iterator *iter = (Iterator *)calloc(1, sizeof(Iterator));
	if(iter == NULL){ return NULL; }
	iter->table = table;
	iter->ptr = ptr;
	return iter;
}

void delete_iterator(Iterator *iter){
	if(iter == NULL){ return; }
	free(iter);
}

char cmp_iterator(Iterator *iter1, Iterator *iter2){
	if((iter1 == NULL) && (iter2 == NULL)){ return 1; }
	if((iter1 == NULL) || (iter2 == NULL)){ return 0; }
	char *s1 = iter1->table->ks[iter1->ptr].info;
	char *s2 = iter2->table->ks[iter2->ptr].info;
	if(&s1 == &s2){
		return 1;
	}
	return 0;
}

char next(Iterator *iter){
	if(iter == NULL){ return 0; }
	if(iter->table->csize > iter->ptr){
		iter->ptr += 1;
		return 1;
	}
	return 0;
}

char prev(Iterator *iter){
	if(iter == NULL){ return 0; }
	if(0 < iter->ptr){
		iter->ptr -= 1;
		return 1;
	}
	return 0;
}

KeySpace *peek(Iterator *iter){
	if(iter == NULL){ return NULL; }
	return (iter->table->ks + iter->ptr);
}
