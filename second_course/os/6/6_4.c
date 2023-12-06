#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    NUM_OF_ARGS = 3,
    SYS_BASE = 10,
    MAX_ITER = 4,
};

const char cur_d[] = ".";
const char prev_d[] = "..";

int
strtol_safe(char *src)
{
    char *buf = NULL;
    errno = 0;
    int res = strtol(src, &buf, SYS_BASE);
    if (errno || *buf || buf == src) {
        fprintf(stderr, "Error, invalid size of file");
        exit(1);
    }
    return res;
}

void
recursive_dir_seek(const char *path, int f_size, int iter, const int abs_path_len)
{
    if (iter > MAX_ITER) {
        return;
    }

    struct stat st;
    DIR *cur_dir = opendir(path);
    struct dirent *cur_elem = readdir(cur_dir);
    while (cur_elem) {
        if (strcmp(cur_elem->d_name, cur_d) && strcmp(cur_elem->d_name, prev_d)) {
            char full_path[PATH_MAX] = {0};
            snprintf(full_path, PATH_MAX, "%s/%s", path, cur_elem->d_name);
            if (lstat(full_path, &st) != -1 && access(full_path, R_OK) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    recursive_dir_seek(full_path, f_size, iter + 1, abs_path_len);
                } else if (st.st_size <= f_size && S_ISREG(st.st_mode)) {
                    const char *rel_path = full_path + abs_path_len;
                    printf("%s\n", rel_path);
                }
            }
        }
        cur_elem = readdir(cur_dir);
    }
    closedir(cur_dir);
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong number of arguments of command line\n");
        exit(1);
    }
    int Z = strtol_safe(argv[2]);
    recursive_dir_seek(argv[1], Z, 1, strlen(argv[1]) + 1);
}
