#include<stdio.h>
#include<stdlib.h>

int
main(void){
    int n;
    scanf("%d", &n);
    int *cur_line = calloc(n - 1, 4);
    int *add = calloc(n - 1, 4);
    for (int a = 1; a < n; a++){
        for (int b = 0; b < n; b++){
            if (a * b % n == 1){
                add[a - 1] = b;
                break;
            }
        }
    }
    for (int c = 0; c < n; c++){
        for (int a = 1; a < n; a ++){
            printf("%d ", cur_line[a - 1]);
            cur_line[a - 1] = (cur_line[a - 1] + add[a - 1]) % n;
        }
        printf("\n");
    }
}
