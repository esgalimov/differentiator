#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"

int main(void)
{
    open_log_file();
    tree_t tree = {};
    tree_ctor(&tree);

    tree_read_expression(&tree, IN, "./read/expr.txt");
    tree_dump(&tree);
    printf("%d", have_var(tree.root));
    tree_simplify(&tree.root);
    tree_dump(&tree);

    // tree_t dtree = {};
    // tree_ctor(&dtree);
    // dtree.root = diff(tree.root);
    // tree_dump(&dtree);
    // tree_eval_simplify(dtree.root);
    // tree_dump(&dtree);
    // tree_make_expression(&dtree, IN, "./prints/tree_print.txt");
    //tree_dtor(&dtree);

    tree_dtor(&tree);
    close_log_file();
    return 0;
}

