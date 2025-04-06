#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "err.h"
#include "table.h"
#include "input.h"
#include "my_readline.h"

#ifdef ITER
typedef struct Iterator{
	Table *table;
	unsigned ptr;
} Iterator;
#endif

Table *create_table(const unsigned msize){
	Table *table = (Table *)calloc(1, sizeof(Table));
	if(table == NULL){
		return NULL;
	}
	table->csize = 0;
	table->msize = msize;
	table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if(table->ks == NULL){
		free(table);
		return NULL;
	}
	return table;
}

void setter_keyspase(KeySpace * const ks, const unsigned key, const unsigned release, const char * const info){
		ks->key = key;
		ks->release = release;
		ks->info = strdup(info);
}

char find_last_release(const Table * const table, const unsigned key, unsigned * const release){
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

#ifdef ITER
Iterator *iter_begin(Table * const table){
	return create_iterator(table, 0);
}
#endif

#ifdef ITER
Iterator *iter_insert(Table * const table, const unsigned key, const char * const elem){
	if(table == NULL){ return NULL; }
	if(table->csize == table->msize){ return NULL; }

	Iterator *iter = create_iterator(table, 0);
	if(iter == NULL){ return NULL; }
	KeySpace *i = peek(iter);
	unsigned new_release = 0;
	char flag = 1;
	while((iter->ptr < table->csize) && (flag == 1)){
		if(i->key == key){
			new_release = (i->release >= new_release) ? (i->release + 1) : (new_release);
		}
		flag = next(iter);
		i = peek(iter);
	}
	if(flag == 0){ return NULL; }
	setter_keyspase(&(iter->table->ks[iter->ptr]), key, new_release, elem);
	return iter;
}
#endif

err insert_elem(Table * const table, const unsigned key, const char * const elem){
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


#ifdef ITER
Iterator *iter_delete(Iterator *iter){
	if(iter == NULL){ return NULL; }
	free(iter->table->ks[iter->ptr].info);
	iter->table->csize -= 1;
	KeySpace last = iter->table->ks[iter->table->csize];
	setter_keyspase(&(iter->table->ks[iter->ptr]), last.key, last.release, last.info);
	free(last.info);
	return iter;
}
#endif

err delete_elem(Table * const table, const unsigned key, const unsigned release){
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
			KeySpace last = table->ks[table->csize];
			setter_keyspase(&(table->ks[i]), last.key, last.release, last.info);
			free(last.info);
			return ERR_OK;
		}
	}
	return ERR_VAL;
}


void clear_table(Table * const table){
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

Table *find_by_key(const Table * const table, const unsigned key){
	if(table == NULL || table->ks == NULL){
		return NULL;
	}
	unsigned count = 0;
	for(unsigned i = 0; i < table->csize; i++){
		if(table->ks[i].key == key){
			count += 1;
		}
	}

	Table *new_table = create_table(count);
	if(new_table == NULL || new_table->ks == NULL){
		return NULL;
	}
	new_table->csize = count;
	count = 0;
	for(unsigned i = 0; i < table->csize; i++){
		if(table->ks[i].key == key){
			setter_keyspase(&(new_table->ks[count]), key, table->ks[i].release, table->ks[i].info);
			count++;
		}
	}
	return new_table;
}

#ifdef ITER
Iterator *iter_find(Table * const table, const unsigned key, const unsigned release){
	if(table == NULL || table->ks == NULL){ return NULL; }
	Iterator *iter = iter_begin(table);
	char flag = 1;
	KeySpace *i = peek(iter);
	while((iter->ptr < table->csize) && (flag == 1)){
		if((i->key == key) && (i->release == release)){
			return iter;
		}
		flag = next(iter);
		i = peek(iter);
	}
	return NULL;
}
#endif

Table *find_by_release(const Table * const table, const unsigned key, const unsigned release){
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
			setter_keyspase(&(new_table->ks[0]), key, release, table->ks[i].info);
		}
	}
	return new_table;
}


err load_from_txt(Table *table, FILE *file){
	Table *new_table = (Table *)calloc(1, sizeof(Table));
	if(new_table == NULL){ return ERR_MEM; }
	unsigned msize = 0;
	err flag = txt_input_uint(file, &msize, 0, UINT_MAX);
	if(flag != ERR_OK || msize == 0){ goto clean_and_return; }
	unsigned csize = 0;
	flag = txt_input_uint(file, &csize, 0, msize);
	if(flag != ERR_OK){ goto clean_and_return; }
	new_table->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if(new_table->ks == NULL){
		flag = ERR_MEM;
		goto clean_and_return;
	}
	new_table->msize = msize;
	for(unsigned i = 0; i < csize; i++){
		unsigned key = 0;
		flag = txt_input_uint(file, &key, 0, UINT_MAX);
		if(flag != ERR_OK){ goto clean_and_return; }
		char *info = txt_readline(file);
		if(info == NULL){
			flag = ERR_MEM;
			goto clean_and_return;
		}
		flag = insert_elem(new_table, key, info);
		free(info);
		if(flag != ERR_OK){ goto clean_and_return; }
	}
	clear_table(table);
	*table = *new_table;
	free(new_table);
	return ERR_OK;

clean_and_return:
	clear_table(new_table);
	free(new_table);
	return flag;
}

err reorganize_table(Table *table){
	err flag = ERR_OK;
	Table *new_table = (Table *)calloc(1, sizeof(Table));
	if(new_table == NULL){ return ERR_MEM; }
	new_table->ks = (KeySpace *)calloc(table->msize, sizeof(KeySpace));
	if(new_table->ks == NULL){
		free(new_table);
		return ERR_MEM;
	}
	new_table->msize = table->msize;
	new_table->csize = 0;
	for(unsigned i = 0; i < table->csize; i++){
		unsigned last_release = 0;
		find_last_release(table, table->ks[i].key, &last_release);
		if(table->ks[i].release == last_release){
			flag = insert_elem(new_table, table->ks[i].key, table->ks[i].info);
			if(flag != ERR_OK){
				clear_table(new_table);
				free(new_table);
				return flag;
			}
		}
	}
	clear_table(table);
	*table = *new_table;
	free(new_table);
	return ERR_OK;
}


#ifdef ITER
void show_by_iter(Iterator *iter){
	printf("----Реализация на итераторе----\n");
	if(iter == NULL){ return; }
	char flag = 1;
	printf("Размер таблицы: %u\nЗаполнено ячеек: %u\n\n", iter->table->msize, iter->table->csize);
	printf("   Ключ    |   Версия   | Значение \n");
	KeySpace *i = NULL;
	while((iter->ptr < iter->table->csize) && (flag == 1)){
		i = peek(iter);
		printf("%10u | %10u | \"%s\"\n", i->key, i->release, i->info);
		flag = next(iter);
	}
}
#endif

void show_table(const Table * const table){
	if(table == NULL){
		return;
	}
	printf("Размер таблицы: %u\nЗаполнено ячеек: %u\n\n", table->msize, table->csize);
	printf("   Ключ    |   Версия   | Значение \n");
	for(unsigned i = 0; i < table->csize; i++){
		printf("%10u | %10u | \"%s\"\n", table->ks[i].key, table->ks[i].release, table->ks[i].info);
	}
}
