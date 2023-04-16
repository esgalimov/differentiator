#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"

const char * str = NULL;
int p = 0;

double getG(const char * exp)
{
    str = exp; p = 0;

    double val = getE();

    if (str[p] != '\0' && !is_equal(val, ERROR))
    {
        fprintf(log_file, "Func: getG; ERROR: must be \"\\0\" in the end; pos = %d; ch = %c\n", p, str[p]);
        return ERROR;
    }
    return val;
}

double getE(void)
{
    double val = getT();
    while (str[p] == '+'|| str[p] == '-')
    {
        char op = str[p];
        p++;
        double val2 = getT();
        if (op == '+') val += val2;
        else           val -= val2;
    }
    return val;
}

double getT(void)
{
    double val = getD();
    while (str[p] == '*'|| str[p] == '/')
    {
        char op = str[p];
        p++;
        double val2 = getD();
        if (op == '*') val *= val2;
        else
        {
            if (is_equal(val2, 0))
            {
                fprintf(log_file, "Func: getT; ERROR: division by zero; pos = %d\n", p);
                return ERROR;
            }
            val /= val2;
        }
    }
    return val;
}

double getD(void)
{
    double val = getP();

    while (str[p] == '^')
    {
        p++;
        double val2 = getP();
        val = pow(val, val2);
    }
    return val;
}

double getP(void)
{
    if (str[p] == '(')
    {
        p++;
        double val = getE();
        if (str[p] != ')')
        {
            fprintf(log_file, "Func: getP; ERROR: no \")\", pos = %d, ch = %c/n", p, str[p]);
            return ERROR;
        }
        p++;
        return val;
    }
    else return getN();
}

double getN(void)
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
                return ERROR;
            }
        }
        else
            val = val * 10 + str[p] - '0';

        p++;
    }
    if (p == saved_p)
    {
        fprintf(log_file, "Func: getN; ERROR: digit didn't find; pos = %d; ch = %c\n", p, str[p]);
        return ERROR;
    }
    return (is_neg ? -1 : 1) * val / pow(10, (d_after_dot == -1) ? 0 : d_after_dot);
}
