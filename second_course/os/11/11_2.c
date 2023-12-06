#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    int p1, p2;
    if (!(p1 = fork())) {
        if (!(p2 = fork())) {
            printf("3 ");
        } else if (p2 == -1) {
            fprintf(stderr, "Error, can't create process");
            _exit(0);
        } else {
            wait(NULL);
            printf("2 ");
        }
    } else if (p1 == -1) {
        fprintf(stderr, "Error, can't create process");
        _exit(0);
    } else {
        wait(NULL);
        printf("1\n");
    }
}
