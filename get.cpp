#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"

tree_node_t * getG(expr_text * expr)
{
    tree_node_t * val = getE(expr);

    if (expr->buffer[expr->pos] != '$' && val != NULL)
    {
        fprintf(log_file, "<pre>Func: getG; ERROR: must be \"$\" in the end; pos = %d, ch = %c\n</pre>",
                           expr->pos, expr->buffer[expr->pos]);
        return NULL;
    }
    return val;
}

tree_node_t * getE(expr_text * expr)
{
    tree_node_t * val = getT(expr);
    while (expr->buffer[expr->pos] == '+' || expr->buffer[expr->pos] == '-')
    {
        char op = expr->buffer[expr->pos];
        expr->pos++;
        tree_node_t * val2 = getT(expr);
        if (op == '+') val = ADD(val, val2);
        else           val = SUB(val, val2);
    }
    return val;
}

tree_node_t * getT(expr_text * expr)
{
    tree_node_t * val = getD(expr);
    while (expr->buffer[expr->pos] == '*'|| expr->buffer[expr->pos] == '/')
    {
        char op = expr->buffer[expr->pos];
        expr->pos++;
        tree_node_t * val2 = getD(expr);
        if (op == '*') val = MUL(val, val2);
        else
        {
            if (is_equal(val2->value, 0))
            {
                fprintf(log_file, "<pre>Func: getT; ERROR: division by zero; pos = %d\n</pre>", expr->pos);
                return NULL;
            }
            val = DIV(val, val2);
        }
    }
    return val;
}

tree_node_t * getD(expr_text * expr)
{
    tree_node_t * val = getP(expr);

    while (expr->buffer[expr->pos] == '^')
    {
        expr->pos++;
        tree_node_t * val2 = getP(expr);
        val = POW(val, val2);
    }
    return val;
}

tree_node_t * getP(expr_text * expr)
{
    if (expr->buffer[expr->pos] == '(')
    {
        expr->pos++;
        tree_node_t * val = getE(expr);
        if (expr->buffer[expr->pos] != ')')
        {
            fprintf(log_file, "<pre>Func: getP; ERROR: no \")\", pos = %d, ch = %c\n</pre>",
                               expr->pos, expr->buffer[expr->pos]);
            return NULL;
        }
        expr->pos++;
        return val;
    }
    else return getN(expr);
}

tree_node_t * getN(expr_text * expr)
{
    double val = 0;
    int saved_p = expr->pos;
    int is_neg = 0, d_after_dot = -1;

    if (expr->buffer[expr->pos] == '-')
    {
        is_neg = 1;
        expr->pos++;
    }

    while (('0' <= expr->buffer[expr->pos] && expr->buffer[expr->pos] <= '9') || expr->buffer[expr->pos] == '.')
    {
        if (d_after_dot >= 0)
            d_after_dot++;

        if (expr->buffer[expr->pos] == '.')
        {
            if (d_after_dot == -1)
                d_after_dot++;
            else
            {
                fprintf(log_file, "<pre>Func: getN; ERROR: second \".\" in number, pos = %d\n</pre>", expr->pos);
                return NULL;
            }
        }
        else
            val = val * 10 + expr->buffer[expr->pos] - '0';

        expr->pos++;
    }
    if (expr->pos == saved_p)
    {
        fprintf(log_file, "<pre>Func: getN; ERROR: digit didn't find; pos = %d, ch = %c\n</pre>",
                           expr->pos, expr->buffer[expr->pos]);
        return NULL;
    }
    double ret_val = (is_neg ? -1 : 1) * val / pow(10, (d_after_dot == -1) ? 0 : d_after_dot);
    return NUM(ret_val);
}
