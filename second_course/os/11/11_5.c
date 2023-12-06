#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    int n;
    pid_t p;
    int i = 0;
    while (scanf("%d", &n) != EOF) {
        if (!(p = fork())) {
            i = 1;
            continue;
        } else if (p == -1) {
            _exit(1);
        } else {
            int status = 0;
            waitpid(p, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 1) {
                printf("%d\n", n);
                _exit(0);
            } else {
                if (i == 0) {
                    printf("-1\n");
                    return 0;
                } else {
                    _exit(1);
                }
            }
        }
    }
}
