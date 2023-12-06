#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

const char exec_prog[] = "#include <stdio.h>\n\
            #include <unistd.h>\n\
            const char summon[] = \"summon\";\n\
            const char reject[] = \"reject\";\n\
            const char disqualify[] = \"disqualify\";\n\
            \n\
            int main(void){\n\
                int x = 0;\n\
                while (scanf(\"%%d\", &x) != EOF){\n\
                    printf(\"%%s\\n\", %s);\n\
                }\n\
                unlink(\"%s\");\n\
                return 0;\n\
            }";

const char rand_file[] = "/dev/urandom";
const char *tmps[] = {"XDG_RUNTIME_DIR", "TMPDIR", "/tmp"};

enum
{
    NUM_OF_ARGS = 2,
    MAX_INT_LEN = 11,
    POST_LEN = 2,
};

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong num of args\n");
        exit(1);
    }
    int rand_fd = open(rand_file, O_RDONLY);
    unsigned rand_u = 0;
    read(rand_fd, &rand_u, sizeof(rand_u));
    char *tmp = getenv(tmps[0]);
    if (!tmp) {
        tmp = getenv(tmps[1]);
        if (!tmp) {
            tmp = strdup(tmps[2]);
        }
    }
    char *name = calloc(MAX_INT_LEN + POST_LEN, sizeof(*name));
    snprintf(name, MAX_INT_LEN + POST_LEN, "%u.c", rand_u);

    char *path = calloc(PATH_MAX, sizeof(*path));
    snprintf(path, PATH_MAX, "%s/%s", tmp, name);
    char *exec_path = strdup(path);
    exec_path[strlen(exec_path) - POST_LEN] = '\0';

    FILE *exec_file = fopen(path, "w");
    fprintf(exec_file, exec_prog, argv[1], path);
    fclose(exec_file);
    if (!fork()) {
        execlp("gcc", "gcc", path, "-o", exec_path, NULL);
        _exit(1);
    }
    int status = 0;
    wait(&status);
    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
        fprintf(stderr, "Error: compilation failed\n");
        _exit(1);
    }
    execlp(exec_path, exec_path, NULL);
    exit(1);
}
