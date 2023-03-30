#include "tree.h"
#include "tree_debug.h"

int tree_ctor_(tree_t * tree, var_info info)
{
    ASSERT(tree != NULL);

    tree->root = NULL;
    tree->info = info;

    return STATUS_OK;
}

int tree_dtor(tree_t * tree)
{
    ASSERT(tree != NULL);

    nodes_dtor(tree->root);

    tree->status = 0;

    #ifdef LOG_MODE
        fprintf(log_file, "<pre>\nTree %p \"%s\" at %s at %s(%d): DESTRUCTED\n</pre>\n",
                    tree, tree->info.name, tree->info.func, tree->info.file, tree->info.line);
    #endif

    tree->info.file = NULL;
    tree->info.func = NULL;
    tree->info.line = 0;

    return STATUS_OK;
}

void nodes_dtor(tree_node_t * node)
{
    if (node == NULL) return;
    if (node != node->left)
        nodes_dtor(node->left);
    if (node != node->right)
        nodes_dtor(node->right);

    free(node);
}

int link_root(tree_t * tree, tree_node_t * root)
{
    ASSERT(tree != NULL);

    if (root == NULL) return LINK_NULL_ROOT;

    tree->root = root;

    return STATUS_OK;
}

tree_node_t * create_node(node_type type, elem_t value)
{
    if (type != TYPE_NUM && type != TYPE_OP)
        return NULL;

    tree_node_t * node = (tree_node_t *) calloc(1, sizeof(tree_node_t));

    if (node == NULL)
        return NULL;

    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

int link_node(tree_node_t * parent, tree_node_t * child, link_mode mode)
{
    if (parent == NULL || child == NULL) return NODE_LINK_ERROR;
    if (mode != LEFT && mode != RIGHT)   return MODE_LINK_ERROR;

    if (mode == LEFT)
        parent->left = child;
    else
        parent->right = child;

    child->parent = parent;

    return STATUS_OK;
}

int tree_make_expression(tree_t * tree, print_mode mode)
{
    ASSERT(tree != NULL);

    FILE * fp = fopen("./prints/tree_print.txt", "w");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file for tree print</pre>\n");
        return 1;
    }

    switch (mode)
    {
        case PRE:
           tree_print_preorder(tree->root, fp);
           break;
        case IN:
           tree_print_inorder(tree->root, fp);
           break;
        case POST:
           tree_print_postorder(tree->root, fp);
           fprintf(fp, "out\nhlt\n");
           break;
        default:
            fprintf(log_file, "<pre>Wrong print mode</pre>\n");
    }

    fclose(fp);
    return 0;
}

void tree_print_preorder(tree_node_t * node, FILE * stream) //(*(+(5)(7))(10))
{
    if (node == NULL) return;

    fprintf(stream, "(");

    if (node->type == TYPE_OP)
        fprintf(stream, "%c", OPERATIONS[node->value - 1]);
    else
        fprintf(stream, "%d", node->value);

    tree_print_preorder(node->left, stream);
    tree_print_preorder(node->right, stream);
    fprintf(stream, ")");
}

void tree_print_inorder(tree_node_t * node, FILE * stream) //(((5)+(7))*(10))
{
    if (node == NULL) return;

    fprintf(stream, "(");
    tree_print_inorder(node->left, stream);

    if (node->type == TYPE_OP)
        fprintf(stream, "%c", OPERATIONS[node->value - 1]);
    else
        fprintf(stream, "%d", node->value);

    tree_print_inorder(node->right, stream);
    fprintf(stream, ")");
}

void tree_print_postorder(tree_node_t * node, FILE * stream)
{
    if (node == NULL) return;

    tree_print_postorder(node->left, stream);
    tree_print_postorder(node->right, stream);

    if (node->type == TYPE_NUM)
        fprintf(stream, "push %d\n", node->value);
    else
        switch (node->value)
        {
            case OP_ADD:
                fprintf(stream, "add\n");
                break;
            case OP_SUB:
                fprintf(stream, "sub\n");
                break;
            case OP_MUL:
                fprintf(stream, "mul\n");
                break;
            case OP_DIV:
                fprintf(stream, "div\n");
                break;
            default:
                fprintf(log_file, "<pre>Wrong operation</pre>\n");
        }
}
