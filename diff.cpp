#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"


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
           //tree_print_postorder(tree->root, fp);
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

    if (node->type >= TYPE_ADD && node->type <= TYPE_DIV)
        fprintf(stream, "%c", OPERATIONS[node->type - 1]);
    else if (node->type == TYPE_NUM)
        fprintf(stream, "%lg", node->value);
    else if (node->type == TYPE_VAR)
        fprintf(stream, "%c", (char) node->value);
    else
    {
        switch (node->type)
        {
            case TYPE_SIN: fprintf(stream, "sin("); break;
            case TYPE_COS: fprintf(stream, "cos("); break;
            case TYPE_LN:  fprintf(stream, "ln(");  break;
            case TYPE_POW: fprintf(stream, "^(");   break;
            case TYPE_LOG: fprintf(stream, "log("); break;
            case TYPE_EXP: fprintf(stream, "exp("); break;

        }
    }

    tree_print_preorder(node->left, stream);
    tree_print_preorder(node->right, stream);

    if (node->type >= TYPE_SIN && node->type <= TYPE_EXP)
        fprintf(stream, ")");

    fprintf(stream, ")");
}

void tree_print_inorder(tree_node_t * node, FILE * stream) //(((5)+(7))*(10))
{
    if (node == NULL) return;

    fprintf(stream, "(");

    if (node->type != TYPE_LOG)
        tree_print_inorder(node->left, stream);

    if (node->type >= TYPE_ADD && node->type <= TYPE_DIV)
        fprintf(stream, "%c", OPERATIONS[node->type - 1]);
    else if (node->type == TYPE_NUM)
        fprintf(stream, "%lg", node->value);
    else if (node->type == TYPE_VAR)
        fprintf(stream, "%c", (char) node->value);
    else
    {
        switch (node->type)
        {
            case TYPE_SIN: fprintf(stream, "sin(");     break;
            case TYPE_COS: fprintf(stream, "cos(");     break;
            case TYPE_LN:  fprintf(stream, "ln(");      break;
            case TYPE_POW: fprintf(stream, "^(");       break;
            case TYPE_LOG: fprintf(stream, "log(");
                tree_print_inorder(node->left, stream); break;
            case TYPE_EXP: fprintf(stream, "exp(");     break;
        }
    }

    tree_print_inorder(node->right, stream);
    if (node->type >= TYPE_SIN && node->type <= TYPE_EXP)
        fprintf(stream, ")");
    fprintf(stream, ")");
}

/*
void tree_print_postorder(tree_node_t * node, FILE * stream) // пока не трогаю
{
    if (node == NULL) return;

    tree_print_postorder(node->left, stream);
    tree_print_postorder(node->right, stream);

    if (node->type == TYPE_NUM)
        fprintf(stream, "push %lg\n", node->value);
    else
        switch (node->type)
        {
            case TYPE_ADD:
                fprintf(stream, "add\n");
                break;
            case TYPE_SUB:
                fprintf(stream, "sub\n");
                break;
            case TYPE_MUL:
                fprintf(stream, "mul\n");
                break;
            case TYPE_DIV:
                fprintf(stream, "div\n");
                break;
            default:
                fprintf(log_file, "<pre>Wrong operation</pre>\n");
        }
}*/

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

//----------------------------read preorder part------------------------------
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
            return get_oper(buffer, pos, ch);
        }
        else if (isdigit(ch) || ch == '-')
            return get_number(buffer, pos);

        else if (isalpha(ch) || ch == '^')
            return get_func_or_var(buffer, pos);

        else if (ch == '(')
        {
            *pos += cnt;
            return tree_read_preorder(buffer, pos);
        }

        else
        {
            fprintf(log_file, "<pre>Undefind symbol: %c </pre>\n", ch);
            return NULL;
        }
    }
    else
    {
        fprintf(log_file, "<pre>Must start with \"(\" | ch = %c | pos = %d |</pre>\n", ch, *pos);
        return NULL;
    }
}

int check_close_bracket(char * buffer, int * pos)
{
    int cnt = 0;
    char ch = '\0';

    sscanf(buffer + *pos, "%c%n", &ch, &cnt);

    if (ch != ')')
    {
        fprintf(log_file, "<pre>Must be \")\" after node: pos = %d, ch = %c</pre>\n", *pos, ch);
        //subtree_dump(node_num);
        return 0;
    }

    return cnt;
}

tree_node_t * get_number(char * buffer, int * pos)
{
    elem_t value = 0;
    int cnt = 0;

    sscanf(buffer + *pos, "%lg%n", &value, &cnt);
    tree_node_t * node_num = create_num(value);
    *pos += cnt;

    cnt = check_close_bracket(buffer, pos);
    if (!cnt)
        return NULL;

    *pos += cnt;
    return node_num;
}

tree_node_t * get_oper(char * buffer, int * pos, char ch)
{
    node_type op = TYPE_MUL;

    switch (ch)
    {
        case '*':
            break;
        case '+':
            op = TYPE_ADD;
            break;
        case '-':
            op = TYPE_SUB;
            break;
        case '/':
            op = TYPE_DIV;
            break;
    }

    tree_node_t * node_ch1 = tree_read_preorder(buffer, pos);
    tree_node_t * node_ch2 = tree_read_preorder(buffer, pos);
    tree_node_t * node_op = create_op(op, node_ch1, node_ch2);

    int cnt = check_close_bracket(buffer, pos);
    if (!cnt)
        return NULL;

    *pos += cnt;

    return node_op;
}

