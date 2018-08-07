#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
int a = 10;

void *run(void *arg) {
    printf("%d\n", ++a);
}

int main(void) {

    pthread_t thread1, thread2;
    int err;

    pthread_create(&thread1, NULL, run, NULL);
    pthread_create(&thread2, NULL, run, NULL);

    sleep(3);
    exit(0);
}