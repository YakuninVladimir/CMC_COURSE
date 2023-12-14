#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

enum
{
    NUM_OF_ARGS = 6,
    ERR_CODE = 0,
};

int
handler(const char *name, int ifd, int ofd, int efd, bool s_flag)
{
    pid_t pid = fork();
    if (!pid) {
        if (ifd) {
            dup2(ifd, STDIN_FILENO);
            close(ifd);
        }
        if (ofd) {
            dup2(ofd, STDOUT_FILENO);
            close(ofd);
        }
        if (efd) {
            dup2(efd, STDERR_FILENO);
            close(efd);
        }
        execlp(name, name, NULL);
        _exit(ERR_CODE);
    } else if (pid == -1) {
        return ERR_CODE;
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        if (ifd) {
            close(ifd);
        }
        if (ofd && s_flag) {
            close(ofd);
        }
        if (efd) {
            close(efd);
        }
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            return !ERR_CODE;
        }
        return ERR_CODE;
    }
}

int
main(int argc, char **argv)
{
    if (argc != NUM_OF_ARGS) {
        exit(1);
    }
    int pipes[2] = {0};
    pipe(pipes);
    int file1_fd = open(argv[4], O_RDONLY);
    int file2_fd = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0600);

    pid_t pid = fork();
    if (!pid) {
        close(pipes[0]);
        close(file2_fd);
        if (handler(argv[1], file1_fd, pipes[1], 0, 0)) {
            close(file1_fd);
            handler(argv[2], 0, pipes[1], 0, 1);
        }
        close(pipes[1]);
    } else if (pid == -1) {
        fprintf(stderr, "Error, can't fork proccess\n");
        exit(1);
    } else {
        close(pipes[1]);
        close(file1_fd);
        handler(argv[3], pipes[0], file2_fd, 0, 1);
        waitpid(pid, NULL, 0);
        close(pipes[0]);
        close(file2_fd);
    }
}
