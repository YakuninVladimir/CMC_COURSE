#include <stdio.h>
#include <stdlib.h>

typedef struct s1
{
    char f1;
    long long f2;
    char f3;
} s1;

typedef struct s2
{
    long long f2;
    char f1;
    char f3;
} s2;

size_t
compactify(void *ptr, size_t size)
{
    s1 cur;
    s2 new_cur;
    s2 *new = ptr;
    s1 *old = ptr;
    for (int i = 0; i < size / sizeof(cur); i++) {
        cur = *old;
        new_cur.f1 = cur.f1;
        new_cur.f2 = cur.f2;
        new_cur.f3 = cur.f3;
        *new = new_cur;
        new ++;
        old++;
    }
    return size * sizeof(s2) / sizeof(s1);
}

