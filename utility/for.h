#ifndef _FOR_H_
#define _FOR_H_

#define forrange(var, start, end) \
    for (__typeof__(start) var = (start); var < (end); (var)++)

#define foriter(type, var, container) \
    for (type##_iter(__##type##_type(container)) var = type##_begin(container); \
        var != type##_end(container); \
        type##_iter_next(var))

#endif
