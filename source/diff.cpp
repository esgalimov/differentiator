#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/diff.h"
#include "../include/dsl.h"
#include "../include/latex_funcs.h"


int diff_expression(const char* filename)
{
    expr_t* expr = expr_ctor(filename);

    ASSERT(expr);
    open_latex_file();

    init_latex_file();
    print_expr_latex(expr->tree->root);

    tree_t dtree = {};
    tree_ctor(&dtree);

    link_root(&dtree, diff(expr->tree->root));
    print_expr_latex(dtree.root);
    tree_dump(&dtree);
    tree_simplify(&dtree, &dtree.root);
    tree_dump(&dtree);
    print_expr_latex(dtree.root);

    tree_dtor(&dtree);
    expr_dtor(expr);
    create_pdf();

    return 0;
}

int is_operation(char ch)
{
    if (ch == '*' || ch == '/' || ch == '+' || ch == '-')
        return 1;
    return 0;
}

double eval(const tree_node_t * node)
{
    if (node == NULL)
    {
        fprintf(log_file, "<pre>Try to eval NULL node</pre>");
        return NAN;
    }
    if (node->type == TYPE_VAR)
    {
        fprintf(log_file, "<pre>Try to eval TYPE_VAR node</pre>");
        return NAN;
    }
    if (node->type == TYPE_NUM)
    {
        if (!isfinite(node->value))
            return NAN;
        return node->value;
    }

    switch (node->type)
    {
        case TYPE_ADD: return evalL + evalR;
        case TYPE_SUB: return evalL - evalR;
        case TYPE_MUL: return evalL * evalR;
        case TYPE_DIV:
        {
            double eval_right = evalR;

            if (is_equal(eval_right, 0))
            {
                fprintf(log_file, "<pre>ERROR: Division by zero, %p</pre>", node);
                subtree_dump(node);
                return NAN;
            }
            return eval(node->left) / eval_right;
        }
        case TYPE_SIN: return sin(evalR);
        case TYPE_COS: return cos(evalR);
        case TYPE_LN:  return log(evalR);
        case TYPE_POW:
        {
            double eval_left = evalL;
            if (eval_left <= 0)
            {
                fprintf(log_file, "<pre>ERROR: base of power is below or equal zero, %p</pre>", node);
                subtree_dump(node);
                return NAN;
            }
            return pow(eval_left, evalR);
        }
        case TYPE_LOG:
        {
            double eval_left = evalL;

            if (is_equal(eval_left, 1))
            {
                fprintf(log_file, "<pre>Log base is 1</pre>");
                subtree_dump(node);
                return NAN;
            }
            return log(evalL) / log(evalR);
        }
        case TYPE_EXP: return exp(evalR);
        default:
            fprintf(log_file, "<pre>Undefind operation</pre>");
            subtree_dump(node);
            return NAN;
    }
}

tree_node_t * diff(tree_node_t * node)
{
    if (node == NULL) return NULL;

    switch (node->type)
    {
        case TYPE_NUM:
        {
            if (!isfinite(node->value))
                return NUM(NAN);
            return NUM(0);
        }
        case TYPE_VAR: return NUM(1);
        case TYPE_ADD: return ADD(dL, dR);
        case TYPE_SUB: return SUB(dL, dR);
        case TYPE_MUL: return ADD(MUL(dL, cR), MUL(cL, dR));
        case TYPE_DIV: return DIV(SUB(MUL(dL, cR), MUL(cL, dR)), MUL(cR, cR));
        case TYPE_SIN: return MUL(COS(cR), dR);
        case TYPE_COS: return MUL(MUL(NUM(-1), SIN(cR)), dR);
        case TYPE_LN:  return DIV(dR, cR);
        case TYPE_POW:
        {
            if (node->left->type == TYPE_NUM && node->right->type == TYPE_NUM)
                return NUM(0);
            else if (node->left->type == TYPE_NUM && node->right->type != TYPE_NUM)
                return MUL(MUL(POW(cL, cR), LN(cL)), dR);
            else if (node->left->type != TYPE_NUM && node->right->type == TYPE_NUM)
                return MUL(MUL(cR, POW(cL, NUM(node->right->value - 1))), dL);
            else
            {
                tree_node_t * node_pow   = POW(cL, cR),
                            * node_sum_1 = DIV(MUL(dL, cR), cL),
                            * node_sum_2 = MUL(dR, LN(cL));
                return MUL(node_pow, ADD(node_sum_1, node_sum_2));
            }
        }
        case TYPE_LOG: return DIV(dR, MUL(LN(cL), cR));
        case TYPE_EXP: return MUL(EXP(cR), dR);
        default: return NULL;
    }
}

int is_equal(double num1, double num2)
{
    ASSERT(isfinite(num1));
    ASSERT(isfinite(num2));

    return fabs(num1 - num2) < EPS;
}
