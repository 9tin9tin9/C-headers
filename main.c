#include <stdio.h>
#include "container/hashmap.h"
#include "container/vec.h"
#include "utility/print.h"
#include "utility/for.h"

int main(){
    HMap(int) a = HMap_new(int);
    HMap_insert(a, "first", 10);
    HMap_at(a, "first") = 20;
    HMap_insert(a, "first", 1);
    HMap_insert(a, "second", 2);
    HMap_insert(a, "third", 3);

    foreach_iter(HMap, a, entry){
        printsp(**entry);
    }

    Vec(int) b = Vec_new(int, 1, 2, 3, 4);
    foreach_iter(Vec, b, it){
        printsp(*it);
    }
}
