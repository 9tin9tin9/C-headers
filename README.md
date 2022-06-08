# C-headers
Some C headers that may come in handy

## Containers:
Implemented mostly with function-like macros to ensure type safety.
Tuple is obviously not type safe though.

### Vec:
```C
Vec(int) emptyVec = Vec_new(int);
Vec(int) initializedVec = Vec_new(int, 1, 3, 4, 5, 7);
Vec_push_back(initializedVec, 10);

for (Vec_iter(int) iter = Vec_begin(initializedVec);
    iter != Vec_end(initializedVec);
    iter++)
{
    printf("%d, ", *iter);
}
putc('\n', stdout);
// 1, 3, 4, 5, 7, 10, 

Vec_pop_back(initializedVec);
Vec_pop_back(initializedVec);
printf("%d", initializedVec[Vec_size(initializedVec)-1]);  // 5

Vec_del(initializedVec);
```

### Deque
```C
Deque(long) emptyDeque = Deque_new(long);
Deque(long) initializedDeque = Deque_new(long, 1, 2, 3);
Deque_push_back(initializedDeque, 4);
Deque_push_front(initializedDeque, 0);

for (Deque_iter(long) iter = Deque_begin(initializedDeque);
    iter != Deque_end(initializedDeque);
    Deque_iter_next(iter, initializedDeque))
{
    printf("%ld, ", *iter);
}
putc('\n', stdout);

Deque_pop_front(initializedDeque);
Deque_pop_front(initializedDeque);
printf("%ld\n", Deque_at(initializedDeque, 0));  // 2

Deque_del(initializedDeque);
```

### Str
```C
Str emptyString = Str_new();

Str str = Str_from("abcd");
Str_append(str, "efgh");
puts(str);   // abcdefgh

Str_del(str);
```

### Hashmap
```C
HMap(short) hmap = HMap_new(short);
HMap_insert(hmap, "first", (short)1);
HMap_insert(hmap, "second", (short)2);
HMap_insert(hmap, "third", (short)3);

*HMap_at(hmap, "third") = 20;
printf("%d", *HMap_at(hmap, "third"));  // 20

HMap_remove(hmap, "second");
printf("%p\n", HMap_at(hmap, "second"));  // 0x0

HMap_del(hmap);
```

### Forward list
```C
FList(double) flist = FList_new(double);
FList_push_front(flist, 3.14);
FList_push_front(flist, 2.718);
FList_push_front(flist, 9.81);
FList_pop_front(flist);

for(FList_iter(double) it = FList_begin(flist);
    it != FList_end(flist);
    FList_iter_next(it))
{
    printf("%f, ", *it);  // 2.718000, 3.140000,
}

FList_del(flist);
```
