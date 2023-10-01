#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
    DEGREE = 10000,
    PERCENT = 100
};

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        return 1;
    }
    char *endptr = NULL;
    double rate = strtod(argv[1], &endptr);
    rate *= DEGREE;
    for (int q = 2; q < argc; ++q) {
        double cur = strtod(argv[q], &endptr);
        rate = round((rate * (PERCENT + cur)) / PERCENT);
    }
    rate /= DEGREE;
    printf("%.4lf\n", rate);
}
