#ifndef DIALOG_TABLE_H
#define DIALOG_TABLE_H

#include "err.h"
#include "table.h"

void Dialog();
err D_insert(Table *);
err D_delete(Table *);
err D_find(Table *);
err D_show(Table *);
// err D_input(Table *);
// err D_output(Table *table);

#endif
