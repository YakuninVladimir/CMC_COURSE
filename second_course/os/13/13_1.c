#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

enum
{
    FIRST_M = 1,
    FIRST_Y = 1900,
};

int
main(int argc, char *argv[])
{
    int fds[2];
    pipe(fds);
    pid_t p1 = fork();
    if (!p1) {
        pid_t p2 = fork();
        if (!p2) {
            pid_t p3 = fork();
            if (!p3) {
                time_t t = time(0);
                struct tm time_data;
                localtime_r(&t, &time_data);
                write(fds[1], &time_data.tm_mday, sizeof(time_data.tm_mday));
                write(fds[1], &time_data.tm_mon, sizeof(time_data.tm_mon));
                write(fds[1], &time_data.tm_year, sizeof(time_data.tm_year));
                close(fds[1]);
                close(fds[0]);
                _exit(0);

            } else if (p3 == -1) {
                fprintf(stderr, "Error, can't fork proccess\n");
                _exit(1);
            } else {
                waitpid(p3, NULL, 0);
                close(fds[1]);
                struct tm channel_data;
                read(fds[0], &channel_data.tm_mday, sizeof(channel_data.tm_mday));
                printf("D:%d\n", channel_data.tm_mday);
                fflush(stdout);
                close(fds[0]);
                _exit(0);
            }
        } else if (p2 == -1) {
            fprintf(stderr, "Error, can't fork proccess\n");
            _exit(1);
        } else {
            waitpid(p2, NULL, 0);
            close(fds[1]);
            struct tm channel_data;
            read(fds[0], &channel_data.tm_mon, sizeof(channel_data.tm_mon));
            printf("M:%d\n", channel_data.tm_mon + FIRST_M);
            fflush(stdout);
            close(fds[0]);
            _exit(0);
        }
    } else if (p1 == -1) {
        fprintf(stderr, "Error, can't fork proccess\n");
        _exit(1);
    } else {
        waitpid(p1, NULL, 0);
        struct tm channel_data;
        read(fds[0], &channel_data.tm_year, sizeof(channel_data.tm_year));
        printf("Y:%d\n", channel_data.tm_year + FIRST_Y);
        fflush(stdout);
        close(fds[0]);
        close(fds[1]);
        return 0;
    }
}
