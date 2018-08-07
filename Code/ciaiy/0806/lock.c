#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
pthread_mutex_t lock;

void clean(void * arg);

void clean(void *arg) {
    printf("%x 离开了餐馆\n", pthread_self());
}
void *run_eatFood(void *arg) {
    pthread_cleanup_push(clean, NULL);
    printf("%x want to eat food\n", pthread_self());
    pthread_mutex_lock(&lock);
    printf("%x eating\n", pthread_self());
    sleep(3);
    pthread_mutex_unlock(&lock);
    pthread_cleanup_pop(1);
}


int main(void) {
    pthread_t a, b;
    int err;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&a, NULL, run_eatFood, NULL);
    pthread_create(&b, NULL, run_eatFood, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    exit(0);
}