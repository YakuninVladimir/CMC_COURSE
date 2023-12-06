#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const char temp[] = ".exe";

enum
{
    NUM_OF_ARGS = 2,
    SUF_LEN = sizeof(temp) - 1,
};

bool
is_exe(const char *path)
{
    bool f = true;
    int n = strlen(path);
    for (int i = 0; i < SUF_LEN; i++) {
        if (path[n - 1 - i] != temp[SUF_LEN - i - 1]) {
            f = false;
            break;
        }
    }
    return f;
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error: incorrect number of args\n");
        exit(1);
    }
    DIR *direct = opendir(argv[1]);
    struct dirent *cur;
    int num = 0;
    cur = readdir(direct);
    while (cur) {
        struct stat st;
        char path_name[PATH_MAX] = {0};
        snprintf(path_name, PATH_MAX, "%s/%s", argv[1], cur->d_name);
        if (path_name[PATH_MAX - 1] != '\0') {
            fprintf(stderr, "Error: path is to long\n");
            exit(1);
        }
        stat(path_name, &st);
        if (S_ISREG(st.st_mode) && !access(path_name, X_OK) && is_exe(path_name)) {
            num++;
        }
        cur = readdir(direct);
    }
    printf("%d\n", num);
    closedir(direct);
}

