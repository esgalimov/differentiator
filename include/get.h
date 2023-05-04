#ifndef GET
#define GET

#include "diff.h"
#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "read_preorder.h"
#include "simplify.h"
#include "expr.h"

//! @brief Get funcs
tree_node_t * getG(expr_t* expr);
tree_node_t * getN(expr_t* expr);
tree_node_t * getE(expr_t* expr);
tree_node_t * getT(expr_t* expr);
tree_node_t * getP(expr_t* expr);
tree_node_t * getD(expr_t* expr);
//! @brief Get Func or variable (varibles are only one-letter)
tree_node_t * getW(expr_t* expr);

//! @brief Func to read name of function or variable
//! @param [in] expr - ptr to expression struct
//! @return ptr to name (must be freed after using)
char * read_name(expr_t* expr);

#endif
