#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "err.h"
#include "table.h"
#include "input.h"
#include "my_readline.h"

#define SEED 37

Table *create_table(const unsigned msize){
	Table *table = (Table *)calloc(1, sizeof(Table));
	if(table == NULL){
		return NULL;
	}
	table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if(table->ks == NULL){
		free(table);
		return NULL;
	}
	table->csize = 0;
	table->msize = msize;
	return table;
}

void clear_table(Table * const table){
	if((table == NULL) || (table->ks == NULL)){
		return;
	}
	for(unsigned i = 0; i < table->msize; i++){
		tble->ks[i].busy = 0;
		if(table->ks[i].key){
			free(table->ks[i].key);
		}
		if(table->ks[i].info){
			free(table->ks[i].info);
		}
	}
	free(table->ks);
	table->csize = 0;
	return;
}

void setter_keyspase(KeySpace * const ks, const char * const key, const char * const info){
	ks->busy = 1;
	ks->key = strdup(key);
	ks->info = strdup(info);
}

unsigned hash(const char * const str, unsigned msize){
	unsigned res = 0;
	unsigned pow = 1;
	for(size_t i = 0; i < strlen(str); i++){
		res = (res + s[i] * pow) % msize;
		pow = (pow * SEED) % msize;
	}
	return res;
}

unsigned step_hash(const char * const str, unsigned msize){
	unsigned res = 0;
	for(size_t i = 0; i < strlen(str); i++){
		res = (res * SEED + s[i]) % msize;
	}
	return res;

}

err insert_elem(Table * const table, const char * const key, const char * const info){
	if(find(table, key) != NULL){
		return ERR_VAL;
	}
	unsigned index = hash(key, table->msize);
	unsigned step = step_hash(key, table->msize);
	unsigned seen = 0;
	while((table->ks[index].busy == 1) && (seen < table->csize)){
		index = (index + step) % table->msize;
		seen += 1;
	}
	if(seen >= table->csize){
		return ERR_FULL;
	}
	setter_keyspase(&(table->ks[index]), key, info);
	table->csize += 1;
	return ERR_OK;
}

err delete_elem(Table * const table, const char * const key){
	if(find(table, key) == NULL){
		return ERR_VAL;
	}
	unsigned index = hash(key, table->msize);
	unsigned step = step_hash(key, table->msize);
	unsigned seen = 0;
	while((table->ks[index].busy == 1) && (seen < table->csize)){
		index = (index + step) % table->msize;
		seen += 1;
	}
	table->ks[index].busy = 0;
	table->csize -= 1;
	return ERR_OK;
}

Table *find(const Table * const table, const char * const key){
	unsigned index = hash(key, table->msize);
	unsigned step = step_hash(key, table->msize);
	unsigned seen = 0;
	while((table->ks[index].busy == 1) && (seen < table->csize)){
		if(strcmp() == 0){
			Table *res = create_table(1);
			if(res == NULL){ return NULL; }
			res->msize = 1;
			res->csize = 1;
			setter_keyspase(&(res->ks[0]), key, table->ks[index].info);
		}
		index = (index + step) % table->msize;
		seen += 1;
	}
	return NULL;
}

void show_table(const Table * const table){
	printf("Размер таблицы: %u\nЗаполнено ячеек: %u\n\n", table->msize, table->csize);
	printf("  ------------Ключ------------ | ----Значение----\n");
	unsigned i = 0;
	while(i < table->csize){
		if(table->ks[i].busy == 1){
			printf("%30s | \"%s\"\n", table->ks[i].key, table->ks[i].info);
			i++;
		}
	}
}
