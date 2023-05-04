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
const int POISON = (const int) 0xDEADBEAF;

//! @brief Epsilon to compare double variables
const double EPS = 0.000001;

//! @brief Max lenght of func or variable name
const int NAME_MAX_LEN = 32;
const int VARS_MAX_CNT = 32;

//! @brief var_t
typedef struct
{
    char * name;
    int    type;
    elem_t value;
} var_t;

//! @brief Struct to read expression
typedef struct
{
    char*    buffer;
    int      pos;
    int      var_cnt;
    var_t ** vars;
    tree_t*  tree;
} expr_t;                   //changed to expr_t

//! @brief ...
int tree_print_expression(tree_t * tree, mode print_mode, const char * filename);

//! @brief Different type of print tree
//! @param [in] node - ptr to start node
//! @param [out] stream - ptr to file to write
void tree_print_preorder (tree_node_t * node, FILE * stream);
void tree_print_inorder  (tree_node_t * node, FILE * stream);
//void tree_print_postorder(tree_node_t * node, FILE * stream);

//! @brief ...
int tree_read_expression(const char * filename);

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

//-------------------------Get-part------------------------------

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

//-----------------------Simplify-part----------------------------

//! @brief Func what count subtrees without variables
//! @param [in] node - ptr to start node
//! @return ptr to simplified subtree
void tree_eval_simplify(tree_node_t** node);

//! @brief Func what remove *1, /1, ˆ1, *0, ˆ0, 0ˆ, 1ˆ and count subtrees without variables
//! @param [in] node - ptr to start node
//! @param [in] tree - ptr to tree, it used to change 'simplify'
//! variable to check if tree stop changing and we must stop simplifying
//! @return ptr to simplified subtree
void tree_simplify(tree_t* tree, tree_node_t ** node);

//! @brief Func what check if there are variables in subtree
//! @param [in] node - ptr to node
//! @return 1 - there is(are) var(s), 0 - else
int have_var(tree_node_t* node);

//! @brief ...
void right_instead_node(tree_t * tree, tree_node_t ** node);

//! @brief ...
void left_instead_node(tree_t * tree, tree_node_t ** node);

//! @brief ...
void num_instead_node(tree_t * tree, tree_node_t ** node, elem_t num);

//! @brief ...
void neg_instead_node(tree_t * tree, tree_node_t ** node);

expr_t* expr_ctor(const char* filename);
char* read_buffer(FILE* stream);
int expr_dtor(expr_t* expr);

#endif
