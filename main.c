#include "utility/print.h"
#include "container/vec.h"
#include "container/hashmap.h"
#include "container/optional.h"

int main(int argc, char** argv){
    Optional(int) a = Optional_wrap(10);
    Optional(int) b = Optional_wrap(20);
    Optional(int) c = a;

    printsp(Optional_value(c), Optional_value(a));
    return 0;
}
