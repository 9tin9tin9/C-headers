#include "utility/print.h"
#include "container/vec.h"
#include "container/hashmap.h"
#include "container/optional.h"

Optional(int) fun(){
    int a = 10;
    return Optional_wrap(a);
}

int main(int argc, char** argv){
    Optional(int) a = fun();
    Optional(int) b = fun();
    println(Optional_value(a), (char)' ', Optional_value(b));
    return 0;
}
