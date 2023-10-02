#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum{
  SYS_BASE = 8,
  NUM_OF_RIGHTS = 9
};

int main (int argc, char *argv[]) {
  const char *FORMAT = "rwxrwxrwx";
  for (int i = 1; i < argc; ++i) {
    errno = 0;
    char *buf = NULL;
    unsigned rights = strtoul(argv[i], &buf, SYS_BASE);
    if (*buf || buf == argv[i] || errno){
      printf("Error: %s is not a valid number\n");
      exit(1);
    }
    unsigned mask = 1;
    char res[NUM_OF_RIGHTS];
    for (int j = NUM_OF_RIGHTS - 1; j >= 0; --j){
      if (rights & mask) {
        res[j] = FORMAT[j];
      } else {
        res[j] = '-';
      }
      mask <<=1;
    }
    printf("%s\n", res);
  }
}
