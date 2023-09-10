#include<stdio.h>
#include<stdlib.h>

int
main(void)
{
    int n;
    if (scanf("%d", &n) != EOF) {
        int *cur_line = calloc(n - 1, sizeof(int));
        int *add = calloc(n - 1, sizeof(int));
        for (int a = 1; a < n; a++) {
            for (int b = 0; b < n; b++) {
                if (a * b % n == 1){
                    add[a - 1] = b;
                    break;
                }
            }
        }
        for (int c = 0; c < n; c++) {
            for (int a = 1; a < n; a ++) {
                printf("%d ", cur_line[a - 1]);
                cur_line[a - 1] = (cur_line[a - 1] + add[a - 1]) % n;
            }
            printf("\n");
        }
        free(cur_line);
        free(add);
    } else {
        exit(1);
    }
}
