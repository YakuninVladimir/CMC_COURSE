#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int comprations = 0;
int swaps = 0;

long long int millis()
{
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((long long int)now.tv_sec) * 1000 + ((long long int)now.tv_nsec) / 1000000;
}

int comparator(long long int a, long long int b) {
    return abs(a) - abs(b);
}


long long int* random_array(int n) {
    srand(time(NULL));
    long long int* li = calloc(n, sizeof(long long int));
    for (int i = 0; i < n; i++) {
        li[i] = (RAND_MAX / 2 - rand()) % 15;
    }
    return li;
}

long long int* clone_array(long long int* src, int n) {
    long long int* dst = calloc(n, sizeof(long long int));
    for (int i = 0; i < n; i++) dst[i] = src[i];
    return dst;
}

void print_array(long long int* li, int n) {
    for (int i = 0; i < n; i++) {
        printf("%lld ", li[i]);
    }
    printf("\n");
}

void shellsort( long long int* li, int n) {
    int d = n / 2;
    while (d > 0) {
        for (int i = d; i < n; i++) {
            int k = i;
            while (k >= d) {
                if (comparator(li[k - d], li[k]) < 0) {
                    comprations++;
                    swaps++;
                    long long int buf = li[k];
                    li[k] = li[k - d];
                    li[k - d] = buf;
                }
                k -= d; 
            }
        }
        d /= 2;
    }
 
}

void heapify(long long int * li, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && abs(li[l]) < abs(li[largest])){
        largest = l;
        comprations++;
    }

    if (r < n && abs(li[r]) < abs(li[largest])) {
        largest = r;
        comprations++;
    }
   
    if (largest != i)
    {
        comprations++;
        swaps++;
        long long buf = li[largest];
        li[largest] = li[i];
        li[i] = buf;
        heapify(li, n, largest);
    }
}


void heapsort(long long int * li, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(li, n, i);
    }
    for (int i = n - 1; i >= 0; i--)
    {   
        swaps++;
        long long buf = li[0];
        li[0] = li[i];
        li[i] = buf;
        heapify(li, i, 0);
    }
}

void clean_data() {
    comprations = 0;
    swaps = 0;
}

void print_data() {
    printf("number of comprations: %d\n", comprations);
    printf("number of swaps: %d\n", swaps);
}


int main(void) {
    int n;
    scanf_s("%d", &n);
    long long int* list1 = random_array(n);
    long long int* list2 = clone_array(list1, n);
    if (n < 100)print_array(list1, n);
    long long int t1, t2;
    long long int timer = millis();
    clean_data();
    shellsort(list1, n);
    print_data();
    t1 = millis() - timer;
    timer = millis();
    clean_data();
    heapsort(list2, n);
    print_data();
    t2 = millis() - timer;
    if (n < 100) {
        print_array(list1, n);
        print_array(list2, n);
    }
    printf("Shellsort time: %lld\nHeapsort time : %lld\n", t1, t2);

    return 0;
}