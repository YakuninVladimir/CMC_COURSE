#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void perm_gen (int iter, int n, bool *used, int *perm){
    if (iter == n) {
        for (int i = 0; i < n; i++){
            printf("%d", perm[i]);
        }
        printf("\n");
    } else {
        for (int i = 0; i < n; i++){
            if (!used[i]){
                perm[iter] = i + 1;
                used[i] = true;
                perm_gen(iter + 1, n, used, perm);
                used[i] = false;
            }
        }
    }
}

int main (void) {
    int n;
    scanf("%d", &n);
    int *permutation = calloc(n, 4);
    bool *used_elems = calloc(n, 1);
    perm_gen(0, n, used_elems, permutation);
}
