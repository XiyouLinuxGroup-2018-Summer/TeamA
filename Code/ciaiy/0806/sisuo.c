#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t a, b;

void *run1(void *arg) {
    printf("%x 尝试给a上锁\n", pthread_self());
    pthread_mutex_lock(&a);
    sleep(4);
    printf("%x 尝试给b上锁\n", pthread_self());
    pthread_mutex_lock(&b);
    printf("%x 解锁\n", pthread_self());
    pthread_mutex_unlock(&a);
    pthread_mutex_unlock(&b);
}

void *run2(void *arg) {
    printf("%x 尝试给b上锁\n", pthread_self());
    pthread_mutex_lock(&b);
    sleep(4);
    printf("%x 尝试给a上锁\n", pthread_self());
    while(pthread_mutex_trylock(&a) != 0) {
        printf("%x 尝试失败, 它把b打开了, 并等待3秒\n", pthread_self());
        pthread_mutex_unlock(&b);
        sleep(3);
        pthread_mutex_lock(&b);
    }
    // printf("%x 尝试给a上锁\n", pthread_self());
    // pthread_mutex_lock(&a);    
    printf("%x 解锁\n", pthread_self());
    pthread_mutex_unlock(&a);
    pthread_mutex_unlock(&b);
}

int main(void) {
    pthread_t a, b;
    int err;
    err = pthread_create(&a, NULL, run1, NULL);
    err = pthread_create(&b, NULL, run2, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    exit(0);
}               