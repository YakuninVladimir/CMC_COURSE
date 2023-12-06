#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

enum
{
    FIRST_YEAR = 1900,
    DAY_SEC = 86400,
    THURSDAY = 4,
    MOD = 3,
    DATE_1 = 2,
    DATE_2 = 4,
    WEEK_SEC = DAY_SEC * 7
};

void
gmtime_r_safe(const time_t *t, struct tm *dst)
{
    if (gmtime_r(t, dst) == NULL) {
        fprintf(stderr, "Error: time conversion failed due to following reason: %s\n", strerror(errno));
        exit(1);
    }
}

time_t
timegm_safe(struct tm *src)
{
    time_t res = timegm(src);
    if (res == -1) {
        fprintf(stderr, "Error: time conversion failed due to following reason: %s\n", strerror(errno));
        exit(1);
    }
    return res;
}

int
main(void)
{
    int n;
    scanf("%d", &n);
    struct tm year = {0};
    year.tm_year = n - FIRST_YEAR;
    time_t start = timegm_safe(&year);
    int curm = 0, curf = 0;
    gmtime_r_safe(&start, &year);
    while (year.tm_wday != THURSDAY) {
        start += DAY_SEC;
        gmtime_r_safe(&start, &year);
    }
    do {
        if (year.tm_mon != curm) {
            curm = year.tm_mon;
            curf = 0;
        }
        curf++;
        if ((curf == DATE_1 || curf == DATE_2) && year.tm_mday % 3 != 0 && year.tm_wday == THURSDAY) {
            printf("%d %d\n", year.tm_mon + 1, year.tm_mday);
        }
        start += WEEK_SEC;
        gmtime_r_safe(&start, &year);
    } while (year.tm_year == n - FIRST_YEAR);
}
