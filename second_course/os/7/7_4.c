#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    NUM_OF_ARGS = 2,
};

void
lseek_s(int fd, off_t off, int whence)
{
    errno = 0;
    if (lseek(fd, off, whence) == -1) {
        fprintf(stderr, "Error: can't seek through file: %s\n", strerror(errno));
        exit(1);
    }
}

void
stat_s(const char *path, struct stat *st)
{
    errno = 0;
    if (stat(path, st) == -1) {
        fprintf(stderr, "Error: can't get stat of file: %s\n", strerror(errno));
        exit(1);
    }
}

void
read_s(int fd, char *buf)
{
    errno = 0;
    if (read(fd, buf, sizeof(*buf)) == -1) {
        fprintf(stderr, "Error, can't read from file: %s\n", strerror(errno));
        exit(1);
    }
}

void
write_s(int fd, char *buf)
{
    errno = 0;
    if (write(fd, buf, sizeof(*buf)) == -1) {
        fprintf(stderr, "Error, can't write into file: %s\n", strerror(errno));
        exit(1);
    }
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong number of arguments\n");
        exit(1);
    }
    struct stat st;
    stat_s(argv[1], &st);
    long long int n = st.st_size;
    int fd = open(argv[1], O_RDWR, 0666);
    if (fd == -1) {
        fprintf(stderr, "Error, can't open file: %s\n", strerror(errno));
        exit(1);
    }
    int x;
    int del, mode;
    while (scanf("%d", &x) != -1) {
        if (x > 0 && x <= n * CHAR_BIT) {
            x--;
            del = x / CHAR_BIT;
            mode = x % CHAR_BIT;
            lseek_s(fd, del, SEEK_SET);
            char buf;
            read_s(fd, &buf);
            unsigned char mask = 1 << mode;
            buf = buf | mask;
            lseek_s(fd, del, SEEK_SET);
            write_s(fd, &buf);

        } else if (x < 0 && -x <= n * CHAR_BIT) {
            x = -x;
            x--;
            del = x / CHAR_BIT;
            mode = x % CHAR_BIT;
            lseek_s(fd, del, SEEK_SET);
            char buf;
            read_s(fd, &buf);
            unsigned char mask = ~(0) ^ (1 << mode);
            buf = buf & mask;
            lseek_s(fd, del, SEEK_SET);
            write_s(fd, &buf);
        }
    }
    if (close(fd) == -1) {
        fprintf(stderr, "Error, can't close file: %s\n", strerror(errno));
        exit(1);
    }
}
