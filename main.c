#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

//global memories:
int comparisons = 0;
int swaps = 0;

//supporting functions:

//get time (milliseconds)
long long int millis(void)
{
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((long long int)now.tv_sec) * 1000 + ((long long int)now.tv_nsec) / 1000000;
}

//comparator for sorts
int comparator(long long int a, long long int b) {
    return llabs(a) - llabs(b);
}

//random array generator
long long int* random_array(int n) {
    srand(time(NULL));
    long long int* li = calloc(n, sizeof(long long int));
    for (int i = 0; i < n; i++) {
        li[i] = (RAND_MAX / 2 - rand()) % 15;
    }
    return li;
}

//copy of array
long long int* clone_array(long long int* src, int n) {
    long long int* dst = calloc(n, sizeof(long long int));
    for (int i = 0; i < n; i++) dst[i] = src[i];
    return dst;
}

//printf of array
void print_array(long long int* li, int n) {
    for (int i = 0; i < n; i++) {
        printf("%lld ", li[i]);
    }
    printf("\n");
}

// sort functions;

//Shell sort
void shellsort( long long int* li, int n) {
    //define distance between checking elements
    int d = n / 2;
    while (d > 0) {
        //sort with fixed distance
        for (int i = d; i < n; i++) {
            int k = i;
            //insertion sort
            while (k >= d) {
                if (comparator(li[k - d], li[k]) < 0) {
                    comparisons++;
                    swaps++;
                    long long int buf = li[k];
                    li[k] = li[k - d];
                    li[k - d] = buf;
                }
                else break;
                k -= d;
            }
        }
        d /= 2;
    }

}

//turn array into heap
void heapify(long long int * li, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && llabs(li[l]) < llabs(li[largest])){
        largest = l;
        comparisons++;
    }

    if (r < n && llabs(li[r]) < llabs(li[largest])) {
        largest = r;
        comparisons++;
    }

    if (largest != i)
    {
        comparisons++;
        swaps++;
        long long buf = li[largest];
        li[largest] = li[i];
        li[i] = buf;
        heapify(li, n, largest);
    }
}

//heap sort
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

//print number of swaps and comparisons

void clean_data(void) {
    comparisons = 0;
    swaps = 0;
}

void print_data(void) {
    printf("number of comparisons: %d\n", comparisons);
    printf("number of swaps: %d\n", swaps);
}


int main(void) {
    //enter length
    int n;
    scanf("enter length of test array: %d", &n);
    //create test arrays
    long long int* list1 = random_array(n);
    long long int* list2 = clone_array(list1, n);
    //print test array
    if (n < 100)print_array(list1, n);
    long long int t1, t2;
    //run shell sort
    long long int timer = millis();
    clean_data();
    shellsort(list1, n);
    printf("Shellsort result: \n");
    print_data();
    t1 = millis() - timer;
    //run heap sort
    timer = millis();
    clean_data();
    heapsort(list2, n);
    printf("Heapsort result: \n");
    print_data();
    t2 = millis() - timer;
    //print results
    if (n < 100) {
        printf("Shellsort: \n");
        print_array(list1, n);
        printf("Heapsort: \n");
        print_array(list2, n);
    }
    printf("Shellsort time: %lld\nHeapsort time : %lld\n", t1, t2);

    return 0;
}
