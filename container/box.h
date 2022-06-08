#ifndef _BOX_H_
#define _BOX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define fatal(str_) \
    do { \
        fprintf(stderr, "%s", str_); \
        abort(); \
    } while(0)

static inline void*
xmalloc(size_t size)
{
  void *value = malloc(size);
  if (value == 0)
      fatal("virtual memory exhausted");
  return value;
}

#define Box(c_) \
    memcpy( \
        xmalloc(sizeof(__typeof__(c_))), \
        (__typeof__(c_)[]){(c_)}, \
        sizeof(__typeof__(c_))) \

#undef fatal

#endif
