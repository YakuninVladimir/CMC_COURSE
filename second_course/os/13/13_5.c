#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <wait.h>

enum
{
    NUM_OF_ARGS = 2,
    BASE = 10,
};

void
func(FILE *rfd, FILE *wfd, int num, int max_value)
{
    while (1) {
        int x;
        fscanf(rfd, "%d", &x);
        if (x >= max_value - 1) {
            _exit(0);
        }
        ++x;
        printf("%d %d\n", num, x);
        fflush(stdout);
        fprintf(wfd, "%d\n", x);
    }
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong num of arguments\n");
        return 1;
    }
    char *buf = NULL;
    int maxv = strtol(argv[1], &buf, BASE);
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    FILE *ch1[2];
    FILE *ch2[2];
    ch1[0] = fdopen(p1[0], "r");
    ch1[1] = fdopen(p1[1], "w");
    ch2[0] = fdopen(p2[0], "r");
    ch2[1] = fdopen(p2[1], "w");

    setbuf(ch1[0], NULL);
    setbuf(ch1[1], NULL);
    setbuf(ch2[0], NULL);
    setbuf(ch2[1], NULL);

    fprintf(ch1[1], "0\n");

    int pid1 = fork();
    if (!pid1) {
        fclose(ch1[1]);
        fclose(ch2[0]);
        func(ch1[0], ch2[1], 1, maxv);
        _exit(1);
    }
    int pid2 = fork();
    if (!pid2) {
        fclose(ch1[0]);
        fclose(ch2[1]);
        func(ch2[0], ch1[1], 2, maxv);
        _exit(1);
    }
    fclose(ch1[0]);
    fclose(ch1[1]);
    fclose(ch2[0]);
    fclose(ch2[1]);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
}
