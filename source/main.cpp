#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/diff.h"
#include "../include/dsl.h"

int main(void)
{
    open_log_file();
    tree_t tree = {};
    tree_ctor(&tree);

    tree_read_expression(&tree, IN, "./expr.txt");
    tree_dump(&tree);
    tree_simplify(&tree, &tree.root);
    tree_dump(&tree);

    tree_t dtree = {};
    tree_ctor(&dtree);
    dtree.root = diff(tree.root);
    tree_dump(&dtree);
    tree_simplify(&dtree, &dtree.root);
    tree_dump(&dtree);
    tree_make_expression(&dtree, IN, "./expr_print.txt");

    tree_dtor(&dtree);
    tree_dtor(&tree);
    close_log_file();
    return 0;
}

