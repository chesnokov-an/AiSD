#include <stdio.h>
#include <stdlib.h>
#include "err.h"
#include "table.h"

Table *create_table(unsigned msize){
	Table *table = (Table *)calloc(1, sizeof(Table));
	if(table == NULL){
		return NULL;
	}
	table->csize = 0;
	table->msize = msize;
	table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if(table->ks == NULL){
		return NULL;
	}
	return table;
}

char find_last_release(Table *table, unsigned key, unsigned *release){
	if(table == NULL){
		return 0;
	}
	char flag = 0;
	for(unsigned i = 0; i < table->csize; i++){
		if((table->ks[i].key == key) && (table->ks[i].release >= *release)){
			flag = 1;
			*release = table->ks[i].release;
		}
	}
	return flag;
}

err insert_elem(Table *table, unsigned key, char *elem){
	if(table == NULL){
		return ERR_NULL;
	}
	if(table->csize == table->msize){
		return ERR_MEM;
	}
	unsigned last_release = 0;
	char flag = find_last_release(table, key, &last_release);
	if(flag == 0){
		(table->ks)[table->csize].release = 0;
	}
	else{
		(table->ks)[table->csize].release = last_release + 1;
	}
	(table->ks)[table->csize].key = key;
	(table->ks)[table->csize].info = elem;
	table->csize += 1;
	return ERR_OK;
}

void show_table(Table *table){
	if(table == NULL){
		return;
	}
	printf("Размер таблицы: %u\nЗаполнено ячеек: %u\n\n", table->msize, table->csize);
	printf("   Ключ    |   Версия   | Значение \n");
	for(unsigned i = 0; i < table->csize; i++){
		printf("%10u | %10u | \"%s\"\n", table->ks[i].key, table->ks[i].release, table->ks[i].info);
	}
}
