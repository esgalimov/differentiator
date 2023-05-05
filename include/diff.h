#ifndef DIFF
#define DIFF

#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "get.h"
#include "read_preorder.h"
#include "print.h"
#include "simplify.h"
#include "expr.h"

//! @brief POISON to ctor expr
const int POISON = (const int) 0xDEADBEAF;

//! @brief Epsilon to compare double variables
const double EPS = 0.000001;

//! @brief ...
int diff_expression(const char * filename);

//! @brief ...
int is_operation(char ch);

//! @brief Eval ...
double eval(const tree_node_t * node);

//! @brief Eval tree with vars values
double eval_var(const tree_node_t* node, expr_t* expr);

//! @brief Func to compare two double numbers, use EPS constant in it
int is_equal(double num1, double num2);

//! @brief Diff ...
tree_node_t * diff(tree_node_t * node, int id);

//! @brief ...
int get_var_values(expr_t* expr);

#endif
