#include "tree.h"
#include "tree_debug.h"
#include "diff.h"


int tree_make_expression(tree_t * tree, print_mode mode, const char * filename)
{
    ASSERT(tree != NULL);

    FILE * fp = fopen(filename, "w");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file \"%s\" for tree print</pre>\n", filename);
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

int tree_read_expression(tree_t * tree, const char * filename)
{
    FILE * fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file \"%s\" for read tree</pre>\n", filename);
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    size_t filesize = (size_t) ftell(fp);
    rewind(fp);

    char * buffer = (char *) calloc(filesize + 1, sizeof(char));
    fread(buffer, sizeof(char), filesize, fp);
    *(buffer + filesize) = '\0';

    int pos = 0;
    link_root(tree, tree_read_preorder(buffer, &pos));

    free(buffer);
    return 0;
}

tree_node_t * tree_read_preorder(char * buffer, int * pos)
{
    char ch = '\0';
    int cnt = 0;

    sscanf(buffer + *pos, "%c%n", &ch, &cnt);
    *pos += cnt;

    if (ch == '(')
    {
        sscanf(buffer + *pos, "%c%n", &ch, &cnt);

        if (is_operation(ch))
        {
            *pos += cnt;
            operation op = OP_MUL;

            switch (ch)
            {
                case '*':
                    break;
                case '+':
                    op = OP_ADD;
                    break;
                case '-':
                    op = OP_SUB;
                    break;
                case '/':
                    op = OP_DIV;
                    break;
            }

            tree_node_t * node_op = create_node(TYPE_OP, op);
            tree_node_t * node_ch1 = tree_read_preorder(buffer, pos);
            tree_node_t * node_ch2 = tree_read_preorder(buffer, pos);
            link_node(node_op, node_ch1, LEFT);
            link_node(node_op, node_ch2, RIGHT);

            sscanf(buffer + *pos, "%c%n", &ch, &cnt);
            if (ch != ')')
            {
               fprintf(log_file, "<pre>Must be \")\" after node</pre>\n");
               subtree_dump(node_op);
               return NULL;
            }
            *pos += cnt;

            return node_op;
        }

        else if (isdigit(ch))
        {
            elem_t value = 0;
            sscanf(buffer + *pos, "%d%n", &value, &cnt);
            tree_node_t * node_num = create_node(TYPE_NUM, value);
            *pos += cnt;

            sscanf(buffer + *pos, "%c%n", &ch, &cnt);
            if (ch != ')')
            {
               fprintf(log_file, "<pre>Must be \")\" after node</pre>\n");
               subtree_dump(node_num);
               return NULL;
            }
            *pos += cnt;

            return node_num;
        }
        else
        {
            fprintf(log_file, "<pre>Undefind symbol</pre>\n");
            return NULL;
        }
    }
    else
    {
        fprintf(log_file, "<pre>Must start with \"(\" | ch = %c | pos = %d |</pre>\n", ch, *pos);
        return NULL;
    }
}

int is_operation(char ch)
{
    if (ch == '*' || ch == '/' || ch == '+' || ch == '-')
        return 1;
    return 0;
}

int eval(const tree_node_t * node)
{
    if (node == NULL)
    {
        fprintf(log_file, "<pre>Try to eval NULL node</pre>");
        return NULL_EVAL;
    }
    if (node->type == TYPE_NUM)
        return node->value;

    switch (node->value)
    {
        case OP_ADD:
            return eval(node->left) + eval(node->right);

        case OP_SUB:
            return eval(node->left) - eval(node->right);

        case OP_MUL:
            return eval(node->left) * eval(node->right);

        case OP_DIV:
        {
            int eval_right = eval(node->right);

            if (eval_right == 0)
            {
                fprintf(log_file, "<pre>Division by zero</pre>");
                subtree_dump(node);
                return NULL_EVAL;
            }
            return eval(node->left) / eval_right;
        }
        default:
            fprintf(log_file, "<pre>Undefind operation</pre>");
            subtree_dump(node);
            return NULL_EVAL;
    }
}
