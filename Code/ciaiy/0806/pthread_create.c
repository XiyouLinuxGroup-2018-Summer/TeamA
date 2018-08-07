#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static int tickets = 10;

void *run_sale(void *);

void *run_sale(void *arg) {
    printf("**");
    while(tickets > 0) {
        printf("%d : now : %d\n", (unsigned int)pthread_self(), --tickets);
    }
}

int main(void) {
    pthread_t thread1, thread2;
    int err;

    err = pthread_create(&thread1, NULL, run_sale, NULL);
    if(err != 0) {
        fprintf(stderr, "%s", strerror(err));
    }
    err = pthread_create(&thread2, NULL, run_sale, NULL);
    if(err != 0) {
        fprintf(stderr, "%s", strerror(err));
    }

    sleep(1000);
    exit(0);
}