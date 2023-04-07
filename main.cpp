#include "tree.h"
#include "tree_debug.h"
#include "diff.h"

int main(void)
{
    open_log_file();
    tree_t tree = {};
    tree_ctor(&tree);

//     tree_read_expression(&tree, "./read/expr.txt");
//     tree_make_expression(&tree, PRE, "./prints/tree_print.txt");
//
//     printf("result = %d", eval(tree.root));

    tree_node_t * n1 = create_func(TYPE_SIN, create_var('x'));
    tree_node_t * n2 = create_func(TYPE_COS, create_num(5));
    tree_node_t * n3 = create_op(TYPE_ADD, n1, n2);

    link_root(&tree, n3);

    tree_dump(&tree);

    tree_dtor(&tree);
    close_log_file();

    return 0;
}



//     tree_node_t * n1 = create_node(TYPE_OP, OP_MUL);
//     tree_node_t * n2 = create_node(TYPE_OP, OP_ADD);
//     tree_node_t * n3 = create_node(TYPE_NUM, 5);
//     tree_node_t * n4 = create_node(TYPE_NUM, 7);
//     tree_node_t * n5 = create_node(TYPE_NUM, 10);
//
//     link_root(&tree, n1);
//     tree_dump(&tree);
//     link_node(n1, n2, LEFT);
//     tree_dump(&tree);
//     link_node(n1, n5, RIGHT);
//     tree_dump(&tree);
//     link_node(n2, n3, LEFT);
//     tree_dump(&tree);
//     link_node(n2, n4, RIGHT);
//     tree_dump(&tree);
//
//     tree_make_expression(&tree, POST);
