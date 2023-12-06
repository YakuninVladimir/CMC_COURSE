#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

enum
{
    KIB = 1024
};

int
main(int argc, char *argv[])
{
    unsigned long long sum = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat buf;
        if (lstat(argv[i], &buf) >= 0 && buf.st_nlink == 1 && buf.st_size % KIB == 0 && S_ISREG(buf.st_mode)) {
            sum += buf.st_size;
        }
    }
    printf("%llu\n", sum);
}
