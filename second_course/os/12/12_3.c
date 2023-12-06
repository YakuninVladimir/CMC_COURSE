#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const char PAR = 'p';
const char STEP = 's';

int
main(int argc, char *argv[])
{
    int counter = 0;
    for (int q = 1; q < argc; q++) {
        while (q < argc && argv[q][0] == PAR) {
            pid_t pid = fork();
            if (!pid) {
                execlp(argv[q] + 1, argv[q] + 1, NULL);
                _exit(1);
            } else {
                q++;
                continue;
            }
        }
        int stat;
        while (wait(&stat) != -1) {
            if (WIFEXITED(stat) && !WEXITSTATUS(stat)) {
                counter++;
            }
        }
        if (q < argc && argv[q][0] == STEP) {
            pid_t pid = fork();
            if (!pid) {
                execlp(argv[q] + 1, argv[q] + 1, NULL);
                _exit(1);
            } else {
                int stat;
                wait(&stat);
                if (WIFEXITED(stat) && !WEXITSTATUS(stat)) {
                    counter++;
                }
            }
        }
    }
    printf("%d\n", counter);
}
