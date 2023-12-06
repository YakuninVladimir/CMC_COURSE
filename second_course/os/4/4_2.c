#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
        ssize_t rd = read(fd, &prev, sizeof(prev));
        if (rd == 0) {
            exit(0);
        }
        if (rd == -1) {
            printf("Error, can't read from file\n");
            exit(1);
        }
        double cur = 0;
        for (int i = 1; i < N; ++i) {
            rd = read(fd, &cur, sizeof(cur));
            if (rd == 0) {
                exit(0);
            }
            if (rd == -1) {
                printf("Error, can't read from file\n");
                exit(1);
            }
            if (lseek(fd, -(off_t) sizeof(cur), SEEK_CUR) == -1) {
                printf("Error, can't seek throw file\n");
                exit(1);
            }
            cur = cur - prev;
            if (write(fd, &cur, sizeof(cur)) == -1) {
                printf("Error, can't write to file\n");
                exit(1);
            }
            prev = cur;
        }
        close(fd);
    }
    return 0;
}
