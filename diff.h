#ifndef DIFF
#define DIFF

#include "tree.h"
#include "tree_debug.h"

//! @brief Types of print tree
enum print_mode
{
    PRE  = 1,
    IN   = 2,
    POST = 3,
};

//! @brief Return bad value in eval
const int NULL_EVAL = (const int) 0xDEADBEAF;

//! @brief ...
int tree_make_expression(tree_t * tree, print_mode mode, const char * filename);

//! @brief Different type of print tree
//! @param [in] node - ptr to start node
//! @param [out] stream - ptr to file to write
void tree_print_preorder (tree_node_t * node, FILE * stream);
void tree_print_inorder  (tree_node_t * node, FILE * stream);
void tree_print_postorder(tree_node_t * node, FILE * stream);

//! @brief ...
int tree_read_expression(tree_t * tree, const char * filename);

//! @brief ...
tree_node_t * tree_read_preorder(char * buffer, int * pos);

//! @brief ...
int is_operation(char ch);

//! @brief Eval ...
int eval(const tree_node_t * node);

#endif
