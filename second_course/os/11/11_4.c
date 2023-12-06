#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    int n;
    setbuf(stdout, 0);
    setbuf(stdin, 0);
    scanf("%d", &n);
    int i = 1;
    while (i <= n) {
        pid_t p;
        printf("%d", i);
        if (i < n) {
            printf(" ");
        }
        if (i < n) {
            if (!(p = fork())) {
                i++;
            } else if (p == -1) {
                fprintf(stderr, "Error, can't create new process\n");
                _exit(0);
            } else {
                waitpid(p, NULL, 0);
                _exit(0);
            }
        } else {
            printf("\n");
            _exit(0);
        }
    }
}
