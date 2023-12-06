#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum
{
    INT_BASE = 10
};

int
main(int argc, char *argv[])
{
    long long int sum_pos = 0;
    long long int sum_neg = 0;
    for (int q = 1; q < argc; ++q) {
        char *endptr = NULL;
        errno = 0;
        long int cur = strtol(argv[q], &endptr, INT_BASE);
        if (errno || *endptr || argv[q] == endptr) {
            printf("programm failed: cannot convert input into integer");
            exit(1);
        } else {
            if (cur > 0) {
                sum_pos += cur;
            } else {
                sum_neg += cur;
            }
        }
    }
    printf("%lld\n%lld\n", sum_pos, sum_neg);
}
