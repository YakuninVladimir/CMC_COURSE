#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    NUM_OF_ARGS = 3,
};

void
print_err(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        print_err("Error, wrong num of args.");
    }
    DIR *inp_d = opendir(argv[1]);
    if (inp_d == NULL) {
        print_err("Error, can't open first directory.");
    }
    DIR *out_d = opendir(argv[2]);
    if (out_d == NULL) {
        print_err("Error, can't open second directory.");
    }
    unsigned long long size = 0;
    struct dirent *cur_inp_file = NULL;
    struct dirent *cur_out_file = NULL;
    struct stat inp_stat = {0};
    struct stat out_stat = {0};
    long start = telldir(out_d);
    char cur_path[PATH_MAX] = {0};
    char cur_out_path[PATH_MAX] = {0};
    while ((cur_inp_file = readdir(inp_d))) {
        snprintf(cur_path, PATH_MAX, "%s/%s", argv[1], cur_inp_file->d_name);
        if (lstat(cur_path, &inp_stat) == -1) {
            print_err("Error: can't get stat of file in first directory\n");
        }
        if (S_ISREG(inp_stat.st_mode) && access(cur_path, W_OK) == 0) {
            seekdir(out_d, start);
            while ((cur_out_file = readdir(out_d))) {
                snprintf(cur_out_path, PATH_MAX, "%s/%s", argv[2], cur_out_file->d_name);
                if (stat(cur_out_path, &out_stat) == -1) {
                    print_err("Error: can't get stat of file in first directory\n");
                }
                if (strcmp(cur_out_file->d_name, cur_inp_file->d_name) == 0 && inp_stat.st_ino == out_stat.st_ino &&
                    inp_stat.st_dev == out_stat.st_dev) {
                    size += inp_stat.st_size;
                }
            }
        }
    }
    if (closedir(inp_d) == -1) {
        print_err("Error: can't close input directory");
    }
    if (closedir(out_d) == -1) {
        print_err("Error: can't close output directory");
    }
    printf("%llu\n", size);
}
