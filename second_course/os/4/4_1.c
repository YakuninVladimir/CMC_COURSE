#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum
{
    CONV_BUF_SIZE = 4,
    FULL_MASK = 0xff,
    HALF_MASK = 0x0f,
    PENS_CHAR_BIT = 12
};

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: Not enough argumens\n");
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        printf("Error, can't open file\n");
        exit(1);
    }
    unsigned cur = 0;
    while (scanf("%i", &cur) == 1) {
        char buf[CONV_BUF_SIZE] = {0};
        buf[0] = (cur & (HALF_MASK << (PENS_CHAR_BIT + CHAR_BIT))) >> (PENS_CHAR_BIT + CHAR_BIT);
        buf[1] = (cur & (FULL_MASK << PENS_CHAR_BIT)) >> PENS_CHAR_BIT;
        buf[2] = (cur & (HALF_MASK << CHAR_BIT)) >> CHAR_BIT;
        buf[3] = cur & FULL_MASK;
        if (write(fd, buf, CONV_BUF_SIZE) == -1) {
            printf("Error, can't write to file\n");
            exit(1);
        }
    }
    close(fd);
}
