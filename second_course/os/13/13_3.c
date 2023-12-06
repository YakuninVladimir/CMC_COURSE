#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

enum {
    NUM_OF_ARGS = 6,
    ERR_CODE = 0,
};

int handler (const char *name, int ifd, int ofd, int efd){
    printf("%s\n", name);
    if (ifd){
        dup2(ifd, STDIN_FILENO);
        close(ifd);
    }
    if (ofd){
        dup2(ofd, STDOUT_FILENO);
        close(ofd);
    }
    if (efd){
        dup2(efd, STDERR_FILENO);
        close(efd);
    }
    pid_t pid = fork();
    if (!pid){
        execlp(name, name, NULL);
        _exit(ERR_CODE);
    } else if (pid == -1){
        return ERR_CODE;
    } else {
        int status = 0;
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)){
            return !ERR_CODE;            
        }
        return ERR_CODE;
    }
}
// ( cmd1 < file1 && cmd2 ) | cmd3 >> file2

int main(int argc, char **argv) {
    if (argc != NUM_OF_ARGS){
        exit(1);
    } 
    int pipes[2] = {0};
    pipe(pipes);
    int file1_fd = open(argv[4], O_RDONLY);
    int file2_fd = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0777);
    return (handler(argv[1], file1_fd, pipes[1], 0) && handler(argv[2], 0, pipes[1], 0)) 
            && handler(argv[3], pipes[0], file2_fd, 0);
}



