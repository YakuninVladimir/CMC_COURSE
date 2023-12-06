#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    NUM_OF_ARGS = 4,
};

int
execute(const char *name)
{
    pid_t pid = fork();
    if (!pid) {
        execlp(name, name, NULL);
        _exit(1);
    } else if (pid == -1) {
        fprintf(stderr, "Error, can't create new proccess\n");
        return 1;
    } else {
        int stat;
        waitpid(pid, &stat, 0);
        return (WIFEXITED(stat) && !WEXITSTATUS(stat));
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong num of args\n");
        return 1;
    }
    return !((execute(argv[1]) || execute(argv[2])) && execute(argv[3]));
}
