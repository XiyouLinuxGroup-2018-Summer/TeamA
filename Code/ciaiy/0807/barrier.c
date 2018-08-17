#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

pthread_barrier_t bar;

void *run(void *arg) {
    printf("%d 正在写作业\n", pthread_self());
    sleep(*(int *)arg);
    printf("%d 写完作业啦\n");
    pthread_barrier_wait(&bar);
    printf("三个小伙伴愉快的玩耍\n");
}

void printErr(int err) {
    if (err != 0) {
        fprintf(stderr, "%s\n", strerror(err));
    }
}

int main(void) {
    pthread_t stu[3];
    int err;

    pthread_barrier_init(&bar, NULL, 3);
    for(int i = 0; i < 3; i++) {
        int *temp = (int *)malloc(4);
        *temp = i;
        err = pthread_create(&stu[i], NULL, run, (void *)temp);
        printErr(err);
    }

    for(int i = 0; i < 3; i++) {
        pthread_join(stu[i], NULL);
    }
    return 0;
}