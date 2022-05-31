#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "forward_list.h"
#include <assert.h>

// HMap: type** / size_t*
// (size_t)size, (size_t)capacity, (size_t(*)(unsigned char))hashFunc,
// (FList(char**))buckets...
// ^ user pointer
//
// Entry: type* / char**
// (char*)key, (type) data
//             ^ user pointer
//
// Keys are char* only

_Static_assert(
        sizeof(size_t) == sizeof(void*),
        "This HMap breaks when sizeof(size_t) != sizeof(void*)");

typedef size_t (*HMap_hashfunc)(const char*);

static inline size_t
HMap_sdbm(const char *str){
    size_t hash = 0;
    int c;
    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

#ifndef HMap_calloc
#define HMap_calloc calloc
#endif

#ifndef HMap_free
#define HMap_free free
#endif

#ifndef HMap_realloc
#define HMap_realloc realloc
#endif

#ifndef HMap_hash
#define HMap_hash HMap_sdbm
#endif

#ifndef HASHMAP_INIT_BUCKET_NUMBER
#define HASHMAP_INIT_BUCKET_NUMBER 8
#endif

#define HMap(__type) __type**

#define HMap_iter(__type) HMap_entry(__type)*

#define HMap_citer(__type) const HMap_iter(__type)

#define HMap_new(__type) ((HMap(__type))NULL)

#define HMap_del(__hmap) \
    do { \
        for (size_t __hmap_i = 0; __hmap_i < HMap_capacity(__hmap); __hmap_i++) { \
            FList(char**)* __hmap_bucket = (FList(char**)*)(__hmap) + __hmap_i; \
            for (FList_iter(char**) __hmap_it = FList_begin(*__hmap_bucket); \
                __hmap_it != FList_end(__hmap_it); \
                FList_iter_next(__hmap_it)) \
            { \
                HMap_entry_del(*__hmap_it);\
            } \
            FList_del(*__hmap_bucket); \
        } \
        free(__HMap_basePtr(__hmap)); \
    }while(0)

#define HMap_size(__hmap) \
    ((__hmap) ? __HMap_basePtr(__hmap)[0] : 0)

#define HMap_capacity(__hmap) \
    ((__hmap) ? __HMap_basePtr(__hmap)[1] : 0)

#define HMap_func(__hmap) \
    ((__hmap) ? \
        (HMap_hashfunc)(__HMap_basePtr(__hmap)[2]) : \
        0)

#define HMap_insert(__hmap, __key, ...) \
    do { \
        if (!(__hmap) || __HMap_loadFactor(__hmap) > 0.75) { \
            __HMap_grow((__hmap), __HMap_nextGrowSize(__hmap)); \
        } \
        __HMap_basePtr(__hmap)[0]++; \
        size_t __hmap_index = __HMap_index(__hmap, __key); \
        FList_push_front( \
                ((FList(HMap_entry(__HMap_type(__hmap)))*)__hmap)[__hmap_index], \
                HMap_entry_new((__key), __VA_ARGS__)); \
    }while(0)

#define HMap_remove(__hmap, __key) \
    do { \
        if (!(__hmap)) { \
            break; \
        } \
        size_t __hmap_index = __HMap_index(__hmap, __key); \
        FList_remove_if(__hmap[__hmap_index], \
            !strcmp( \
                HMap_entry_key( \
                    *(FList_iter(HMap_entry(__HMap_type(__hmap))))__flist_it), \
                (__key))); \
    }while(0)

#define HMap_at(__hmap, __key) \
    *(__HMap_type(__hmap)*)__HMap_at((void**)(__hmap), (__key))

#define HMap_begin(__hmap) \
    FList_begin( \
            (FList(HMap_entry(__HMap_type(__hmap)))) \
            __HMap_nextNonEmptyBucket((void**)(__hmap), 0))

#define HMap_end(__hmap) \
    ((HMap_iter(__HMap_type(__hmap)))NULL)

#define HMap_iter_next(__iter, __hmap) \
    ((__iter) = __FList_nextPointer(__iter) == FList_end(__iter) ? \
        FList_begin(__HMap_nextNonEmptyBucket( \
            (void**)(__hmap), \
            __HMap_index(__hmap, HMap_entry_key(*(__iter))) + 1)) : \
        __FList_nextPointer(__iter))

#define HMap_entry(__type) __type*

#define HMap_entry_new(__key, ...) \
    (__typeof__(__VA_ARGS__)*)__HMap_entry_new(\
            (__key), \
            (__typeof__(__VA_ARGS__)[]){(__VA_ARGS__)}, \
            sizeof(__typeof__(__VA_ARGS__)))

#define HMap_entry_del(__entry) \
    do { \
        free(HMap_entry_key(__entry)); \
        free(&HMap_entry_val(__entry)); \
        free(__HMap_entry_basePtr(__entry)); \
    }while(0)

#define HMap_entry_key(__entry) \
    (__HMap_entry_basePtr(__entry)[0])

#define HMap_entry_val(__entry) \
    (*(__typeof__(__entry))(__HMap_entry_basePtr(__entry)[1]))




#define __HMap_basePtr(__hmap) \
    ((__hmap) ? &((size_t*)(__hmap))[-3] : NULL)

#define __HMap_loadFactor(__hmap) \
    ((double)HMap_size(__hmap) / HMap_capacity(__hmap))

static inline void*
__HMap_nextNonEmptyBucket(void** hmap, size_t from){
    size_t* ptr = __HMap_basePtr(hmap);
    size_t capacity = HMap_capacity(hmap);
    for (size_t i = from; i < capacity; i++){
        if (hmap[i]){
            return hmap[i];
        }
    }
    return NULL;
}

#define __HMap_index(__hmap, __key) \
    (HMap_func(__hmap)((__key)) % HMap_capacity(__hmap))

#define __HMap_type(__hmap) __typeof__(**(__hmap))

#define __HMap_nextGrowSize(__hmap) \
    ((__hmap) ? \
        HMap_capacity(__hmap) << 1 : \
        HASHMAP_INIT_BUCKET_NUMBER)

#define __HMap_grow(__hmap, __count) \
    do { \
        size_t __hmap_count = (__count); \
        assert(__hmap_count >= HMap_size(__hmap)); \
        size_t __hmap_size = sizeof(size_t)*3 + __hmap_count*sizeof(FList(char**)); \
        size_t* __hmap_ptr = HMap_calloc(1, __hmap_size); \
        assert(__hmap_ptr); \
        __hmap_ptr[2] = (size_t)HMap_hash; \
        __hmap_ptr[1] = __hmap_count; \
        FList(char**)* __hmap_base = (FList(char**)*)&__hmap_ptr[3]; \
        if ((__hmap)) {\
            for (HMap_iter(__HMap_type(__hmap)) __hmap_it = HMap_begin(__hmap); \
                __hmap_it != HMap_end(__hmap); \
                HMap_iter_next(__hmap_it, __hmap)) \
            { \
                const char* __hmap_key = HMap_entry_key(*__hmap_it); \
                size_t __hmap_index = __HMap_index(__hmap_base, __hmap_key); \
                FList_push_front(__hmap_base[__hmap_index], (char**)*__hmap_it); \
            } \
            for (size_t __hmap_i = 0; __hmap_i < HMap_capacity(__hmap); \
                __hmap_i++) \
            { \
                FList_del((__hmap)[__hmap_i]); \
            } \
            free(__HMap_basePtr(__hmap)); \
        } \
        (__hmap) = (HMap(__HMap_type(__hmap)))__hmap_base; \
    }while(0)

#define __HMap_entry_basePtr(__entry) \
    ((__entry) ? &((char**)(__entry))[-1] : NULL)

static inline void*
__HMap_at(void** hmap, const char* key){
    size_t index = __HMap_index(hmap, key);
    for (FList_iter(char**) it = FList_begin(hmap[index]);
        it != FList_end(hmap[index]);
        FList_iter_next(it))
    {
        if (!strcmp(HMap_entry_key(*it), key)){
            return (*it)[1];
        }
    }
    assert(0);
    return NULL;
}

static inline char**
__HMap_entry_new(const char* key, void* data, size_t size){
    char** ptr = calloc(2, sizeof(void*)); assert(ptr);
    ptr[0] = strdup(key); assert(ptr[0]);
    ptr[1] = calloc(1, size); assert(ptr[1]);
    memcpy(ptr[1], data, size);
    return (char**)ptr + 1;
}

#endif
