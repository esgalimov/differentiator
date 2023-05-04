#include "./include/tree.h"
#include "./include/tree_debug.h"
#include "./include/diff.h"
#include "./include/dsl.h"

int main(void)
{
    open_log_file();

    diff_expression("./expr.txt");

    close_log_file();
    return 0;
}

