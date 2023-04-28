#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/diff.h"
#include "../include/dsl.h"


void tree_print_preorder(tree_node_t * node, FILE * stream) //(*(+(5)(7))(10))
{
    if (node == NULL) return;

    fprintf(stream, "(");

    if (node->type >= TYPE_ADD && node->type <= TYPE_DIV)
        fprintf(stream, "%c", OPERATIONS[node->type - 1]);
    else if (node->type == TYPE_NUM)
        fprintf(stream, "%lg", node->value);
    else if (node->type == TYPE_VAR)
        fprintf(stream, "%c", (char) node->value);
    else
    {
        switch (node->type)
        {
            case TYPE_SIN: fprintf(stream, "sin("); break;
            case TYPE_COS: fprintf(stream, "cos("); break;
            case TYPE_LN:  fprintf(stream, "ln(");  break;
            case TYPE_POW: fprintf(stream, "^(");   break;
            case TYPE_LOG: fprintf(stream, "log("); break;
            case TYPE_EXP: fprintf(stream, "exp("); break;

        }
    }

    tree_print_preorder(node->left, stream);
    tree_print_preorder(node->right, stream);

    if (node->type >= TYPE_SIN && node->type <= TYPE_EXP)
        fprintf(stream, ")");

    fprintf(stream, ")");
}

void tree_print_inorder(tree_node_t * node, FILE * stream)
{
    if (node == NULL) return;

    if (node->type != TYPE_LOG)
        tree_print_inorder(node->left, stream);

    if (node->type >= TYPE_ADD && node->type <= TYPE_DIV)
        fprintf(stream, "%c", OPERATIONS[node->type - 1]);
    else if (node->type == TYPE_NUM)
        fprintf(stream, "%lg", node->value);
    else if (node->type == TYPE_VAR)
        fprintf(stream, "%c", (char) node->value);
    else
    {
        switch (node->type)
        {
            case TYPE_SIN: fprintf(stream, "sin(");     break;
            case TYPE_COS: fprintf(stream, "cos(");     break;
            case TYPE_LN:  fprintf(stream, "ln(");      break;
            case TYPE_POW: fprintf(stream, "^(");       break;
            case TYPE_LOG: fprintf(stream, "log(");
                tree_print_inorder(node->left, stream); break;
            case TYPE_EXP: fprintf(stream, "exp(");     break;
        }
    }

    tree_print_inorder(node->right, stream);
    if (node->type >= TYPE_SIN && node->type <= TYPE_EXP)
        fprintf(stream, ")");
}

/*
void tree_print_postorder(tree_node_t * node, FILE * stream) // пока не трогаю
{
    if (node == NULL) return;

    tree_print_postorder(node->left, stream);
    tree_print_postorder(node->right, stream);

    if (node->type == TYPE_NUM)
        fprintf(stream, "push %lg\n", node->value);
    else
        switch (node->type)
        {
            case TYPE_ADD:
                fprintf(stream, "add\n");
                break;
            case TYPE_SUB:
                fprintf(stream, "sub\n");
                break;
            case TYPE_MUL:
                fprintf(stream, "mul\n");
                break;
            case TYPE_DIV:
                fprintf(stream, "div\n");
                break;
            default:
                fprintf(log_file, "<pre>Wrong operation</pre>\n");
        }
}*/
