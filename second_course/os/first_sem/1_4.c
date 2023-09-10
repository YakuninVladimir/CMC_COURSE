#include<stdio.h>
#include<stdlib.h>

int
comp(const void *val1, const void *val2)
{
    int a = *(int*)val1;
    int b = *(int*)val2;
    if (a % 2 == 0 && b % 2 != 0){
        return -1;
    } else if (a % 2 != 0 && b % 2 == 0){
        return 1;
    } else if (a % 2 != 0 && b % 2 != 0){
        if (b > a){
            return 1;
        } else {
            return -1;
        }
    } else {
        if (a > b){
            return 1;
        } else {
            return -1;
        }
    }
}

void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, 4,comp);
}
