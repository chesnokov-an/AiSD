#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "err.h"
#include "table.h"
#include "input.h"
#include "my_readline.h"

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "\nDEBUG: %s:%d:%s(): " fmt "\n", \
								__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

#define SEED 31
#define GREEN "\033[38;2;0;255;0m"
#define RESET "\033[0;0m"

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
		table->ks[i].busy = FREE;
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
	ks->busy = BUSY;
	ks->key = strdup(key);
	ks->info = strdup(info);
}

unsigned hash(const char * const str, unsigned msize){
	unsigned res = 0;
	unsigned pow = 1;
	for(size_t i = 0; i < strlen(str); i++){
		res = (res + str[i] * pow) % msize;
		pow = (pow * SEED) % msize;
	}
	return res;
}

unsigned step_hash(const char * const str, unsigned msize){
	unsigned res = 0;
	for(size_t i = 0; i < strlen(str); i++){
		res = (res * SEED + str[i]) % (msize - 1);
	}
	return (res + 1);

}

char cmp(const char * const key1, const char * const key2){
	return ((strcmp(key1, key2) == 0) ? 0 : 1);
}

err insert_elem(Table *table, const char * const key, const char * const info){
	Table *find_table = find(table, key);
	if(find_table == NULL){
		return ERR_MEM;
	}
	if(find_table->csize == 1){
		return ERR_VAL;
	}
	clear_table(find_table);
	free(find_table);
	unsigned index = hash(key, table->msize);
	unsigned step = step_hash(key, table->msize);

	DEBUG_PRINT(GREEN"\nhash_index: %u\nhash_step: %u\n\n"RESET, index, step);

	unsigned seen = 0;
	while((table->ks[index].busy != FREE) && (seen < table->msize)){
		if(cmp(table->ks[index].key, key) == 0){
			free(table->ks[index].info);
			table->ks[index].info = strdup(info);
			table->ks[index].busy = BUSY;
			table->csize += 1;
			return ERR_OK;
		}
		index = (index + step) % table->msize;
		seen += 1;
	}
	err flag = ERR_OK;
	if(seen >= table->msize){
		if(table->csize < table->msize){
			flag = rehash(table);
		}
		else{
			flag = resize(table);
		}
		if(flag != ERR_OK){ return flag; }
		flag = insert_elem(table, key, info);
		return flag;

	}
	setter_keyspase(&(table->ks[index]), key, info);
	table->csize += 1;
	return ERR_OK;
}

err delete_elem(Table * const table, const char * const key){
	Table *find_table = find(table, key);
	if(find_table == NULL){
		return ERR_MEM;
	}
	if(find_table->csize == 0){
		return ERR_VAL;
	}
	clear_table(find_table);
	free(find_table);
	unsigned index = hash(key, table->msize);
	unsigned step = step_hash(key, table->msize);
	unsigned seen = 0;
	while((table->ks[index].busy != FREE) && (seen < table->msize)){
		if(cmp(table->ks[index].key, key) == 0){
			table->csize -= 1;
			table->ks[index].busy = DELETED;
			return ERR_OK;
		}
		index = (index + step) % table->msize;
		seen += 1;
	}
	return ERR_OK;
}

Table *find(const Table * const table, const char * const key){
	unsigned index = hash(key, table->msize);
	unsigned step = step_hash(key, table->msize);
	unsigned seen = 0;
	Table *res = create_table(1);
	if(res == NULL){ return NULL; }
	res->msize = 1;
	res->csize = 0;
	while((table->ks[index].busy != FREE) && (seen < table->msize)){
		if(cmp(table->ks[index].key, key) == 0){
			if(table->ks[index].busy == DELETED){ return res; }
			res->csize = 1;
			setter_keyspase(&(res->ks[0]), key, table->ks[index].info);
			return res;
		}
		index = (index + step) % table->msize;
		seen += 1;
	}
	return res;
}

void show_table(const Table * const table){
	printf("Размер таблицы: %u\nЗаполнено ячеек: %u\n\n", table->msize, table->csize);
	printf("Состояние | -------------Ключ------------ | ----Значение----\n");
	for(unsigned i = 0; i < table->msize; i++){
		printf("%5d     |%30s | \"%s\"\n", (int)(table->ks[i].busy), table->ks[i].key, table->ks[i].info);
	}
}

