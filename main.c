#include "container/str.h"
#include "utility/print.h"
#include "container/vec.h"
#include "utility/for.h"

int main(){
    Vec(int) a = Vec_new(int);
    Vec_push_back(a, 10);
    Vec_push_back(a, 20);
    Vec_push_back(a, 30);
    Vec_push_back(a, 40);

    forrange(i, 0, Vec_size(a)){
        println(Vec_at(a, i));
    }

    foriter(Vec, it, a){
        println(*it);
    }
}
