#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int
main(void)
{
    int fds[2];
    pipe(fds);
    pid_t pid1 = fork();
    if (!pid1) {
        pid_t pid2 = fork();
        if (!pid2) {
            close(fds[1]);
            int buf;
            long long sum = 0;
            while (read(fds[0], &buf, sizeof(buf)) > 0) {
                sum += buf;
            }
            printf("%lld\n", sum);
            fflush(stdout);
            close(fds[0]);
            _exit(0);

        } else if (pid2 == -1) {
            fprintf(stderr, "Error, can't fork proccess\n");
            _exit(1);
        } else {
            close(fds[0]);
            close(fds[1]);
            int status;
            waitpid(pid2, &status, 0);
            _exit(0);
        }
    } else if (pid1 == -1) {
        fprintf(stderr, "Error, can't fork proccess\n");
        return 1;
    } else {
        close(fds[0]);
        int cur;
        while (scanf("%d", &cur) > 0) {
            write(fds[1], &cur, sizeof(cur));
        }
        close(fds[1]);
        int status;
        waitpid(pid1, &status, 0);
        return 0;
    }
}

