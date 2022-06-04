#ifndef _FOR_H_
#define _FOR_H_

#define foreach_range(var, start, end) \
    for (__typeof__(start) var = (start); var < (end); (var)++)

#define foreach_iter(type, container, var) \
    for (type##_iter(__##type##_type(container)) var = type##_begin(container); \
        var != type##_end(container); \
        type##_iter_next(var))

#endif
