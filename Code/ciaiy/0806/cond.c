#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_cond_t cond;
pthread_mutex_t lock;

void *run_man(void *arg) {
    printf("%d 员工在线\n", pthread_self());
    pthread_cond_wait(&cond, &lock);
    printf("%d : 卧槽, 又要加班!!!!\n", pthread_self());
}

void *run_boss(void *arg) {
    sleep(2);
    printf("boss 发了一条通知!\n");
    pthread_mutex_lock(&lock);
    pthread_mutex_unlock(&lock);
    pthread_cond_broadcast(&cond);

    sleep(1);
    printf("boss 又发了一条通知!\n");
    pthread_mutex_lock(&lock);
    pthread_mutex_unlock(&lock);
    pthread_cond_broadcast(&cond);
}

int main(void) {
    int err;
    pthread_t boss;
    pthread_t man[4];
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&lock, NULL);
    pthread_create(&boss, NULL, run_boss, NULL);
    for(int i = 0; i < 4; i++) {
        err = pthread_create(&man[i], NULL, run_man, NULL);
        if(err != 0) {
            fprintf(stderr, "%s\n", strerror(err));
        }
    }

    for(int i = 0; i < 4; i++) {
        pthread_join(man[i], NULL);
    }
    pthread_join(boss, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}