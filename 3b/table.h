#ifndef TABLE_H
#define TABLE_H

#include "err.h"

typedef struct KeySpace{
	char busy;
	char *key;
	char *info;
} KeySpace;

typedef struct Table{
	KeySpace *ks;
	unsigned msize;
	unsigned csize;
} Table;

Table *create_table(const unsigned msize);
void clear_table(Table * const table);

err insert_elem(Table * const table, const char * const key, const char * const info);
err delete_elem(Table * const table, const char * const key);
Table *find(const Table * const table, const char * const key);
void show_table(const Table * const table);
err input_bin(Table *table, FILE * const file);
err output_bin(const Table * const table, FILE * const file);

err rehash(Table * const table);
err resize(Table *table);

#endif
