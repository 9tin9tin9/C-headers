#ifndef _VEC_H_
#define _VEC_H_

#include <stdlib.h>
#include <assert.h>

// vec: type* / size_t*
// (size_t)size, (size_t)capacity, (type)data...
//                                 ^ user pointer

#ifndef Vec_calloc
#define Vec_calloc calloc
#endif

#ifndef Vec_free
#define Vec_free free
#endif

#ifndef Vec_realloc
#define Vec_realloc realloc
#endif

#define Vec(__type) __type*

#define Vec_iter(__type) __type*

#define Vec_citer(__type) const __type*

#define Vec_new(__type) ((Vec(__type))NULL)

#define Vec_del(__vec) \
    Vec_free(__Vec_basePtr(__vec))

#define Vec_capacity(__vec) \
    ((__vec) ? ((size_t*)(__vec))[-1] : 0)

#define Vec_size(__vec) \
    ((__vec) ? ((size_t*)(__vec))[-2] : 0)

#define Vec_at(__vec, __pos) \
    (assert((__vec) && (__pos) >= 0 && (__pos) < Vec_size(__vec)), \
     (__vec)[(__pos)])

#define Vec_push_back(__vec, ...) \
    do { \
        if (Vec_size(__vec) >= Vec_capacity(__vec)) { \
            __Vec_grow((__vec), __Vec_nextGrowSize(__vec)); \
        } \
        size_t* __vec_ptr = __Vec_basePtr(__vec); \
        (__vec)[*__vec_ptr] = (__VA_ARGS__); \
        __vec_ptr[0]++; \
    }while(0)

#define Vec_pop_back(__vec) \
    do { \
        if (!Vec_size(__vec)) { break; } \
        size_t* __vec_ptr = __Vec_basePtr(__vec); \
        __vec_ptr[0]--; \
    }while(0)

#define Vec_reserve(__vec, __count) \
    do { \
        if (Vec_capacity(__vec) < (__count)) { \
            __Vec_grow((__vec), (__count)); \
        } \
    }while(0)

#define Vec_begin(__vec) \
    (__vec)

#define Vec_end(__vec) \
    ((__vec) ? &((__vec)[Vec_size(__vec)]) : NULL)

#define Vec_cbegin(__vec) \
    ((const __Vec_type(__vec)*)Vec_begin(__vec))

#define Vec_cend(__vec) \
    ((const __Vec_type(__vec)*)Vec_end(__vec))

#define Vec_iter_next(__iter) \
    ((__iter)++)




#define __Vec_basePtr(__vec) \
    ((__vec) ? &((size_t*)(__vec))[-2] : NULL)


#define __Vec_nextGrowSize(__vec) \
    ((__vec) ? Vec_capacity(__vec) << 1 : 1)

#define __Vec_type(__vec) __typeof__(*(__vec))

#define __Vec_grow(__vec, __count) \
    do { \
        size_t __vec_count = (__count); \
        assert(__vec_count >= Vec_size(__vec)); \
        size_t __vec_size = sizeof(size_t)*2 + __vec_count*sizeof(*(__vec)); \
        size_t* __vec_ptr; \
        if ((__vec)) { \
            __vec_ptr = __Vec_basePtr(__vec); \
            __vec_ptr = Vec_realloc(__vec_ptr, __vec_size); \
            assert(__vec_ptr); \
        }else{ \
            __vec_ptr = Vec_calloc(1, __vec_size); \
            assert(__vec_ptr); \
        } \
        __vec_ptr[1] = __vec_count; \
        (__vec) = (__Vec_type(__vec)*)(&__vec_ptr[2]); \
    }while(0)


#endif
