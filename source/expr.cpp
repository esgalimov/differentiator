#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/diff.h"
#include "../include/dsl.h"
#include "../include/expr.h"


expr_t* expr_ctor(const char* filename)
{
    ASSERT(filename);

    FILE * fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file \"%s\" for read tree</pre>\n", filename);
        return NULL;
    }

    expr_t* expr = (expr_t*) calloc(1, sizeof(expr_t));

    expr->buffer = read_buffer(fp);
    expr->pos = 0;
    expr->var_cnt = 0;
    expr->vars = (var_t**) calloc(VARS_MAX_CNT, sizeof(var_t*));

    tree_t* tree = (tree_t*) calloc(1, sizeof(tree_t));
    tree_ctor(tree);

    expr->tree = tree;

    link_root(expr->tree, getG(expr));

    tree_dump(expr->tree, expr);

    return expr;
}

char* read_buffer(FILE* stream)
{
    ASSERT(stream);

    fseek(stream, 0L, SEEK_END);
    size_t filesize = (size_t) ftell(stream);
    rewind(stream);

    char* buffer = (char*) calloc(filesize + 1, sizeof(char));
    fread(buffer, sizeof(char), filesize, stream);
    *(buffer + filesize) = '\0';

    return buffer;
}

int expr_dtor(expr_t* expr)
{
    ASSERT(expr);

    free(expr->buffer);

    for (int i = 0; i < expr->var_cnt; i++)
    {
        printf("COCK\n");
        free(expr->vars[i]->name);
        free(expr->vars[i]);
    }

    expr->var_cnt = POISON;
    expr->pos = POISON;
    free(expr->vars);

    tree_dtor(expr->tree);

    free(expr->tree);
    free(expr);

    return STATUS_OK;
}
