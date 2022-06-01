#include <stdio.h>
#include "container/str.h"

int main(){
    Str s = Str_new();
    Str_copy(s, "abcd");
    puts(s);
    Str_append(s, "efgh");
    puts(s);
}
