#ifndef EXPR
#define EXPR

#include "diff.h"
#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "get.h"
#include "read_preorder.h"
#include "print.h"
#include "simplify.h"

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
    var_t**  vars;
    tree_t*  tree;
} expr_t;

//! @brief...
expr_t* expr_ctor(const char* filename);

//! @brief...
char* read_buffer(FILE* stream);

//! brief...
int expr_dtor(expr_t* expr);

#endif
