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

#ifdef ITER
typedef struct Iterator Iterator;

Iterator *create_iterator(Table * const, unsigned);
void delete_iterator(Iterator *);
char cmp_iterator(Iterator *, Iterator *);
char next(Iterator *);
char prev(Iterator *);
KeySpace *peek(Iterator *);

Iterator *iter_insert(Table * const table, const unsigned key, const char * const elem);
Iterator *iter_delete(Iterator *iter);
Iterator *iter_find(Table * const table, const unsigned key, const unsigned release);
Iterator *iter_begin(Table * const table);
void show_by_iter(Iterator *iter);
#endif

err insert_elem(Table * const table, const unsigned key, const char * const elem);
err delete_elem(Table * const table, const unsigned key, const unsigned release);
Table *find_by_release(const Table * const table, const unsigned key, const unsigned release);
Table *create_table(const unsigned msize);
Table *find_by_key(const Table * const table, const unsigned key);
char find_last_release(const Table * const table, const unsigned key, unsigned * const release);
err reorganize_table(Table *table);
void clear_table(Table * const table);
err load_from_txt(Table *table, FILE * const file);
void show_table(const Table * const table);

#endif
