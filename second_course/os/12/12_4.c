#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    INT_BUF = 11,
};

const char rand_dir[] = "/dev/urandom";
const char pystart[] = "from os import remove\nfrom sys import argv\nprint(";
const char pyend[] = "1)\nremove(argv[0])\n";
const char intername[] = "#! /bin/python\n";

int
main(int argc, char *argv[])
{
    int rand_fd = open(rand_dir, O_RDONLY);
    char rand_name[INT_BUF] = {0};
    unsigned rand_int;
    read(rand_fd, &rand_int, sizeof(rand_int));
    snprintf(rand_name, INT_BUF, "%u", rand_int);

    int ofd = open(rand_name, O_WRONLY | O_CREAT | O_TRUNC, 0700);
    write(ofd, intername, sizeof(intername) - 1);
    int n = sizeof(pystart) + sizeof(pyend) - 2;
    char *printstr = calloc(n, sizeof(*printstr));
    int i = 0;
    for (; i < sizeof(pystart) - 1; i++) {
        printstr[i] = pystart[i];
    }
    for (int q = 1; q < argc; q++) {
        int len = strlen(argv[q]);
        n += len + 1;
        printstr = realloc(printstr, n);
        for (int j = 0; j < len; j++) {
            printstr[i] = argv[q][j];
            i++;
        }
        printstr[i] = '*';
        i++;
    }
    for (int j = 0; j < sizeof(pyend); j++) {
        printstr[i + j] = pyend[j];
    }
    printstr[i + sizeof(pyend)] = '\0';
    if (write(ofd, printstr, n) == -1) {
        fprintf(stderr, "Error, can't write to file\n");
        exit(1);
    }
    free(printstr);
    close(ofd);
    execl(rand_name, rand_name, NULL);
    _exit(1);
}
