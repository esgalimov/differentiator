#ifndef LATEX_FUNCS
#define LATEX_FUNCS


#include "tree.h"
#include "tree_debug.h"
#include "diff.h"

extern FILE* latex_file;

int open_latex_file(void);

int close_latex_file(void);

int init_latex_file(void);

void print_expr_latex(tree_node_t* node);

void print_subtree_latex(tree_node_t* node);

void create_pdf(void);

#endif
