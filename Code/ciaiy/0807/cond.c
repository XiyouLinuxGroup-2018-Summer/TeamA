#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int a = 0;

void *run2(void *arg) {
    printf("%d run2 \n", pthread_self());
    pthread_mutex_lock(&lock);
    printf("%d run2 解锁成功");
    a++;
    pthread_mutex_unlock(&lock);
    printf("%d run2 发送signal\n", pthread_self);
    //pthread_cond_signal(&cond);
}
void * run3 (void *arg) {
    printf("3 尝试加锁\n");
    pthread_mutex_lock(&lock);
    printf("3 加锁成功\n");
    sleep(100);
}
void *run1(void *arg) {
        printf("%d 准备加锁\n", pthread_self());
        pthread_mutex_lock(&lock);
        printf("%d 加锁成功\n", pthread_self());
        while(a == 0) {
            printf("等待cond\n");
            pthread_cond_wait(&cond, &lock);
        }
        printf("%d 响应了条件变量\n", pthread_self());
        a++;
        printf("%d 解锁\n", pthread_self);
        pthread_mutex_unlock(&lock);
}

void printErr(int err) {
    if(err != 0) {
        fprintf(stderr, "%s\n", strerror(err));
    }
}

int main(void) {
    pthread_t thread[4], thread_s;
    pthread_mutex_init(&lock, NULL);
    int err;
    for(int i = 0; i < 4; i++) {
        err = pthread_create(&thread[i], NULL, run1, NULL);
        printErr(err);
    }
    err = pthread_create(&thread_s, NULL, run3, NULL);
    printErr(err);


    for(int i = 0; i < 4; i++) {
        pthread_join(thread[i], NULL);
    }
    pthread_join(thread_s, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);
    return 0;
}