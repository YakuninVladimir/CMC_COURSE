#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

enum
{
    INT_BASE = 10
};

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Error: not enouth arguments\n");
        exit(1);
    }
    char *endptr = NULL;
    long N = strtol(argv[2], &endptr, INT_BASE);
    if (errno || argv[2] == endptr || *endptr) {
        printf("Error: wrong arguments\n");
        exit(1);
    }
    if (N > 1) {
        int fd = open(argv[1], O_RDWR, 0666);
        if (fd == EOF) {
            printf("Error: cannot open file\n");
            exit(1);
        }
        double prev = 0;
        if (read(fd, &prev, sizeof(prev))) {
            exit(0);
        }
        double cur = 0;
        for (int i = 1; i < N; ++i) {
            if (read(fd, &cur, sizeof(cur))) {
                exit(0);
            }
            lseek(fd, -(off_t) sizeof(cur), SEEK_CUR);
            cur = cur - prev;
            write(fd, &cur, sizeof(cur));
            prev = cur;
        }
        close(fd);
    }
    return 0;
}

