#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_key_t key;

void *run(void *arg) {
    int *value = (int *)malloc(4);
    *value = 10;
    pthread_setspecific(key, (void *)value);
    printf("%d", *(int *)pthread_getspecific(key));
}

void freeKey(void *arg) {
    free((int *)pthread_getspecific(key));
}

void printErr(int err) {
    if(err != 0) {
        fprintf(stderr, "%s\n", strerror(err));
    }
}
int main(void) {
    pthread_key_create(&key, freeKey);
    pthread_t a, b;
    int err;
    pthread_create(&a, NULL, run, NULL);
    pthread_create(&b, NULL, run, NULL);    

    pthread_join(a, NULL);
    pthread_join(b, NULL);
    exit(0);
}