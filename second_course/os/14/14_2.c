#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int mode = 0;

enum
{
    ADD = 0,
    MUL = 1,
};

void
handler(int s)
{
    if (s == SIGQUIT) {
        mode = MUL;
    }
    if (s == SIGINT) {
        mode = ADD;
    }
}

int
main(int argc, char *argv[])
{
    sigaction(SIGINT, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGQUIT, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int cur = 0;
    int res = 0;
    while (scanf("%d", &cur) != EOF) {
        if (mode == ADD) {
            long long buf = 1ll * res + cur;
            res = buf;
        } else if (mode == MUL) {
            long long buf = 1ll * res * cur;
            res = buf;
        }
        printf("%d\n", res);
        fflush(stdout);
    }
}
