#ifndef _DEQUE_H_
#define _DEQUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// deque: type* / size_t*
//
// (size_t)size, (size_t)capacity,
// (size_t)firstItemIndex, (size_t)lastItemIdex,
// (type)data...
// ^ user pointer

#ifndef Deque_calloc
#define Deque_calloc calloc
#endif

#ifndef Deque_free
#define Deque_free free
#endif

static inline int
__Deque_assert(int cond, const char* expr, const char* file, int line){
    if (!cond){
        fprintf(stderr, 
               "assertion \"%s\" failed: file \"%s\", line %d\n",
               expr, file, line);
        abort();
    }
    return 1;
}

#define Deque_assert(__cond) \
    __Deque_assert((__cond) ? 1 : 0, #__cond, __FILE__, __LINE__)

#define Deque(__type) __type*

#define Deque_iter(__type) __type*

#define Deque_citer(__type) const __type*

#define Deque_new(__type, ...) \
    ((Deque(__type))__Deque_new( \
        sizeof((__type[]){__VA_ARGS__}) / sizeof(__type) + 1, \
        sizeof(__type), \
        (__type[]){__VA_ARGS__}))

#define Deque_del(__deque) \
    Deque_free(__Deque_basePtr(__deque))

#define Deque_capacity(__deque) \
    ((__deque) ? ((size_t*)(__deque))[-3] : 0)

#define Deque_size(__deque) \
    ((__deque) ? ((size_t*)(__deque))[-4] : 0)

#define Deque_at(__deque, __pos) \
    ((__deque)[((__Deque_basePtr(__deque)[2] + (__pos)) % \
               Deque_capacity(__deque)) * \
               Deque_assert((__deque) && \
                   (__pos) >= 0 && \
                   (__pos) < Deque_size(__deque))])

#define Deque_push_front(__deque, ...) \
    do { \
        if (Deque_size(__deque) + 1 >= Deque_capacity(__deque)) { \
            __Deque_grow((__deque), __Deque_nextGrowSize(__deque)); \
        } \
        size_t* __deque_ptr = __Deque_basePtr(__deque); \
        __deque_ptr[0]++; \
        __deque_ptr[2] = (__deque_ptr[2] - 1) % Deque_capacity(__deque); \
        (__deque)[__deque_ptr[2]] = (__VA_ARGS__); \
    }while(0)

#define Deque_push_back(__deque, ...) \
    do { \
        if (Deque_size(__deque) + 1 >= Deque_capacity(__deque)) { \
            __Deque_grow((__deque), __Deque_nextGrowSize(__deque)); \
        } \
        size_t* __deque_ptr = __Deque_basePtr(__deque); \
        __deque_ptr[0]++; \
        __deque_ptr[3] = (__deque_ptr[3] + 1) % Deque_capacity(__deque); \
        (__deque)[__deque_ptr[3]] = (__VA_ARGS__); \
    }while(0)

#define Deque_pop_back(__deque) \
    do { \
        if (!Deque_size(__deque)) { break; } \
        size_t* __deque_ptr = __Deque_basePtr(__deque); \
        __deque_ptr[0]--; \
        __deque_ptr[3] = (__deque_ptr[3] - 1) % Deque_capacity(__deque); \
    }while(0)

#define Deque_pop_front(__deque) \
    do { \
        if (!Deque_size(__deque)) { break; } \
        size_t* __deque_ptr = __Deque_basePtr(__deque); \
        __deque_ptr[0]--; \
        __deque_ptr[2] = (__deque_ptr[2] + 1) % Deque_capacity(__deque); \
    }while(0)

#define Deque_begin(__deque) \
    ((__deque) ? &(__deque)[__Deque_basePtr(__deque)[2]] : NULL)

#define Deque_end(__deque) \
    ((__deque) ? \
        &(__deque)[(__Deque_basePtr(__deque)[3] + 1) % Deque_capacity(__deque)] : \
        NULL)

#define Deque_cbegin(__deque) \
    ((const __Deque_type(__deque)*)Deque_begin(__deque))

#define Deque_cend(__deque) \
    ((const __Deque_type(__deque)*)Deque_end(__deque))

#define Deque_iter_next(__iter, __deque) \
    ((__iter) = &(__deque)[ \
        ((__iter) + 1 - (__deque)) % Deque_capacity(__deque)])




#define __Deque_basePtr(__deque) \
    ((__deque) ? &((size_t*)(__deque))[-4] : NULL)

#define __Deque_nextGrowSize(__deque) \
    ((__deque) ? Deque_capacity(__deque) << 1 : 1)

#define __Deque_type(__deque) __typeof__(*(__deque))

#define __Deque_grow(__deque, __count) \
    do { \
        size_t __deque_count = (__count); \
        Deque_assert(__deque_count >= Deque_size(__deque)); \
        size_t __deque_size = \
            sizeof(size_t)*4 + __deque_count*sizeof(__typeof__(*(__deque))); \
        size_t* __deque_ptr; \
        if ((__deque)) { \
            __deque_ptr = calloc(1, __deque_size); \
            __deque_ptr[0] = __Deque_basePtr(__deque)[0]; \
            __deque_ptr[1] = __deque_count; \
            __deque_ptr[3] = __deque_ptr[0] - 1; \
            for (int __deque_i = 0; \
                __deque_i < __deque_ptr[0]; \
                __deque_i++) \
            { \
                ((__Deque_type(__deque)*)&__deque_ptr[4])[__deque_i] = \
                        Deque_at((__deque), __deque_i); \
            } \
        }else{ \
            __deque_ptr = calloc(1, __deque_size); \
            Deque_assert(__deque_ptr); \
        } \
        __deque_ptr[1] = __deque_count; \
        (__deque) = (__Deque_type(__deque)*)(&__deque_ptr[4]); \
    }while(0)

static inline void* __Deque_new(size_t count, size_t size, void* data){
    if (!count) return NULL;

    size_t dequesize = sizeof(size_t)*4 + size * count;
    size_t* d = Deque_calloc(1, dequesize);

    d[0] = count-1;
    d[1] = count;
    d[3] = count-2;
    memcpy(d+4, data, size * count);

    return d+4;
}

#endif
