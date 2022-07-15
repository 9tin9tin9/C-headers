#ifndef _OPTIONAL_H_
#define _OPTIONAL_H_

#define Optional(T) T*

#define Optional_wrap(v) ((__typeof__(v)[]){v})

#define Optional_value(v) (assert(v), *v)
#define None NULL

#endif
