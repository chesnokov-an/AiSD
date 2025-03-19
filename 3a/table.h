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

Table *create_table(unsigned msize);
err insert_elem(Table *table, unsigned key, char *elem);
err delete_elem(Table *table, unsigned key, unsigned release);
Table *find_by_key(Table *table, unsigned key);
Table *find_by_release(Table *table, unsigned key, unsigned release);
char find_last_release(Table *table, unsigned key, unsigned *release);
void show_table(Table *table);
err load_from_txt(Table *table, FILE *file);
err reorganize_table(Table *table);
void clear_table(Table *table);

#endif
