#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

enum
{
    MOON_YEAR = 2021,
    MOON_MONTH = 4,
    MOON_DAY = 26,
    MOON_HOUR = 11,
    MOON_MIN = 14,
    FIRST_YEAR = 1900,
    MIN_SEC = 60,
    HOUR_SEC = MIN_SEC * 60,
    DAY_SEC = HOUR_SEC * 24,
    SINOD_SEC = 29 * DAY_SEC + 12 * HOUR_SEC + 44 * MIN_SEC,
    DAY_BEFORE_YOU_CAME = 256,
    MONDAY = 1,
    WEEK_SEC = DAY_SEC * 7,
    OUTPUT_LEN = 11,
};

const char template[] = "%Y-%m-%d";

int
main(void)
{
    int n;
    scanf("%d", &n);
    struct tm cur_date = {0};
    cur_date.tm_year = n - FIRST_YEAR;
    cur_date.tm_mday = DAY_BEFORE_YOU_CAME;
    errno = 0;
    time_t cur_t = timegm(&cur_date);
    if (cur_t == -1) {
        fprintf(stderr, "Error: %s", strerror(errno));
        exit(1);
    }
    struct tm moon_date = {0};
    moon_date.tm_year = MOON_YEAR - FIRST_YEAR;
    moon_date.tm_mon = MOON_MONTH;
    moon_date.tm_mday = MOON_DAY;
    moon_date.tm_hour = MOON_HOUR;
    moon_date.tm_min = MOON_MIN;
    errno = 0;
    time_t moon_t = timegm(&moon_date);
    if (moon_t == -1) {
        fprintf(stderr, "Error: %s", strerror(errno));
        exit(1);
    }
    time_t delta = (cur_t - moon_t) % SINOD_SEC;
    time_t event_t = cur_t + (delta >= 0 ? SINOD_SEC - delta : -1 * delta) + DAY_SEC;
    struct tm event_time = {0};
    errno = 0;
    gmtime_r(&event_t, &event_time);
    while (event_time.tm_wday != MONDAY) {
        event_t += DAY_SEC;
        if (gmtime_r(&event_t, &event_time) == NULL) {
            fprintf(stderr, "Error: %s", strerror(errno));
            exit(1);
        }
    }
    event_t += WEEK_SEC * 3;
    errno = 0;
    if (gmtime_r(&event_t, &event_time) == NULL) {
        fprintf(stderr, "Error: %s", strerror(errno));
        exit(1);
    }
    char buf[OUTPUT_LEN] = {0};
    strftime(buf, OUTPUT_LEN, template, &event_time);
    printf("%s\n", buf);
}

