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

const char prev[] = "..";
const char cur[] = ".";

void
finish_process(int sourse_des, DIR *sourse_dir)
{
    fchdir(sourse_des);
    close(sourse_des);
    closedir(sourse_dir);
    exit(-1);
}

void
fstat_s(int filedes, struct stat *st, int sourse_des, DIR *sourse_dir)
{
    if (fstat(filedes, st) == -1) {
        finish_process(sourse_des, sourse_dir);
    }
}

void
fchdir_s(int filedes, int sourse_des, DIR *sourse_dir)
{
    if (fchdir(filedes) == -1) {
        finish_process(sourse_des, sourse_dir);
    }
}

void
chdir_s(const char *path, int sourse_des, DIR *sourse_dir)
{
    if (chdir(path) == -1) {
        finish_process(sourse_des, sourse_dir);
    }
}

void
close_s(int filedes, int sourse_des, DIR *sourse_dir)
{
    if (close(filedes) == -1) {
        finish_process(sourse_des, sourse_dir);
    }
}

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    ino_t inodes[PATH_MAX + 1] = {0};

    DIR *src_dir = opendir(cur);
    int src_fd = dirfd(src_dir);
    fchdir_s(fd, src_fd, src_dir);

    struct stat cur_st;
    fstat_s(fd, &cur_st, src_fd, src_dir);
    int node_i = 0;
    int cur_fd = fd;
    do {
        inodes[node_i] = cur_st.st_ino;
        node_i++;
        DIR *new_dir = opendir(prev);
        cur_fd = dirfd(new_dir);
        fchdir_s(cur_fd, src_fd, src_dir);
        fstat_s(cur_fd, &cur_st, src_fd, src_dir);
        close_s(cur_fd, src_fd, src_dir);
        closedir(new_dir);
    } while (inodes[node_i - 1] != cur_st.st_ino);
    int buf_ind = 0, cou = 0;
    struct dirent *cur_elem = NULL;
    for (int i = node_i - 2; i >= 0; i--) {
        DIR *rec_dir = opendir(cur);
        while ((cur_elem = readdir(rec_dir))) {
            if (cur_elem->d_ino == inodes[i]) {
                if (buf_ind < size) {
                    buf[buf_ind] = '/';
                    buf_ind++;
                }
                cou++;
                for (int j = 0; cur_elem->d_name[j] != '\0'; j++) {
                    if (buf_ind < size) {
                        buf[buf_ind] = cur_elem->d_name[j];
                        buf_ind++;
                    }
                    cou++;
                }
                chdir_s(cur_elem->d_name, src_fd, src_dir);
                break;
            }
        }

        closedir(rec_dir);
    }
    int output_len = buf_ind < size - 1 ? buf_ind : size - 1;
    buf[output_len] = '\0';
    if (cou == 0) {
        buf[0] = '/';
        buf[1] = '\0';
        cou++;
    }
    fchdir_s(src_fd, src_fd, src_dir);
    close_s(src_fd, src_fd, src_dir);
    closedir(src_dir);
    return cou;
}

