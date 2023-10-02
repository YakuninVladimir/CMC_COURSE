#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

enum {
  BIT_LEN = 12
};


typedef struct Node {
  int32_t key;
  int32_t left_idx;
  int32_t right_idx;
} Node;

Node read_node (int fd) {
  Node new_node;
  int pos = 0;
  while (pos != 12){
    pos += read(fd, &new_node + pos, BIT_LEN - pos);
  }
  return new_node;
}

void dfs (Node head, int fd) {
  printf("<%d %d %d>\n", head.key, head.left_idx, head.right_idx);
  if (head.left_idx != 0){
    lseek(fd, head.left_idx, SEEK_SET);
    Node left_node = read_node(fd);
    dfs(left_node, fd);
  }
  printf("%d\n", head.key);
  if (head.right_idx != 0){
    lseek(fd, head.right_idx, SEEK_SET);
    Node right_node = read_node(fd);
    dfs(right_node, fd);
  }
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
     printf("Error: Not enough argumens\n");
     exit(1);
  }
  int fd = open(argv[1], O_RDONLY, 0600);
  if (fd < 0){
    printf("Error: Can't open file\n");
    exit(0);
  }
  Node head = read_node(fd);
  dfs(head, fd);
}
