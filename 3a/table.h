#ifndef TABLE_H
#define TABLE_H

#include "err.h"

typedef struct KeySpace{
	unsigned key;
	unsigned release;
	char *info;
} KeySpace;

typedef struct Table{
	KeySpace *ks;
	unsigned msize;
	unsigned csize;
} Table;

Table *create_table(const unsigned msize);
err insert_elem(Table * const table, const unsigned key, const char * const elem);
err delete_elem(Table * const table, const unsigned key, const unsigned release);
Table *find_by_key(const Table * const table, const unsigned key);
Table *find_by_release(const Table * const table, const unsigned key, const unsigned release);
char find_last_release(const Table * const table, const unsigned key, unsigned * const release);
void show_table(const Table * const table);
err reorganize_table(Table *table);
void clear_table(Table * const table);
err load_from_txt(Table *table, FILE *file);

#endif
