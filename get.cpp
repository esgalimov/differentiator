#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"

const char * str = NULL;
int p = 0;

int getG(const char * exp)
{
    str = exp; p = 0;

    int val = getE();

    if (str[p] != '\0')
    {
        fprintf(log_file, "Func: getG; ERROR: must be \"\\0\" in the end; pos = %d; ch = %c\n", p, str[p]);
        return ERROR;
    }
    return val;
}

int getN(void)
{
    int val = 0, saved_p = p;

    while ('0' <= str[p] && str[p] <= '9')
    {
        val = val * 10 + str[p] - '0';
        p++;
    }
    if (p == saved_p)
    {
        fprintf(log_file, "Func: getN; ERROR: digit didn't find; pos = %d; ch = %c\n", p, str[p]);
        return ERROR;
    }
    return val;
}

int getE(void)
{
    int val = getT();
    while (str[p] == '+'|| str[p] == '-')
    {
        char op = str[p];
        p++;
        int val2 = getT();
        if (op == '+') val += val2;
        else           val -= val2;
    }
    return val;
}

int getT(void)
{
    int val = getP();
    while (str[p] == '*'|| str[p] == '/')
    {
        char op = str[p];
        p++;
        int val2 = getP();
        if (op == '*') val *= val2;
        else
        {
            if (val2 == 0)
            {
                fprintf(log_file, "Func: getT; ERROR: division by zero; pos = %d\n", p);
                return ERROR;
            }
            val /= val2;
        }
    }
    return val;
}

int getP(void)
{
    if (str[p] == '(')
    {
        p++;
        int val = getE();
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
