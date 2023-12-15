#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>

enum
{
    SYS_BASE = 10,
    NUM_OF_ARGS = 2,
    ERR_CODE = -1,
};

typedef struct thr_arg
{
    int num;
    int nproc;
} thr_arg;

typedef struct msg
{
    int val;
} msg;

int
strtol_s(char *s)
{
    char *buf = NULL;
    long ans = strtol(s, &buf, SYS_BASE);
    if (*buf || buf == s || ans != (int) ans) {
        fprintf(stderr, "Error, can't convert input to integer\n");
        exit(1);
    }
    return ans;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t *cond;
msg *msgs;

pthread_mutex_t close_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t close_thr = PTHREAD_COND_INITIALIZER;
volatile bool close_f = false;

void *
handler(void *arg)
{
    thr_arg args = *(thr_arg *) arg;
    int i = args.num;
    while (1) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond[i], &mutex);
        int val = msgs[i].val;
        if (val == ERR_CODE) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        int cur;

        if (scanf("%d", &cur) == EOF) {
            pthread_mutex_lock(&close_mutex);
            pthread_mutex_unlock(&mutex);
            close_f = true;
            pthread_cond_signal(&close_thr);
            pthread_mutex_unlock(&close_mutex);
            pthread_exit(NULL);
        }

        printf("%d %d\n", i, cur);
        int new_num = cur % args.nproc;
        printf("$$$%d\n", new_num);
        pthread_cond_signal(&cond[new_num]);
        pthread_mutex_unlock(&mutex);
    }
}

int
main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS) {
        fprintf(stderr, "Error, wrong num of arguments\n");
        exit(1);
    }
    int nproc = strtol_s(argv[1]);

    pthread_t *thread_arr = calloc(nproc, sizeof(*thread_arr));
    cond = calloc(nproc, sizeof(*cond));
    msgs = calloc(nproc, sizeof(*msgs));

    for (int i = 0; i < nproc; i++) {
        msgs[i].val = 0;
        pthread_cond_init(&cond[i], NULL);
        thr_arg *cur_arg = calloc(1, sizeof(*cur_arg));
        cur_arg->nproc = nproc;
        cur_arg->num = i;
        pthread_create(&thread_arr[i], NULL, handler, cur_arg);
    }

    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond[0]);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&close_mutex);
    while (!close_f) {
        pthread_cond_wait(&close_thr, &close_mutex);
    }
    for (int i = 0; i < nproc; i++) {
        pthread_mutex_lock(&mutex);
        msgs[i].val = ERR_CODE;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond[i]);
        pthread_join(thread_arr[i], NULL);
    }
    pthread_mutex_unlock(&close_mutex);
    exit(0);
}
