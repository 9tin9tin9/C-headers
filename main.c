#include <stdlib.h>
#include <string.h>
#include "utility/print.h"
#include "utility/for.h"
#include "container/vec.h"

int main(){
    Vec(int) a = Vec_new(int);
    Vec(int) b = Vec_new(int, -1, -2, -3);
    Vec_insert(a, 0, Vec_begin(b), Vec_end(b), ++);
    Vec_insert(a, 2, b, b+Vec_size(b), Vec_iter_next);
    Vec_del(b);

    foreach_iter(Vec, a, it) {
        printsp(*it);
    }
}
