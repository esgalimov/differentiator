#ifndef DIFF
#define DIFF

#include "tree.h"
#include "tree_debug.h"

//! @brief Types of print tree
enum mode
{
    PRE  = 1,
    IN   = 2,
    POST = 3,
};

//! @brief Return bad value in eval
const int ERROR = (const int) 0xDEADBEAF;

//! @brief
const double EPS = 0.000001;

typedef struct
{
    char * buffer;
    int    pos;
} expr_text;

//! @brief ...
int tree_make_expression(tree_t * tree, mode print_mode, const char * filename);

//! @brief Different type of print tree
//! @param [in] node - ptr to start node
//! @param [out] stream - ptr to file to write
void tree_print_preorder (tree_node_t * node, FILE * stream);
void tree_print_inorder  (tree_node_t * node, FILE * stream);
//void tree_print_postorder(tree_node_t * node, FILE * stream);

//! @brief ...
int tree_read_expression(tree_t * tree, mode read_mode, const char * filename);

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
//! @brief ...
int is_operation(char ch);

//! @brief Eval ...
double eval(const tree_node_t * node);

//! @brief Func to compare two double numbers, use EPS constant in it
int is_equal(double num1, double num2);

//! @brief Diff ...
tree_node_t * diff(tree_node_t * node);

//-------------------------Get part------------------------------

//! @brief Get funcs
tree_node_t * getG(expr_text * expr);
tree_node_t * getN(expr_text * expr);
tree_node_t * getE(expr_text * expr);
tree_node_t * getT(expr_text * expr);
tree_node_t * getP(expr_text * expr);
tree_node_t * getD(expr_text * expr);

#endif
