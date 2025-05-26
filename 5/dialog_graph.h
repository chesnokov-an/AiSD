#ifndef DIALOG_GRAPH_H
#define DIALOG_GRAPH_H

#include "err.h"
#include "graph.h"

void Dialog();
err D_insert_node(Graph *);
err D_insert_edge(Graph *);
err D_remove_node(Graph *);
err D_remove_edge(Graph *);
err D_modify_node(Graph *);
err D_modify_edge(Graph *);
err D_show(Graph *);
err D_draw(Graph *);
err D_import(Graph *);
err D_generate(Graph *);

#endif
