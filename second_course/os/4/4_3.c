#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: Not enough argumens\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDWR, 0600);
    if (fd == -1) {
        printf("Error: can't open file\n");
        exit(1);
    }
    long long cur = 0, minim = LLONG_MAX;
    bool used = false;
    off_t delta = 0;
    ssize_t status;
    while ((status = read(fd, &cur, sizeof(cur)))) {
        if (status == -1) {
            printf("Error, can't read from file\n");
            exit(1);
        }
        if (cur < minim || !used) {
            used = true;
            minim = cur;
            delta = lseek(fd, 0, SEEK_CUR);
            if (delta == -1) {
                printf("Error, can't seek in file\n");
                exit(1);
            }
        }
    }
    if (delta) {
        if (lseek(fd, delta - sizeof(minim), SEEK_SET) == -1) {
            printf("Error, can't seek in file\n");
            exit(1);
        }
        minim = -(unsigned long long) minim;
        if (write(fd, &minim, sizeof(minim)) == -1) {
            printf("Error, can't write in file\n");
            exit(1);
        }
    }
    close(fd);
}
