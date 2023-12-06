#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

enum
{
    NUM_OF_ARGS = 5,
    SYSCALL_ERR_CODE = 42,
};

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong number of arguments\n");
        return 1;
    }
    pid_t pid = fork();
    if (!pid) {
        int f1 = open(argv[2], O_RDONLY, 0660);
        int f2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        int f3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (f1 == -1 || f2 == -1 || f3 == -1) {
            _exit(SYSCALL_ERR_CODE);
        }
        if (dup2(f1, STDIN_FILENO) == -1) {
            _exit(SYSCALL_ERR_CODE);
        }
        close(f1);
        if (dup2(f2, STDOUT_FILENO) == -1) {
            _exit(SYSCALL_ERR_CODE);
        }
        close(f2);
        if (dup2(f3, STDERR_FILENO) == -1) {
            _exit(SYSCALL_ERR_CODE);
        }
        close(f3);
        execlp(argv[1], argv[1], NULL);
        _exit(SYSCALL_ERR_CODE);
    } else if (pid == -1) {
        fprintf(stderr, "Error, wrong number of arguments\n");
        return SYSCALL_ERR_CODE;
    } else {
        int status = 0;
        wait(&status);
        printf("%d\n", status);
        return 0;
    }
}
