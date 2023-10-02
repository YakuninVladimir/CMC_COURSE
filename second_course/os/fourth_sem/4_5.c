#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>

enum
{
    NUM_OF_ARGS = 4,
    INT_BASE = 10
};

int
open_file_s(char *name, int flags)
{
    int fd = open(name, flags, 0600);
    if (fd < 0) {
        printf("Error: can't open file: %s\n", name);
        exit(1);
    }
}

int
str_to_int_s(char *str)
{
    errno = 0;
    char *buf = NULL;
    int num = strtol(str, &buf, INT_BASE);
    if (*buf || errno || buf == str) {
        printf("Error: can't convert %s to integer\n", str);
        exit(1);
    }
    return num;
}

int
square_sum(int n, int mod)
{
    return (n * (n + 1) * (2 * n + 1) / 6) % mod;
}

int
main(int argc, char *argv[])
{
    if (argc < NUM_OF_ARGS) {
        printf("Error: Not enough argumens\n");
        exit(1);
    }
    int inp_fd = open_file_s(argv[1], O_RDONLY);
    int out_fd = open_file_s(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    int mod = str_to_int_s(argv[3]);

    int cur_num = 1;
    unsigned char buf = 0;

    while (read(inp_fd, &buf, sizeof(buf))) {
        unsigned char mask = 1;
        for (int i = 0; i < CHAR_BIT; ++i) {
            if (buf & mask) {
                int sq_sum = square_sum(cur_num, mod);
                write(out_fd, &sq_sum, sizeof(cur_num));
            }
            mask <<= 1;
            cur_num++;
        }
    }
}
