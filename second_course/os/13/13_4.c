#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>

void
genocide(int n, int *pids)
{
    for (int i = 0; i < n; i++) {
        kill(pids[i], SIGKILL);
    }
    for (int i = 0; i < n; i++) {
        waitpid(pids[i], NULL, 0);
    }
    exit(1);
}

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 0;
    } else if (argc == 2) {
        pid_t pid = fork();
        if (!pid) {
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        } else if (pid == -1) {
            exit(1);
        } else {
            waitpid(pid, NULL, 0);
            return 0;
        }
    } else {
        int pip[2] = {0};
        int *pids = calloc(argc - 1, sizeof(*pids));
        pipe(pip);
        pid_t pid = fork();
        if (!pid) {
            dup2(pip[1], STDOUT_FILENO);
            close(pip[1]);
            close(pip[0]);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        } else if (pid == -1) {
            close(pip[0]);
            close(pip[1]);
            exit(1);
        } else {
            pids[0] = pid;
            close(pip[1]);
        }

        for (int i = 0; i < argc - 3; i++) {
            int new_pip[2] = {0};
            pipe(new_pip);
            pid_t pid = fork();
            if (!pid) {
                dup2(new_pip[1], STDOUT_FILENO);
                dup2(pip[0], STDIN_FILENO);
                close(new_pip[0]);
                close(pip[0]);
                close(new_pip[1]);
                execlp(argv[i + 2], argv[i + 2], NULL);
                _exit(1);
            } else if (pid == -1) {
                close(pip[0]);
                close(new_pip[0]);
                close(new_pip[1]);
                genocide(i + 1, pids);
            } else {
                pids[i + 1] = pid;
                if (close(pip[0]) == -1) {
                    genocide(i + 2, pids);
                }
                if (close(new_pip[1]) == -1) {
                    genocide(i + 2, pids);
                }
                pip[0] = new_pip[0];
            }
        }
        pid = fork();
        if (!pid) {
            dup2(pip[0], STDIN_FILENO);
            close(pip[0]);
            execlp(argv[argc - 1], argv[argc - 1], NULL);
            _exit(1);
        } else if (pid == -1) {
            close(pip[0]);
            genocide(argc - 2, pids);
        } else {
            pids[argc - 2] = pid;
            if (close(pip[0]) == -1) {
                genocide(argc - 1, pids);
            }
        }
        for (int i = 0; i < argc - 1; i++) {
            waitpid(pids[i], NULL, 0);
        }
        free(pids);
        return 0;
    }
}
