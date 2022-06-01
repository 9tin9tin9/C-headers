#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdio.h>

// print can only accept up to 19 arguments
#define print(...) \
    (__PRINT_MAP(__PRINT_WRAP_PRINTF, __VA_ARGS__) 0)

#define println(...) \
    (__PRINT_MAP(__PRINT_WRAP_PRINTF, __VA_ARGS__) putc('\n', stdout), 0)


#define __print_fmt(x) _Generic((x), \
        unsigned char: "%c", \
        char: "%c", \
        signed char: "%c", \
        \
        _Bool: "%d", \
        short int: "%hd", \
        unsigned short int: "%hu", \
        int: "%d", \
        unsigned int: "%u", \
        \
        long int: "%ld", \
        unsigned long int: "%lu", \
        long long int: "%lld", \
        unsigned long long int: "%llu", \
        \
        float: "%f", \
        double: "%f", \
        long double: "%Lf", \
        \
        char*: "%s", \
        void*: "%p", \
        default: "(Unknown type)")

#define __PRINT_WRAP_PRINTF(x) printf(__print_fmt(x), x),

#define __PRINT_COUNT(...) \
            __PRINT_COUNT_(__VA_ARGS__, __PRINT_NUM())
#define __PRINT_COUNT_(...) \
            __PRINT_DISPLACE(__VA_ARGS__)
#define __PRINT_DISPLACE( \
            _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
            _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N
#define __PRINT_NUM() \
            20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
            10, 9, 8, 7, 6, 5, 4, 3, 2, 1


#define __PRINT_M19(map_, x, ...) map_(x) __PRINT_M18(map_, __VA_ARGS__)
#define __PRINT_M18(map_, x, ...) map_(x) __PRINT_M17(map_, __VA_ARGS__)
#define __PRINT_M17(map_, x, ...) map_(x) __PRINT_M16(map_, __VA_ARGS__)
#define __PRINT_M16(map_, x, ...) map_(x) __PRINT_M15(map_, __VA_ARGS__)
#define __PRINT_M15(map_, x, ...) map_(x) __PRINT_M14(map_, __VA_ARGS__)
#define __PRINT_M14(map_, x, ...) map_(x) __PRINT_M13(map_, __VA_ARGS__)
#define __PRINT_M13(map_, x, ...) map_(x) __PRINT_M12(map_, __VA_ARGS__)
#define __PRINT_M12(map_, x, ...) map_(x) __PRINT_M11(map_, __VA_ARGS__)
#define __PRINT_M11(map_, x, ...) map_(x) __PRINT_M10(map_, __VA_ARGS__)
#define __PRINT_M10(map_, x, ...) map_(x) __PRINT_M9(map_, __VA_ARGS__)

#define __PRINT_M9(map_, x, ...) map_(x) __PRINT_M8(map_, __VA_ARGS__)
#define __PRINT_M8(map_, x, ...) map_(x) __PRINT_M7(map_, __VA_ARGS__)
#define __PRINT_M7(map_, x, ...) map_(x) __PRINT_M6(map_, __VA_ARGS__)
#define __PRINT_M6(map_, x, ...) map_(x) __PRINT_M5(map_, __VA_ARGS__)
#define __PRINT_M5(map_, x, ...) map_(x) __PRINT_M4(map_, __VA_ARGS__)
#define __PRINT_M4(map_, x, ...) map_(x) __PRINT_M3(map_, __VA_ARGS__)
#define __PRINT_M3(map_, x, ...) map_(x) __PRINT_M2(map_, __VA_ARGS__)
#define __PRINT_M2(map_, x, ...) map_(x) __PRINT_M1(map_, __VA_ARGS__)
#define __PRINT_M1(map_, x) map_(x)

#define __PRINT_CONCAT(a, b) a##b

#define __PRINT_MAP_(N, map, ...) \
    __PRINT_CONCAT(__PRINT_M, N)(map, __VA_ARGS__)

#define __PRINT_MAP(map, ...) \
    __PRINT_MAP_(__PRINT_COUNT(__VA_ARGS__), map, __VA_ARGS__)

#endif
