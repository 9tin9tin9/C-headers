#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdlib.h>
#include <string.h>

// Forward List: type* / void**
// (type*)next user pointer, (type)data
//                           ^ user pointer
// First in last out

#ifndef FList_calloc
#define FList_calloc calloc
#endif

#ifndef FList_free
#define FList_free free
#endif

#define FList(__type) __type*

#define FList_iter(__type) __type*

#define FList_citer(__type) const __type*

#define FList_new(__type) ((__type*)NULL)

#define FList_del(__flist) \
    do { \
        void** __flist_ptr = __FList_basePtr(__flist); \
        while(__flist_ptr) { \
            void* __flist_next = __flist_ptr[0]; \
            free(__flist_ptr); \
            __flist_ptr = __FList_basePtr(__flist_next); \
        }\
        (__flist) = ((__FList_type(__flist)*)NULL); \
    }while(0)

#define FList_push_front(__flist, ...) \
    do { \
        size_t __flist_size = sizeof(__FList_type(__flist)) + \
                              sizeof(void*); \
        void** tmp = calloc(1, __flist_size); \
        tmp[0] = (__flist); \
        *(__FList_type(__flist)*)&tmp[1] = (__VA_ARGS__); \
        __flist = (__FList_type(__flist)*)&tmp[1]; \
    }while(0)

#define FList_pop_front(__flist) \
    do { \
        void* next = __FList_basePtr(__flist)[0]; \
        free((__flist)); \
        (__flist) = next; \
    }while(0)

#define FList_begin(__flist) \
    (__flist)

#define FList_end(__flist) \
    (NULL)

#define FList_cbegin(__flist) \
    ((const __FList_type(__flist)*)(__flist))

#define FList_cend(__flist) \
    (NULL)

#define FList_iter_next(__iter, ...) \
    ((__iter) = __FList_basePtr(__iter)[0])

#define FList_remove(__flist, __value) \
    do { \
        FList_iter(__FList_type(__flist)) __flist_last = NULL; \
        for (FList_iter(__FList_type(__flist)) __flist_it = FList_begin(__flist); \
            __flist_it != FList_end(__flist); \
            FList_iter_next(__flist_it)) \
        { \
            if (memcmp(__flist_it, \
                      (__typeof__(__value)[]){(__value)}, \
                      sizeof(__typeof__(__value)))) \
            { \
                __flist_last = __flist_it; \
                continue; \
            }\
            if (__flist_it == (__flist)) { \
                FList_del(__flist); \
                break; \
            } \
            __FList_basePtr(__flist_last)[0] = \
                __FList_basePtr(__flist_it)[0]; \
            free(__FList_basePtr(__flist_it)); \
            break; \
        } \
    }while(0)

#define FList_remove_if(__flist, __if) \
    do { \
        FList_iter(__FList_type(__flist)) __flist_last = NULL; \
        for (FList_iter(__FList_type(__flist)) __flist_it = FList_begin(__flist); \
            __flist_it != FList_end(__flist); \
            FList_iter_next(__flist_it)) \
        { \
            if (!(__if)) { \
                __flist_last = __flist_it; \
                continue; \
            }\
            if (__flist_it == (__flist)) { \
                FList_del(__flist); \
                break; \
            } \
            __FList_basePtr(__flist_last)[0] = \
                __FList_basePtr(__flist_it)[0]; \
            free(__FList_basePtr(__flist_it)); \
            break; \
        } \
    }while(0)




#define __FList_type(__flist) __typeof__(*(__flist))

#define __FList_basePtr(__flist) \
    (__flist ? (&((void**)(__flist))[-1]) : NULL)

#define __FList_nextPointer(__flist) \
    (__FList_basePtr(__flist)[0])

#endif
