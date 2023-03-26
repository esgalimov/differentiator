#include "tree.h"
#include "tree_debug.h"

int tree_ctor_(tree_t * tree, var_info info)
{
    ASSERT(tree != NULL);

    tree->root = NULL;
    tree->info = info;

    return 0;
}

int tree_dtor(tree_t * tree)
{
    ASSERT(tree != NULL);

    nodes_dtor(tree->root);

    tree->status = 0;
    tree->info.file = NULL;
    tree->info.func = NULL;
    tree->info.line = 0;

    return 0;
}

void nodes_dtor(tree_node_t * node)
{
    if (node == NULL) return;

    nodes_dtor(node->left);
    nodes_dtor(node->right);
    free(node);
}

int link_root(tree_t * tree, tree_node_t * root)
{
    ASSERT(tree != NULL);

    if (root == NULL)
        return 1;

    tree->root = root;

    return 0;
}

tree_node_t * create_node(int type, elem_t value)
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

    return node;
}

int link_node(tree_node_t * parent, tree_node_t * child, int mode)
{
    if (parent == NULL && child == NULL) return 1;
    if (mode != LEFT && mode != RIGHT)   return 1;

    if (mode == LEFT)
        parent->left = child;
    else
        parent->right = child;

    return 0;
}

void tree_print_preorder(tree_node_t * node)
{
    if (node == NULL) return;

    printf("%d -> ", node->value);
    tree_print_preorder(node->left);
    tree_print_preorder(node->right);
}

void tree_print_inorder(tree_node_t * node)
{
    if (node == NULL) return;

    tree_print_inorder(node->left);
    printf("%d -> ", node->value);
    tree_print_inorder(node->right);
}

void tree_print_postorder(tree_node_t * node)
{
    if (node == NULL) return;

    tree_print_postorder(node->left);
    tree_print_postorder(node->right);
    printf("%d -> ", node->value);
}
