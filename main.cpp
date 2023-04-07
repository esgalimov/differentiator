#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"

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
    tree_node_t * nlog = create_func(TYPE_POW, create_num(2), create_num(8));
    tree_node_t * n2 = create_func(TYPE_COS, nlog);
    tree_node_t * n3 = create_op(TYPE_ADD, n1, n2);

    link_root(&tree, n3);

    tree_t tree1 = {};
    tree_ctor(&tree1);

    tree1.root = diff(tree.root);

    tree_make_expression(&tree1, IN, "./prints/tree_print.txt");

    tree_dump(&tree);
    tree_dump(&tree1);

    tree_dtor(&tree1);
    tree_dtor(&tree);
    close_log_file();

    return 0;
}

