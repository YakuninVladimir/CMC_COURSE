#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <wait.h>
#include <signal.h>

enum
{
    NUM_OF_ARGS = 2,
    SYS_BASE = 10,
};

int
strtol_s(char *s)
{
    char *buf = NULL;
    long ans = strtol(s, &buf, SYS_BASE);
    if (*buf || buf == s || ans != (int) ans) {
        fprintf(stderr, "Error, can't convert input to integer\n");
        exit(1);
    }
    return ans;
}

void
func1(FILE *rfd, FILE *wfd, int max_value)
{
    printf("Gay1\n");
    fflush(stdout);
    pause();
    pid_t pid;
    fscanf(rfd, "%d", &pid);
    printf("%d\n", pid);
    kill(pid, SIGUSR1);
    pause();
    while (1) {
        int x;
        fscanf(rfd, "%d", &x);
        if (x >= max_value - 1) {
            _exit(0);
        }
        ++x;
        printf("%d %d\n", 1, x);
        fflush(stdout);
        fprintf(wfd, "%d\n", x);
        kill(pid, SIGUSR1);
        pause();
    }
}

void
func2(FILE *rfd, FILE *wfd, int max_value)
{
    printf("Gay2\n");
    fflush(stdout);
    pause();
    pid_t pid;
    fscanf(rfd, "%d", &pid);
    printf("%d\n", pid);
    kill(pid, SIGUSR1);
    while (1) {
        int x;
        pause();
        fscanf(rfd, "%d", &x);
        kill(pid, SIGUSR1);
        if (x >= max_value - 1) {
            _exit(0);
        }
        ++x;
        printf("%d %d\n", 2, x);
        fflush(stdout);
        fprintf(wfd, "%d\n", x);
        kill(pid, SIGUSR1);
    }
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong num of arguments\n");
        return 1;
    }
    int maxv = strtol_s(argv[1]);
    int pip[2];
    pipe(pip);
    FILE *channel[2];
    channel[0] = fdopen(pip[0], "r");
    channel[1] = fdopen(pip[1], "w");

    setbuf(channel[0], NULL);
    setbuf(channel[1], NULL);


    int pid1 = fork();
    if (!pid1) {
        printf("?\n");
        func1(channel[0], channel[1], maxv);
        _exit(1);
    }
    int pid2 = fork();
    if (!pid2) {
        printf("?\n");
        func2(channel[0], channel[1], maxv);
        _exit(1);
    }
    // pid2 = fork();
    // if (!pid2) {
    //     func2(channel[0], channel[1], maxv);
    //     _exit(1);
    // }

    fprintf(channel[1], "%d\n", pid2);
    fprintf(channel[1], "%d\n", pid1);
    fprintf(channel[1], "0\n");
    fclose(channel[0]);
    fclose(channel[1]);
    kill(pid1, SIGUSR1);
    // kill(pid2, SIGUSR1);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("Done\n");
}

