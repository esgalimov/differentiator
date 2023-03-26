#include "tree.h"
#include "tree_debug.h"

FILE * graphviz_file = NULL;
FILE * log_file = NULL;
int dump_cnt = 0;

int open_graphviz_file(void)
{
    graphviz_file = fopen("./tmp/graphviz.dot", "w");

    if (graphviz_file == NULL)
    {
        printf("Can't open graphviz file\n");
        return 1;
    }
    return 0;
}

int close_graphviz_file(void)
{
    if (graphviz_file == NULL)
    {
        printf("Graphiz file has NULL pointer, can't close it\n");
        return 1;
    }
    fprintf(graphviz_file, "}");
    fclose(graphviz_file);
    return 0;
}

#ifdef LOG_MODE
    int open_log_file(void)
    {
        log_file = fopen("./logs/log.html", "w");

        if (log_file == NULL)
        {
            printf("Can't open log file\n");
            return 1;
        }

        fprintf(log_file, "<html>\n");
        return 0;
    }

    int close_log_file(void)
    {
        if (log_file == NULL)
        {
            printf("Log file has NULL pointer, can't close it\n");
            return 1;
        }
        fprintf(log_file, "</html>");
        fclose(log_file);
        return 0;
    }
#else
    int open_log_file(void)  { return 0; }
    int close_log_file(void) { return 0; }
#endif

int graphviz_init(tree_t * tree)
{
    ASSERT(tree != NULL);
    ASSERT(graphviz_file != NULL);

    fprintf(graphviz_file, "digraph\n{\n");
    fprintf(graphviz_file, "    node_info[shape = record, label = \"{root = %p}\"];\n\n", tree->root);
    fprintf(graphviz_file, "    node_info->node_%p [color = \"%s\"];\n", tree->root, GREEN);

    return 0;
}

void add_nodes(tree_node_t * node)
{
    if (node == NULL) return;

    if (node->type == TYPE_OP)
    {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {OP} | {%c} | {%p | %p}}\",\n\
                style=\"filled\", fillcolor=\"%s\"];\n", node, node, OPERATIONS[node->value - 1], node->left, node->right, BLUE);
    }
    else
    {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {NUM} | {%d} | {%p | %p}}\",\n\
                style=\"filled\", fillcolor=\"%s\"];\n", node, node, node->value, node->left, node->right, BLUE);
    }
    add_nodes(node->left);
    add_nodes(node->right);
}

void link_nodes(tree_node_t * node)
{
    if (node->left != NULL)
    {
        fprintf(graphviz_file, "    node_%p->node_%p [color = \"%s\"];\n", node, node->left, GREEN);
        link_nodes(node->left);
    }
    if (node->right != NULL)
    {
        fprintf(graphviz_file, "    node_%p->node_%p [color = \"%s\"];\n", node, node->right, YELLOW);
        link_nodes(node->right);
    }
}

char * create_graphviz_cmd(void)
{
    dump_cnt++;
    const char * begin_cmd = "dot ./tmp/graphviz.dot -Tpng -o ./logs/images/tree_dump";
    const char * end_cmd = ".png";
    char * cmd = (char *) calloc(100, sizeof(char));

    strcat(cmd, begin_cmd);
    snprintf(cmd + strlen(begin_cmd), 20, "%d", dump_cnt);
    strcat(cmd, end_cmd);

    return cmd;
}

int tree_dump_(tree_t * tree, const char * func, const char * file, int line)
{
    ASSERT(tree != NULL);

    char * graphviz_cmd = create_graphviz_cmd();

    fprintf(log_file, "<pre>\n%s at %s(%d):\n", func, file, line);

        fprintf(log_file, "Tree %p (<span style=\"color: green\">OK</span>) \"%s\" at %s at %s(%d):\n",
                tree, tree->info.name, tree->info.func, tree->info.file, tree->info.line);

        fprintf(log_file, "{\n    root = %p\n}", tree->root);
        fprintf(log_file, "</pre>\n");

    open_graphviz_file();
    graphviz_init(tree);
    add_nodes(tree->root);
    link_nodes(tree->root);
    close_graphviz_file();

    system(graphviz_cmd);

    free(graphviz_cmd);

    fprintf(log_file, "<img src=\"./images/tree_dump%d.png\">\n", dump_cnt);

    return 0;
}
