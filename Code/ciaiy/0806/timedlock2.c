#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

pthread_mutex_t time_lock;
pthread_mutex_t lock;
struct timespec ts;

void *run1(void *arg) {
    printf("1号玩家加锁\n");
    pthread_mutex_lock(&time_lock);
    sleep(10);
    printf("1号玩家解锁\n");
    pthread_mutex_unlock(&time_lock);
}

void *run2(void *arg) {
    printf("2号玩家等待加锁五秒\n");
    int err;
    err = pthread_mutex_timedlock(&time_lock, &ts);
    if(err != 0) {
        fprintf(stderr, "%s\n", strerror(err));
    }
}

int main(void) {
    pthread_t thread[4];

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 5;

    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&time_lock, NULL);

    pthread_create(&thread[0], NULL, run1, NULL);
    pthread_create(&thread[1], NULL, run2, NULL);

    for(int i = 0; i < 2; i++) {
        pthread_join(thread[i], NULL);
        printf("main : test%d\n", i);
    }
    exit(0);
}