#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    return fd;
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
square_sum(unsigned long long n, int mod)
{
    static unsigned long long sum = 0;
    static unsigned long long last = 0;
    for (; last <= n; last++) {
        sum += ((last % mod) * (last % mod)) % mod;
        sum %= mod;
    }
    return (int) sum;
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

    unsigned long long cur_num = 1;
    unsigned char buf = 0;
    ssize_t rd;
    while ((rd = read(inp_fd, &buf, sizeof(buf)))) {
        if (rd == -1) {
            printf("Error, can't read from file\n");
            exit(1);
        }
        unsigned char mask = 1;
        for (int i = 0; i < CHAR_BIT; ++i) {
            if (buf & mask) {
                int sq_sum = square_sum(cur_num, mod);
                if (write(out_fd, &sq_sum, sizeof(sq_sum)) == -1) {
                    printf("Error, can't write to file\n");
                    exit(1);
                }
            }
            mask <<= 1;
            cur_num++;
        }
    }
    close(inp_fd);
    close(out_fd);
}
