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

pid_t cur_pid;
int maxv;
FILE *channel[2];
volatile sig_atomic_t proc;

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
handler(int signal)
{
    int x;
    fscanf(channel[0], "%d", &cur_pid);
    fscanf(channel[0], "%d", &x);
    x++;
    fprintf(channel[1], "%d\n", getpid());
    fprintf(channel[1], "%d\n", x);
    if (x >= maxv) {
        kill(cur_pid, SIGUSR1);
        exit(0);
    }
    fprintf(stdout, "%d %d\n", proc, x);
    kill(cur_pid, SIGUSR1);
}

void
func1()
{
    proc = 1;
    while (1) {
        pause();
    }
    exit(0);
}

void
func2()
{
    proc = 2;
    while (1) {
        pause();
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong num of arguments\n");
        return 1;
    }
    sigaction(SIGUSR1, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    maxv = strtol_s(argv[1]);
    int pip[2];
    pipe(pip);
    channel[0] = fdopen(pip[0], "r");
    channel[1] = fdopen(pip[1], "w");

    setbuf(channel[0], NULL);
    setbuf(channel[1], NULL);
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    int pid1 = fork();
    if (!pid1) {
        func1();
        _exit(1);
    }
    int pid2 = fork();
    if (!pid2) {
        func2();
        _exit(1);
    }

    fprintf(channel[1], "%d\n", pid2);
    fprintf(channel[1], "0\n");
    fclose(channel[0]);
    fclose(channel[1]);

    kill(pid1, SIGUSR1);

    int status;
    waitpid(pid1, &status, 0);
    if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
        fprintf(stderr, "Error, wring status of proccess finishing\n");
        exit(1);
    }
    waitpid(pid2, &status, 0);
    if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
        fprintf(stderr, "Error, wring status of proccess finishing\n");
        exit(1);
    }
    printf("Done\n");
    return 0;
}
