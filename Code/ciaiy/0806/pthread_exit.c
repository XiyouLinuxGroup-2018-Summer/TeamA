#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int a;
    int b;
    int c;
}NODE;

void *run(void *arg);


void *run(void * arg) {
    printf("send a package to other thread\n");
    // NODE *package = malloc(12);
    // package->a = 5;
    // package->b = 2;
    // package->c = 0;
    // pthread_exit((void *)package);
    NODE package;
    package.a = 5;
    package.b = 2;
    package.c = 0;
    pthread_exit((void *)&package);
}

int main(void) {
    pthread_t thread1;
    int err;
    NODE * p;
    err = pthread_create(&thread1, NULL, run, NULL);
    if(err != 0) {
        fprintf(stderr, "%s", strerror(err));
    }

    pthread_join(thread1, (void **)&p);
    printf("%d %d %d\n", p->a, p->b, p->c);
    return 0;
}