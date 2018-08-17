#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t lock;

void *run(void *arg) {
    printf("%d 尝试加锁\n", pthread_self());
    pthread_mutex_lock(&lock);
    printf("%d 加锁成功\n", pthread_self());
    sleep(1000);
    pthread_mutex_unlock(&lock);
}

int main(void) {
    pthread_t a, b;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&a, NULL, run, NULL);
    pthread_create(&b, NULL, run, NULL);

    sleep(12213);
    exit(0);
}