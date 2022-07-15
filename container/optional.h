#ifndef _OPTIONAL_H_
#define _OPTIONAL_H_

#define Optional(T) T*

#define Optional_wrap(v) ((__typeof__(v)[]){v})
#define Optional_wrap_copy(v) ((__typeof__(v)*)memcpy((__typeof__(v)[1]){}, (__typeof__(v)[]){v}, sizeof(__typeof__(v))))

#define Optional_value(v) (assert(v), *v)
#define None NULL

#endif
