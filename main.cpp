#include "tree.h"
#include "tree_debug.h"

int main(void)
{
    tree_t tree = {};

    tree_ctor(&tree);
    tree_node_t * n1 = create_node(TYPE_OP, OP_MUL);
    tree_node_t * n2 = create_node(TYPE_OP, OP_ADD);
    tree_node_t * n3 = create_node(TYPE_NUM, 5);
    tree_node_t * n4 = create_node(TYPE_NUM, 7);
    tree_node_t * n5 = create_node(TYPE_NUM, 10);

    link_root(&tree, n1);
    link_node(n1, n2, LEFT);
    link_node(n1, n5, RIGHT);
    link_node(n2, n3, LEFT);
    link_node(n2, n4, RIGHT);

    tree_print_inorder(n1);
    printf("\n");
    tree_print_preorder(n1);
    printf("\n");
    tree_print_postorder(n1);

    tree_dtor(&tree);

    return 0;
}
