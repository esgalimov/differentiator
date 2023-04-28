#define ADD(left, right) create_op(TYPE_ADD, left, right)
#define SUB(left, right) create_op(TYPE_SUB, left, right)
#define MUL(left, right) create_op(TYPE_MUL, left, right)
#define DIV(left, right) create_op(TYPE_DIV, left, right)

#define SIN(R)    create_func(TYPE_SIN, R)
#define COS(R)    create_func(TYPE_COS, R)
#define POW(L, R) create_func(TYPE_POW, L, R)
#define LN(R)     create_func(TYPE_LN,  R)
#define LOG(L, R) create_func(TYPE_LOG, L, R)
#define EXP(R)    create_func(TYPE_EXP, R)

#define NUM(x) create_num(x)
#define VAR(x) create_var(x)

#define dL diff(node->left)
#define dR diff(node->right)

#define cL copy_subtree(node->left)
#define cR copy_subtree(node->right)

#define evalR eval(node->right)
#define evalL eval(node->left)

#define CMP_LEFT(x)  ((*node)->left->type  == TYPE_NUM && is_equal((*node)->left->value,  x))

#define CMP_RIGHT(x) ((*node)->right->type == TYPE_NUM && is_equal((*node)->right->value, x))
