#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void
stat_s(const char *path, struct stat *ptr)
{
    if (stat(path, ptr) == -1) {
        fprintf(stderr, "Error, can't gain file stat\n");
        exit(1);
    }
}

int
comp(const void *ar1, const void *ar2)
{
    const char *s1 = *(char **) ar1;
    const char *s2 = *(char **) ar2;
    struct stat *st1 = calloc(1, sizeof(*st1));
    struct stat *st2 = calloc(1, sizeof(*st2));
    stat_s(s1, st1);
    stat_s(s2, st2);
    int res = 0;
    if (st1->st_ino < st2->st_ino) {
        res = -1;
    } else if (st1->st_ino > st2->st_ino) {
        res = 1;
    } else {
        res = -1 * strcmp(s1, s2);
    }
    free(st1);
    free(st2);
    return res;
}

int
comp_str(const void *s1, const void *s2)
{
    return strcmp(*(char **) s1, *(char **) s2);
}

int
main(int argc, char *argv[])
{
    struct stat *check = calloc(1, sizeof(*check));
    int n = 0;
    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], check) != -1) {
            argv[n] = argv[i];
            n++;
        }
    }
    free(check);
    if (n > 0) {
        qsort(argv, n, sizeof(argv[0]), comp);
        char **fin = calloc(n, sizeof(*fin));
        fin[0] = strdup(argv[0]);
        int j = 1;
        char *prev = argv[0];
        if (n > 1) {
            struct stat *st1 = calloc(1, sizeof(*st1));
            struct stat *st2 = calloc(1, sizeof(*st2));
            for (int i = 1; i < n; i++) {
                stat_s(prev, st1);
                stat_s(argv[i], st2);
                if (st1->st_ino != st2->st_ino) {
                    argv[j] = argv[i];
                    j++;
                }
                prev = argv[i];
            }
            free(st1);
            free(st2);
            qsort(argv, j, sizeof(argv[0]), comp_str);
        }
        for (int q = 0; q < j; q++) {
            printf("%s\n", argv[q]);
        }
    }
}
