#ifndef DIALOG_TREE_H
#define DIALOG_TREE_H

#include "err.h"
#include "tree.h"

void Dialog();
err D_insert(Tree *);
err D_delete(Tree *);
err D_find(Tree *);
err D_spec_find(Tree *);
err D_show(Tree *);
err D_draw(Tree *);
err D_traversal(Tree *);
err D_input(Tree *);

#endif
