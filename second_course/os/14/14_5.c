#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

enum
{
    MAX_NUM_OF_PRIMES = 3,
    MIN_PRIME = 2,
};

volatile int p = 0;
volatile int counter = 0;

void
handler(int s)
{
    if (s == SIGTERM) {
        exit(0);
    }
    if (s == SIGINT) {
        if (counter >= MAX_NUM_OF_PRIMES) {
            exit(0);
        } else {
            printf("%d\n", p);
            fflush(stdout);
            counter++;
        }
    }
}

bool
is_prime(int n)
{
    for (int i = MIN_PRIME; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int
main(void)
{
    sigaction(SIGTERM, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGINT, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    int low, high;
    scanf("%d %d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    int st_val = MIN_PRIME > low ? MIN_PRIME : low;
    for (int i = st_val; i < high; i++) {
        if (is_prime(i)) {
            p = i;
        }
    }
    printf("-1\n");
    fflush(stdout);
    exit(0);
}
