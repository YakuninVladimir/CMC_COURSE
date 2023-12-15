#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

enum
{
    NUM_OF_ARGS = 2,
    MAX_MASK = 128,
};

volatile int num;
FILE *data_file;

char cur_sym = 0;
unsigned bit_mask = 1;

void
usr1_handler(int signal)
{
    if (num == 1) {
        bit_mask <<= 1;
        if (bit_mask >= MAX_MASK) {
            printf("%c", cur_sym);
            bit_mask = 1;
            cur_sym = 0;
        }
    }
}

void
usr2_handler(int signal)
{
    if (num == 1) {
        cur_sym += bit_mask;
        bit_mask <<= 1;
        if (bit_mask >= MAX_MASK) {
            printf("%c", cur_sym);
            bit_mask = 1;
            cur_sym = 0;
        }
    }
}

void
alrm_handler(int signal)
{
    if (num == 2) {

        bit_mask <<= 1;
        if (bit_mask >= MAX_MASK) {
        }
    }
}

void
io_handler(int signal)
{
    if (num == 1) {
        exit(0);
    }
}

void
sender(const char *filename)
{
    data_file = fopen(filename, "r");
    if (data_file == NULL) {
        fprintf(stderr, "Error, can't open file\n");
        exit(1);
    }
    setbuf(data_file, NULL);

    while (1) {
        pause();
    }
}

void
reader()
{
    while (1) {
        pause();
    }
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong num of arguments\n");
        exit(1);
    }

    sigaction(SIGUSR1, &(struct sigaction){.sa_handler = usr1_handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGUSR2, &(struct sigaction){.sa_handler = usr2_handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGALRM, &(struct sigaction){.sa_handler = alrm_handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGIO, &(struct sigaction){.sa_handler = io_handler, .sa_flags = SA_RESTART}, NULL);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    num = 1;
    pid_t pid1 = fork();
    if (!pid1) {
        reader();
    } else if (pid1 == -1) {
        fprintf(stderr, "Error, can't create new proccess\n");
        exit(1);
    }

    num = 2;
    pid_t pid2 = fork();
    if (!pid2) {
        sender(argv[1]);
    } else if (pid2 == -1) {
        fprintf(stderr, "Error, can't create new proccess\n");
        exit(1);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
