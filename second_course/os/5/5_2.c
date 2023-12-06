#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

const char FORMAT[] = "rwxrwxrwx";

enum
{
    SYS_BASE = 8,
    NUM_OF_RIGHTS = sizeof(FORMAT) - 1
};

int
main(int argc, char *argv[])
{

    char res[NUM_OF_RIGHTS + 1] = {0};
    for (int i = 1; i < argc; ++i) {
        errno = 0;
        char *buf = NULL;
        unsigned rights = strtoul(argv[i], &buf, SYS_BASE);
        if (*buf || buf == argv[i] || errno) {
            printf("Error: %s is not a valid number\n", argv[i]);
            exit(1);
        }
        unsigned mask = 1;
        for (int j = NUM_OF_RIGHTS - 1; j >= 0; --j) {
            if (rights & mask) {
                res[j] = FORMAT[j];
            } else {
                res[j] = '-';
            }
            mask <<= 1;
        }
        printf("%s\n", res);
    }
}
