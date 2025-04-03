#ifndef ITERATOR_H
#define ITERATOR_H

#include "err.h"
#include "table.h"

typedef struct Iterator Iterator;

Iterator *create_iterator(Table *, unsigned);
void delete_iterator(Iterator *);
char cmp_iterator(Iterator *, Iterator *);
char next(Iterator *);
char prev(Iterator *);
KeySpace *peek(Iterator *);

#endif
