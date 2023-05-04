#ifndef READ_PREORDER
#define READ_PREORDER

#include "diff.h"
#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "get.h"
#include "print.h"
#include "simplify.h"
#include "expr.h"

//! @brief ...
tree_node_t * tree_read_preorder(char * buffer, int * pos);

//! @brief ...
int check_close_bracket(char * buffer, int * pos);

//! @brief ...
tree_node_t * get_number(char * buffer, int * pos);

//! @brief ...
tree_node_t * get_oper(char * buffer, int * pos, char ch);

//! @brief ...
tree_node_t * get_func_or_var(char * buffer, int * pos);

#endif
