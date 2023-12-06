#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    NUM_OF_CHILDREN = 3,
    STRING_LEN = 9,
    BASE = 10,
};

int
main(void)
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    for (int i = 0; i < NUM_OF_CHILDREN; i++) {
        pid_t p;
        if (!(p = fork())) {
            int val;
            scanf("%d", &val);
            printf("%d %d\n", i + 1, val * val);
            _exit(0);
        } else if (p == -1) {
            fprintf(stderr, "Error, can't create new process\n");
            _exit(0);
        }
    }

    while (wait(NULL) != -1) {
    }
}
