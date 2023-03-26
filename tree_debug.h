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
    //#define list_dump(list) list_dump_((list), __PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
    #define ASSERT(condition)
    //#define list_dump(list)
#endif

#endif
