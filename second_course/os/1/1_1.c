#include<stdio.h>

int
main (void){
    double x, y;
    scanf("%lf %lf", &x, &y);
    if (x >= 2.0 && x <= 5.0 &&
            y >= 1.0 && y <= 7.0
            && x - y - 2 <= 0){
        printf("%d\n", 1);
    } else {
        printf("%d\n", 0);
    }
    return 0;
}