tree_node_t * get_func_or_var(char * buffer, int * pos)
{
    char cmd[32];
    int cnt = 0;
    char ch = '\0';

    sscanf(buffer + *pos, "%[a-z^]%n", cmd, &cnt);
    *pos += cnt;

    sscanf(buffer + *pos, "%c%n", &ch, &cnt);
    int is_var = 0;
    *pos += cnt;

    if (ch != '(')
    {
        is_var = 1;
        *pos -= cnt;
    }

    tree_node_t * node = NULL;

    if (!strcmp(cmd, "sin"))
        node = SIN(tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "cos"))
        node = COS(tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "ln"))
        node = LN(tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "^"))
        node = POW(tree_read_preorder(buffer, pos),
                   tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "log"))
        node = LOG(tree_read_preorder(buffer, pos),
                   tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "exp"))
        node = EXP(tree_read_preorder(buffer, pos));
    else
    {
        node = VAR(cmd[0]);
        //if (is_var == 0)
    }

    cnt = check_close_bracket(buffer, pos);
    if (!cnt)
        return NULL;
    *pos += cnt;
    if (!is_var)
    {
        cnt = check_close_bracket(buffer, pos);
        if (!cnt)
            return NULL;
        *pos += cnt;
    }

    return node;
}
//--------------------------------------------------------------------

int is_operation(char ch)
{
    if (ch == '*' || ch == '/' || ch == '+' || ch == '-')
        return 1;
    return 0;
}

double eval(const tree_node_t * node)
{
    if (node == NULL)
    {
        fprintf(log_file, "<pre>Try to eval NULL node</pre>");
        return NAN;
    }
    if (node->type == TYPE_VAR)
    {
        fprintf(log_file, "<pre>Try to eval TYPE_VAR node</pre>");
        return NAN;
    }
    if (node->type == TYPE_NUM)
        return node->value;

    switch (node->type)
    {
        case TYPE_ADD: return evalL + evalR;
        case TYPE_SUB: return evalL - evalR;
        case TYPE_MUL: return evalL * evalR;
        case TYPE_DIV:
        {
            double eval_right = evalR;

            if (is_equal(eval_right, 0))
            {
                fprintf(log_file, "<pre>Division by zero</pre>");
                subtree_dump(node);
                return NAN;
            }
            return eval(node->left) / eval_right;
        }
        case TYPE_SIN: return sin(evalR);
        case TYPE_COS: return cos(evalR);
        case TYPE_LN:  return log(evalR);
        case TYPE_POW: return pow(evalL, evalR);
        case TYPE_LOG:
        {
            double eval_left = evalL;

            if (is_equal(eval_left, 1))
            {
                fprintf(log_file, "<pre>Log base is 1</pre>");
                subtree_dump(node);
                return NAN;
            }
            return log(evalL) / log(evalR);
        }
        case TYPE_EXP: return exp(evalR);
        default:
            fprintf(log_file, "<pre>Undefind operation</pre>");
            subtree_dump(node);
            return NULL_EVAL;
    }
}

tree_node_t * diff(tree_node_t * node)
{
    if (node == NULL) return NULL;

    switch (node->type)
    {
        case TYPE_NUM: return NUM(0);
        case TYPE_VAR: return NUM(1);
        case TYPE_ADD: return ADD(dL, dR);
        case TYPE_SUB: return SUB(dL, dR);
        case TYPE_MUL: return ADD(MUL(dL, cR), MUL(cL, dR));
        case TYPE_DIV: return DIV(SUB(MUL(dL, cR), MUL(cL, dR)), MUL(cR, cR));
        case TYPE_SIN: return MUL(COS(cR), dR);
        case TYPE_COS: return MUL(MUL(NUM(-1), SIN(cR)), dR);
        case TYPE_LN:  return DIV(dR, cR);
        case TYPE_POW:
        {
            if (node->left->type == TYPE_NUM && node->right->type == TYPE_NUM)
                return NUM(0);
            else if (node->left->type == TYPE_NUM && node->right->type != TYPE_NUM)
                return MUL(MUL(POW(cL, cR), LN(cL)), dR);
            else if (node->left->type != TYPE_NUM && node->right->type == TYPE_NUM)
                return MUL(MUL(cR, POW(cL, NUM(node->right->value - 1))), dL);
            else
            {
                tree_node_t * node_pow   = POW(cL, cR),
                            * node_sum_1 = DIV(MUL(dL, cR), cL),
                            * node_sum_2 = MUL(dR, LN(cL));
                return MUL(node_pow, ADD(node_sum_1, node_sum_2));
            }
        }
        case TYPE_LOG: return DIV(dR, MUL(LN(cL), cR));
        case TYPE_EXP: return MUL(EXP(cR), dR);
        default: return NULL;
    }
}

int is_equal(double num1, double num2)
{
    ASSERT(isfinite(num1));
    ASSERT(isfinite(num2));

    return fabs(num1 - num2) < EPS;
}

// tree_node_t * tree_simplify(tree_node_t * node, tree_t * tree)
// {
//     if ()...
// }
