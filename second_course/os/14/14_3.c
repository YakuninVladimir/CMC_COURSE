#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

enum
{
    SYS_BASE = 10,
    MIN_NUM_OF_ARGS = 2,
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

int
main(int argc, char *argv[])
{
    if (argc < MIN_NUM_OF_ARGS) {
        fprintf(stderr, "Error, wring num of \n");
        exit(1);
    }
    int n = strtol_s(argv[1]);
    n = n > (argc - MIN_NUM_OF_ARGS) ? argc - MIN_NUM_OF_ARGS : n;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        FILE *cur_f = fopen(argv[i + MIN_NUM_OF_ARGS], "r");
        char *exec_name = NULL;
        size_t len = 0;
        getline(&exec_name, &len, cur_f);
        exec_name[strlen(exec_name) - 1] = '\0';
        // printf("/%s/", exec_name);
        fclose(cur_f);
        pid_t pid = fork();
        if (!pid) {
            execlp(exec_name, exec_name, NULL);
            exit(1);
        } else if (pid == -1) {
            fprintf(stderr, "Error, can't create new proccess\n");
            exit(1);
        }
    }
    int status;
    for (int i = 0; i < n; i++) {
        // printf(">>>%d\n", ans);
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            ans++;
        }
    }
    for (int i = n; i < argc - MIN_NUM_OF_ARGS; i++) {
        FILE *cur_f = fopen(argv[i + MIN_NUM_OF_ARGS], "r");
        char *exec_name = NULL;
        size_t len = 0;
        getline(&exec_name, &len, cur_f);
        exec_name[strlen(exec_name) - 1] = '\0';
        fclose(cur_f);
        pid_t pid = fork();
        if (!pid) {
            execlp(exec_name, exec_name, NULL);
            exit(1);
        } else if (pid == -1) {
            fprintf(stderr, "Error, can't create new proccess\n");
            exit(1);
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                ans++;
            }
        }
    }
    printf("%d\n", ans);
    return 0;
}