err input_bin(Table *table, FILE * const file){
	unsigned msize = 0;
	err flag = bin_input_uint(file, &msize, 0, UINT_MAX);
	if(flag != ERR_OK || msize == 0){ goto clean_and_return; }
	Table *new_table = create_table(msize);
	unsigned csize = 0;
	flag = bin_input_uint(file, &csize, 0, UINT_MAX);
	if(flag != ERR_OK){ goto clean_and_return; }
	for(unsigned i = 0; i < msize; i++){
		unsigned key_len = 0;
		unsigned info_len = 0;
		unsigned offset = 0;
		flag = bin_input_uint(file, &key_len, 0, UINT_MAX);
		if(flag != ERR_OK){ goto clean_and_return; }
		flag = bin_input_uint(file, &info_len, 0, UINT_MAX);
		if(flag != ERR_OK){ goto clean_and_return; }
		flag = bin_input_uint(file, &offset, 0, UINT_MAX);
		if(flag != ERR_OK){ goto clean_and_return; }
		if(offset == 0){ continue; }
		unsigned tmp_seek = ftell(file);
		fseek(file, offset, SEEK_SET);

		char *key = bin_read_n_symbols(file, key_len);
		if(key == NULL){
			flag = ERR_MEM;
			goto clean_and_return;
		}
		char *info = bin_read_n_symbols(file, info_len);
		if(info == NULL){
			free(key);
			flag = ERR_MEM;
			goto clean_and_return;
		}

		fseek(file, tmp_seek, SEEK_SET);
		flag = insert_elem(new_table, key, info);
		free(key);
		free(info);
		if(flag != ERR_OK){ goto clean_and_return; }
	}
	clear_table(table);
	table->ks = new_table->ks;
	table->msize = new_table->msize;
	table->csize = new_table->csize;
	free(new_table);
	return ERR_OK;

clean_and_return:
	clear_table(new_table);
	free(new_table);
	return flag;
}


err output_bin(const Table * const table, FILE * const file){
	if(table == NULL){ return ERR_NULL; }
	if(table->msize == 0 || table->csize == 0){ return ERR_EMPTY; }
	fwrite(&(table->msize), 1, sizeof(unsigned), file);
	fwrite(&(table->csize), 1, sizeof(unsigned), file);
	unsigned total_len = 0;
	unsigned *lens = (unsigned *)calloc(2 * table->csize, sizeof(unsigned));
	if(lens == NULL){ return ERR_MEM; }
	for(unsigned i = 0; i < table->msize; i++){
		if(table->ks[i].busy != BUSY){
			unsigned tmp = 0;
			fwrite(&tmp, 1, sizeof(unsigned), file);
			fwrite(&tmp, 1, sizeof(unsigned), file);
			fwrite(&tmp, 1, sizeof(unsigned), file);
			continue;
		}
		unsigned key_len = strlen(table->ks[i].key);
		unsigned info_len = strlen(table->ks[i].info);
		unsigned offset = sizeof(unsigned) * (2 + 3 * table->msize) + sizeof(char) * total_len;
		fwrite(&key_len, 1, sizeof(unsigned), file);
		fwrite(&info_len, 1, sizeof(unsigned), file);
		fwrite(&offset, 1, sizeof(unsigned), file);

		unsigned tmp_seek = ftell(file);
		fseek(file, offset, SEEK_SET);
		fwrite(table->ks[i].key, key_len, sizeof(char), file);
		fwrite(table->ks[i].info, info_len, sizeof(char), file);
		fseek(file, tmp_seek, SEEK_SET);

		total_len += key_len + info_len;
	}
	free(lens);
	return ERR_OK;
}

err rehash(Table * const table){
	Table *new_table = create_table(table->msize);
	if(new_table == NULL){ return ERR_NULL; }
	err flag = ERR_OK;
	for(unsigned i = 0; i < table->msize; i++){
		if(table->ks[i].key == NULL){ continue; }
		if(table->ks[i].busy != BUSY){ continue; }
		flag = insert_elem(new_table, table->ks[i].key, table->ks[i].info);
		if(flag != ERR_OK){
			goto clean_and_return;
		}
	}
	clear_table(table);
	table->ks = new_table->ks;
	table->msize = new_table->msize;
	table->csize = new_table->csize;
	free(new_table);
	return ERR_OK;

clean_and_return:
	clear_table(new_table);
	free(new_table);
	return flag;
}

char is_prime(const unsigned n){
	for(unsigned i = 2; i < sqrt(n) + 1; i++){
		if((n % i) == 0){
			return 0;
		}
	}
	return 1;
}

unsigned bigger_prime(const unsigned n){
	unsigned i = n + 1;
	while(is_prime(i) == 0){
		i++;
	}
	return i;
}

err resize(Table *table){
	Table *new_table = create_table(bigger_prime(table->msize));
	if(new_table == NULL){ return ERR_NULL; }
	err flag = ERR_OK;
	for(unsigned i = 0; i < table->msize; i++){
		if(table->ks[i].key == NULL){ continue; }
		if(table->ks[i].busy != BUSY){ continue; }
		flag = insert_elem(new_table, table->ks[i].key, table->ks[i].info);
		if(flag != ERR_OK){
			goto clean_and_return;
		}
	}
	clear_table(table);
	table->ks = new_table->ks;
	table->msize = new_table->msize;
	table->csize = new_table->csize;
	free(new_table);
	return ERR_OK;

clean_and_return:
	clear_table(new_table);
	free(new_table);
	return flag;
}
