#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"

int main(void)
{
    open_log_file();

    printf("%lg", getG("3^"));

    close_log_file();
    return 0;
}

//     tree_t tree = {};
//     tree_ctor(&tree);
//
//     tree_read_expression(&tree, "./read/expr.txt");
//     tree_dump(&tree);
//     //printf("%lg", eval(tree.root));
//
//     tree_t d_tree = {};
//     tree_ctor(&d_tree);
//     d_tree.root = diff(tree.root);
//
//     tree_dump(&d_tree);
//
//     tree_make_expression(&d_tree, IN, "./prints/tree_print.txt");
//
//     tree_dtor(&tree);
//     tree_dtor(&d_tree);
