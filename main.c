#include <stdio.h>
#include "container/hashmap.h"

int main(int argc, char** argv){
HMap(short) hmap = HMap_new(short);
HMap_insert(hmap, "first", (short)1);
HMap_insert(hmap, "second", (short)2);
HMap_insert(hmap, "third", (short)3);

*HMap_at(hmap, "third") = 20;
printf("%d\n", *HMap_at(hmap, "third"));  // 20

HMap_remove(hmap, "second");
printf("%p\n", HMap_at(hmap, "second"));  // 0x0

HMap_del(hmap);
}
