#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "err.h"
#include "table.h"
#include "input.h"
#include "my_readline.h"

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

err insert_elem(Table *table, unsigned key, const char *elem){
	if(table == NULL){
		return ERR_NULL;
	}
	if(table->csize == table->msize){
		return ERR_FULL;
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
	char *info = strdup(elem);
	(table->ks)[table->csize].info = info;
	table->csize += 1;
	return ERR_OK;
}

err delete_elem(Table *table, unsigned key, unsigned release){
	if(table == NULL){
		return ERR_NULL;
	}
	if(table->csize == 0){
		return ERR_EMPTY;
	}
	for(unsigned i = 0; i < table->csize; i++){
		if((table->ks[i].key == key) && (table->ks[i].release == release)){
			free(table->ks[i].info);
			table->csize -= 1;
			table->ks[i].key = table->ks[table->csize].key;
			table->ks[i].release = table->ks[table->csize].release;
			table->ks[i].info = table->ks[table->csize].info;
			return ERR_OK;
		}
	}
	return ERR_VAL;
}

void clear_table(Table *table){
	if((table == NULL) || (table->ks == NULL)){
		return;
	}
	for(unsigned i = 0; i < table->csize; i++){
		if(table->ks[i].info){
			free(table->ks[i].info);
		}
	}
	free(table->ks);
	table->csize = 0;
	return;
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

Table *find_by_key(Table *table, unsigned key){
	if(table == NULL || table->ks == NULL){
		return NULL;
	}
	unsigned count = 0;
	for(unsigned i = 0; i < table->csize; i++){
		if(table->ks[i].key == key){
			count += 1;
		}
	}
	Table *new_table = (Table *)calloc(1, sizeof(Table));
	new_table->ks = (KeySpace *)calloc(count, sizeof(KeySpace));
	new_table->msize = count;
	new_table->csize = count;
	count = 0;
	for(unsigned i = 0; i < table->csize; i++){
		if(table->ks[i].key == key){
			new_table->ks[count].key = key;
			new_table->ks[count].release = table->ks[i].release;
			new_table->ks[count].info = strdup(table->ks[i].info);
			count++;
		}
	}
	return new_table;
}

Table *find_by_release(Table *table, unsigned key, unsigned release){
	if(table == NULL || table->ks == NULL){
		return NULL;
	}
	Table *new_table = NULL;
	for(unsigned i = 0; i < table->csize; i++){
		if((table->ks[i].key == key) && (table->ks[i].release == release)){
			new_table = (Table *)calloc(1, sizeof(Table));
			new_table->ks = (KeySpace *)calloc(1, sizeof(KeySpace));
			new_table->msize = 1;
			new_table->csize = 1;
			new_table->ks[0].key = key;
			new_table->ks[0].release = release;
			new_table->ks[0].info = strdup(table->ks[i].info);
		}
	}
	return new_table;
}

err load_from_txt(Table *table, FILE *file){
	clear_table(table);
	unsigned msize = 0;
	err flag = txt_input_uint(file, &msize, 0, UINT_MAX);
	if(flag != ERR_OK || msize == 0){ goto clean_and_return; }
	unsigned csize = 0;
	flag = txt_input_uint(file, &csize, 0, msize);
	if(flag != ERR_OK){ goto clean_and_return; }
	table->msize = msize;
	table->csize = csize;
	table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if(table->ks == NULL){
		flag = ERR_MEM;
		goto clean_and_return;
	}
	for(unsigned i = 0; i < csize; i++){
		unsigned key = 0;
		flag = txt_input_uint(file, &key, 0, UINT_MAX);
		if(flag != ERR_OK){ goto clean_and_return; }
		table->ks[i].key = key;
		unsigned release = 0;
		flag = txt_input_uint(file, &release, 0, UINT_MAX);
		if(flag != ERR_OK){ goto clean_and_return; }
		table->ks[i].release = release;
		table->ks[i].info = txt_readline(file);
		if(table->ks[i].info == NULL){
			flag = ERR_MEM;
			goto clean_and_return;
		}
	}
	return ERR_OK;

clean_and_return:
	clear_table(table);
	return flag;
}
