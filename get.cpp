#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"

const char * str = NULL;
int p = 0;

tree_node_t * getG(const char * exp)
{
    str = exp; p = 0;

    tree_node_t * val = getE();

    if (str[p] != '$' && val != NULL)
    {
        fprintf(log_file, "Func: getG; ERROR: must be \"\\0\" in the end; pos = %d; ch = %c\n", p, str[p]);
        return NULL;
    }
    return val;
}

tree_node_t * getE(void)
{
    tree_node_t * val = getT();
    while (str[p] == '+'|| str[p] == '-')
    {
        char op = str[p];
        p++;
        tree_node_t * val2 = getT();
        if (op == '+') val = ADD(val, val2);
        else           val = SUB(val, val2);
    }
    return val;
}

tree_node_t * getT(void)
{
    tree_node_t * val = getD();
    while (str[p] == '*'|| str[p] == '/')
    {
        char op = str[p];
        p++;
        tree_node_t * val2 = getD();
        if (op == '*') val = MUL(val, val2);
        else
        {
            if (is_equal(val2->value, 0))
            {
                fprintf(log_file, "Func: getT; ERROR: division by zero; pos = %d\n", p);
                return NULL;
            }
            val = DIV(val, val2);
        }
    }
    return val;
}

tree_node_t * getD(void)
{
    tree_node_t * val = getP();

    while (str[p] == '^')
    {
        p++;
        tree_node_t * val2 = getP();
        val = POW(val, val2);
    }
    return val;
}

tree_node_t * getP(void)
{
    if (str[p] == '(')
    {
        p++;
        tree_node_t * val = getE();
        if (str[p] != ')')
        {
            fprintf(log_file, "Func: getP; ERROR: no \")\", pos = %d, ch = %c/n", p, str[p]);
            return NULL;
        }
        p++;
        return val;
    }
    else return getN();
}

tree_node_t * getN(void)
{
    double val = 0;
    int saved_p = p, is_neg = 0, d_after_dot = -1;


    if (str[p] == '-')
    {
        is_neg = 1;
        p++;
    }

    while (('0' <= str[p] && str[p] <= '9') || str[p] == '.')
    {
        if (d_after_dot >= 0)
            d_after_dot++;

        if (str[p] == '.')
        {
            if (d_after_dot == -1)
                d_after_dot++;
            else
            {
                fprintf(log_file, "Func: getN; ERROR: second \".\" in number; pos = %d\n", p);
                return NULL;
            }
        }
        else
            val = val * 10 + str[p] - '0';

        p++;
    }
    if (p == saved_p)
    {
        fprintf(log_file, "Func: getN; ERROR: digit didn't find; pos = %d; ch = %c\n", p, str[p]);
        return NULL;
    }
    double ret_val = (is_neg ? -1 : 1) * val / pow(10, (d_after_dot == -1) ? 0 : d_after_dot);
    return NUM(ret_val);
}
