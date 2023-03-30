#ifndef MY_TREE
#define MY_TREE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define tree_ctor(tree) tree_ctor_((tree), var_info {#tree, LOCATION})

typedef int elem_t;

//! @brief Types of node
enum node_type
{
    TYPE_NUM = 0,
    TYPE_OP  = 1,
};

//! @brief Link modes
enum link_mode
{
    LEFT  = 0,
    RIGHT = 1,
};

enum operation
{
    OP_ADD = 1,
    OP_SUB = 2,
    OP_MUL = 3,
    OP_DIV = 4,
};

enum print_mode
{
    PRE  = 1,
    IN   = 2,
    POST = 3,
};

//! @struct var_info
//! @brief Information about tree to write to log
//! @var name - name of tree
//! @var func - function where tree created
//! @var file - file where tree created
//! @var line - line where tree created
typedef struct
{
    const char * name;
    const char * func;
    const char * file;
    int          line;
} var_info;

//! @brief Tree node struct
//! @var type  - type of node (operation of number)
//! @var valur - value of node
//! @var left  - ptr to left child
//! @var right - ptr to right child
typedef struct tree_node
{
    int         type;
    elem_t      value;
    tree_node * left;
    tree_node * right;
    tree_node * parent;
} tree_node_t;

//! @brief Tree struct
//! @var root   - root node ptr
//! @var status - tree status
//! @var info   - struct with info about tree
typedef struct
{
    tree_node_t * root;
    int           status;
    var_info      info;
} tree_t;

//! @brief Tree constructor
//! @param tree - ptr to tree
//! @return 0 - Ok, 1 - else
int tree_ctor_(tree_t * tree, var_info info);

//! @brief Tree destructor
//! @param tree - ptr to tree
//! @return 0 - Ok
int tree_dtor(tree_t * tree);

//! @brief Nodes destructor
//! @param [in] node - ptr to node
void nodes_dtor(tree_node_t * node);

//! @brief Link root node to tree
//! @param [in] tree - ptr to tree
//! @param [in] node - ptr to node
//! @return 0 - Ok, 1 - root is NULL
int link_root(tree_t * tree, tree_node_t * root);

//! @brief Create node
//! @param [in] type  - type of node
//! @param [in] value - value of node
//! @return ptr to node - if Ok, NULL - else
tree_node_t * create_node(node_type type, elem_t value);

//! @brief Link node
//! @param [in] parent - ptr to parent node
//! @param [in] child  - ptr to node to link
//! @param [in] mode   - left or right child
//! @return 0 - Ok, 1 - else
int link_node(tree_node_t * parent, tree_node_t * child, link_mode mode);

//! @brief ...
int tree_make_expression(tree_t * tree, print_mode mode);

//! @brief Different type of print tree
//! @param [in] node - ptr to start node
//! @param [out] stream - ptr to file to write
void tree_print_preorder (tree_node_t * node, FILE * stream);
void tree_print_inorder  (tree_node_t * node, FILE * stream);
void tree_print_postorder(tree_node_t * node, FILE * stream);


#endif
