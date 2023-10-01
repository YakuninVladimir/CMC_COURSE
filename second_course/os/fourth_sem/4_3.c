#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: Not enough argumens\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDWR, 0600);
    long long cur = 0, minim = LLONG_MAX;
    bool used = false;
    off_t delta = 0;
    while (read(fd, &cur, sizeof(cur))) {
        if (cur < minim || !used) {
            used = true;
            minim = cur;
            delta = lseek(fd, 0, SEEK_CUR);
        }
    }
    if (delta) {
        lseek(fd, delta - sizeof(minim), SEEK_SET);
        minim = -(unsigned long long) minim;
        write(fd, &minim, sizeof(minim));
    }
    close(fd);
}

