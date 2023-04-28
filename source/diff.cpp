#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/diff.h"
#include "../include/dsl.h"


int tree_read_expression(tree_t * tree, mode read_mode, const char * filename)
{
    FILE * fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file \"%s\" for read tree</pre>\n", filename);
        return 1;
    }

    expr_text * expr = (expr_text *) calloc(1, sizeof(expr_text));

    fseek(fp, 0L, SEEK_END);
    size_t filesize = (size_t) ftell(fp);
    rewind(fp);

    expr->buffer = (char *) calloc(filesize + 1, sizeof(char));
    fread(expr->buffer, sizeof(char), filesize, fp);
    *(expr->buffer + filesize) = '\0';

    if (read_mode == PRE)
    {
        int pos = 0;
        link_root(tree, tree_read_preorder(expr->buffer, &pos));
    }
    else if (read_mode == IN)
    {
        link_root(tree, getG(expr));
    }
    else
        fprintf(log_file, "<pre>%d read mode doesn't exist\n</pre>", read_mode);

    free(expr->buffer);
    free(expr);
    return 0;
}

int tree_print_expression(tree_t * tree, mode print_mode, const char * filename)
{
    ASSERT(tree != NULL);

    FILE * fp = fopen(filename, "w");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file \"%s\" for tree print</pre>\n", filename);
        return 1;
    }

    switch (print_mode)
    {
        case PRE:
           tree_print_preorder(tree->root, fp);
           break;
        case IN:
           tree_print_inorder(tree->root, fp);
           break;
        case POST:
           //tree_print_postorder(tree->root, fp);
           fprintf(fp, "out\nhlt\n");
           break;
        default:
            fprintf(log_file, "<pre>Wrong print mode</pre>\n");
    }

    fclose(fp);
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
        return node->value;

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
            if (!isfinite(NAN))
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
