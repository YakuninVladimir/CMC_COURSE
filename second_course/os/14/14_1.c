#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
    MAX_VAL = 5,
};

volatile int flag = 0;

void
handler(int s)
{
    if (flag >= MAX_VAL) {
        _exit(0);
    }
    printf("%d\n", flag++);
    fflush(stdout);
}

int
main(void)
{
    sigaction(SIGHUP, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    printf("%d\n", getgid());
    fflush(stdout);
    while (1) {
        pause();
    }
}

