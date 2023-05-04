#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/diff.h"
#include "../include/dsl.h"
#include "../include/latex_funcs.h"

FILE* latex_file = NULL;

int open_latex_file(void)
{
    latex_file = fopen("./tmp/main.tex", "w");

    if (graphviz_file == NULL)
    {
        printf("Can't open latex file\n");
        return 1;
    }
    return 0;
}

int close_latex_file(void)
{
    if (latex_file == NULL)
    {
        printf("Latex file has NULL pointer, can't close it\n");
        return 1;
    }
    fprintf(latex_file, "\\end{document}");
    fclose(latex_file);
    return 0;
}

int init_latex_file(void)
{
    ASSERT(latex_file);

    fprintf(latex_file, "\\documentclass{article}\n");
    fprintf(latex_file, "\\usepackage[english]{babel}\n");
    fprintf(latex_file, "\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\n");
    fprintf(latex_file, "\\DeclareMathSizes{10}{10}{10}{10}\n");
    fprintf(latex_file, "\\usepackage{amsmath}\n");
    fprintf(latex_file, "\\usepackage{graphicx}\n");
    fprintf(latex_file, "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n");
    fprintf(latex_file, "\\title{Differentiator by Emil Galimov}\n");
    fprintf(latex_file, "\\begin{document}\n");
    fprintf(latex_file, "\\maketitle\n");
    fprintf(latex_file, "\\section{Function and its derivative}\n");

    return 0;
}

void print_expr_latex(tree_node_t* node)
{
    ASSERT(latex_file);

    fprintf(latex_file, "\\begin{center}\n$");
    print_subtree_latex(node);
    fprintf(latex_file, "$\\\\\n\\end{center}\n");
}

void print_subtree_latex(tree_node_t* node)
{
    if (node == NULL) return;

    if (node->type == TYPE_ADD || node->type == TYPE_SUB)
        fprintf(latex_file, "(");
    else if (node->type == TYPE_DIV)
        fprintf(latex_file, "\\frac{");

    print_subtree_latex(node->left);

    if (node->type == TYPE_DIV)
        fprintf(latex_file, "}{");

    switch (node->type)
    {
        case TYPE_NUM: fprintf(latex_file, "%lg", node->value);       break;
        case TYPE_VAR: fprintf(latex_file, "%c", (char) node->value); break;
        case TYPE_ADD: fprintf(latex_file, "+");                      break;
        case TYPE_SUB: fprintf(latex_file, "-");                      break;
        case TYPE_MUL: fprintf(latex_file, " \\cdot ");               break;
        case TYPE_SIN: fprintf(latex_file, " \\sin{");                break;
        case TYPE_COS: fprintf(latex_file, " \\cos{");                break;
        case TYPE_LN:  fprintf(latex_file, " \\ln{");                 break;
        case TYPE_POW: fprintf(latex_file, "^{");                     break;
        case TYPE_EXP: fprintf(latex_file, " \\exp{");                break;
    }

    print_subtree_latex(node->right);
    if ((node->type >= TYPE_SIN && node->type <= TYPE_EXP) || node->type == TYPE_DIV)
        fprintf(latex_file, "}");
    if (node->type == TYPE_ADD || node->type == TYPE_SUB)
        fprintf(latex_file, ")");
}

void create_pdf(void)
{
    close_latex_file();
    system("latex -interaction=batchmode -output-format=pdf ./tmp/main.tex");
    system("open main.pdf");
    system("rm main.log");
    system("rm main.aux");
    system("rm main.out");
}
