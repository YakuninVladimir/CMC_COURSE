#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
} Node;

int
convert_num_to_sys(char num[])
{
    unsigned res = 0;
    for (int i = 0; i < sizeof(res); ++i) {
        unsigned cur = (unsigned char) num[sizeof(res) - i - 1];
        res ^= (cur << (CHAR_BIT * i));
    }
    return (int) res;
}

int
read_int(int fd)
{
    int res = 0;
    char buf[sizeof(res)];
    int pos = 0;
    while (pos != sizeof(res)) {
        ssize_t off = read(fd, buf + pos, sizeof(res) - pos);
        if (off == -1) {
            printf("Error, can't read from file\n");
            exit(1);
        }
        pos += off;
    }
    res = convert_num_to_sys(buf);
    return res;
}

Node
read_node(int fd)
{
    Node new_node;
    new_node.key = read_int(fd);
    new_node.right_idx = read_int(fd);
    new_node.left_idx = read_int(fd);
    return new_node;
}

void
dfs(Node head, int fd)
{
    if (head.left_idx != 0) {
        if (lseek(fd, head.left_idx * sizeof(Node), SEEK_SET) == -1) {
            printf("Error? can't seek throw file\n");
            exit(1);
        }
        Node left_node = read_node(fd);
        dfs(left_node, fd);
    }
    printf("%d\n", head.key);
    if (head.right_idx != 0) {
        if (lseek(fd, head.right_idx * sizeof(Node), SEEK_SET) == -1) {
            printf("Error? can't seek throw file\n");
            exit(1);
        }
        Node right_node = read_node(fd);
        dfs(right_node, fd);
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: Not enough argumens\n");
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY, 0600);
    if (fd < 0) {
        printf("Error: Can't open file\n");
        exit(1);
    }
    struct stat st;
    if (stat(argv[1], &st) == -1) {
        printf("Error, can't get info about file\n");
        exit(1);
    }
    if (st.st_size == 0) {
        return 0;
    }
    Node head = read_node(fd);
    dfs(head, fd);
    close(fd);
}
