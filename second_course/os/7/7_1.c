#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_SYM_THREE_LEN = 64,
    SYS_BASE = 3,
};

const char ERROR_OUTPUT[] = "18446744073709551616";

int
main(void)
{
    int cur_num = getchar();
    while (cur_num != EOF && isspace(cur_num)) {
        cur_num = getchar();
    }
    long long res = 0;
    bool overflow = false;
    bool printed = false;
    bool read = false;
    while (cur_num != EOF) {
        char cur_ch = cur_num;
        if (isspace(cur_num)) {
            if (!printed) {
                if (overflow) {
                    printf("%s\n", ERROR_OUTPUT);
                } else {
                    printf("%lld\n", res);
                }
                res = 0;
                overflow = false;
                printed = true;
            }
        } else {
            read = true;
            printed = false;
            if (cur_ch == '1') {
                long long temp = 0;
                if (res < 0) {
                    if (__builtin_mul_overflow(res + 1, 3, &temp)) {
                        overflow = true;
                    }
                    if (__builtin_add_overflow(temp, 1, &res)) {
                        overflow = true;
                    }
                    if (__builtin_sub_overflow(res, 3, &temp)) {
                        overflow = true;
                    }
                    res = temp;
                } else {
                    if (__builtin_mul_overflow(res, 3, &temp)) {
                        overflow = true;
                    }
                    if (__builtin_add_overflow(temp, 1, &res)) {
                        overflow = true;
                    }
                }

            } else if (cur_ch == 'a') {
                long long temp = 0;
                if (res > 0) {
                    if (__builtin_mul_overflow(res - 1, 3, &temp)) {
                        overflow = true;
                    }
                    if (__builtin_sub_overflow(temp, 1, &res)) {
                        overflow = true;
                    }
                    if (__builtin_add_overflow(res, 3, &temp)) {
                        overflow = true;
                    }
                    res = temp;
                } else {
                    if (__builtin_mul_overflow(res, 3, &temp)) {
                        overflow = true;
                    }
                    if (__builtin_sub_overflow(temp, 1, &res)) {
                        overflow = true;
                    }
                }
            } else if (cur_ch == '0') {
                long long temp = 0;
                if (__builtin_mul_overflow(res, 3, &temp)) {
                    overflow = true;
                }
                res = temp;
            }
        }
        cur_num = getchar();
    }
    if (cur_num == EOF && !printed && !overflow && read) {
        printf("%lld\n", res);
    }
}
