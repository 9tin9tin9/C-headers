#include <stdio.h>
#include "container/hashmap.h"

int main(){
    HMap(int) a = HMap_new(int);

    HMap_insert(a, "fit", 1000);
    HMap_insert(a, "sond", 2000);
    HMap_insert(a, "trd", 3000);
    HMap_insert(a, "darth", 4000);
    HMap_insert(a, "fth", 5000);

    for (HMap_iter(int) it = HMap_begin(a);
        it != HMap_end(a);
        HMap_iter_next(it, a))
    {
        printf("%d\n", HMap_entry_val(*it));
    }

    HMap_del(a);

}
