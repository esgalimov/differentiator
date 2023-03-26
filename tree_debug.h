#ifndef DEBUG_TREE
#define DEBUG_TREE

#include "tree.h"

#define LOG_MODE

#ifdef LOG_MODE
    #define ASSERT(condition)                                                               \
        if (!(condition))                                                                   \
        {                                                                                   \
            printf("\nError in \"%s\" in %d line in function %s in file %s\n",              \
                    #condition, __LINE__, __PRETTY_FUNCTION__, __FILE__);                   \
            abort();                                                                        \
        }
    #define tree_dump(list) tree_dump_((list), __PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
    #define ASSERT(condition)
    #define tree_dump(list)
#endif

#define BLUE   "#87CEEB"
#define D_BLUE "#0000CD"
#define RED    "#FFD0D0"
#define D_RED  "#DC143C"
#define YELLOW "#F0E68C"
#define GREEN  "#98FB98"

#define OPERATIONS "+-*/"

//! @brief File to generate graphviz picture
extern FILE * graphviz_file;

//! @brief File to logs
extern FILE * log_file;

//! @brief Dump counter to make pictures names
extern int dump_cnt;

//! @brief Func to open graphiz_file
int open_graphviz_file(void);

//! @brief Func to close graphiz_file
//! write "}"before closing to finish making graphiz file
int close_graphviz_file(void);

//! @brief Func to open log_file and write some html tags to start logging
int open_log_file(void);

//! @brief Func to close log_file
//! write "</html>" before closing to finish making log file
int close_log_file(void);

//! @brief Graphiz file init
//! @param [in] list - ptr to tree's struct
int graphviz_init(tree_t * tree);

//! @brief Add nodes into .dot file to use graphiz
//! @param [in] node - ptr to node
void add_nodes(tree_node_t * node);

//! @brief Link nodes in .dot file
//! @param [in] node - ptr to node
void link_nodes(tree_node_t * node);

//! @brief Tree dump
//! Make .dot file using funcs above and use graphiz to make png picture of dump
//! @param [in] list - ptr to tree
//! @param [in] func - ptr to func name
//! @param [in] file - ptr to file name
//! @param [in] line - line where was dump
//! @return 0
int tree_dump_(tree_t * tree, const char * func, const char * file, int line);

//! @brief Func create string with graphiz console command (make png from .dot file)
//! it use global variable 'dump_number' to create name of next picture
//! cmd string buffer allocated with calloc (don't forget to free)
//! @return ptr to string
char * create_graphviz_cmd(void);


#endif
