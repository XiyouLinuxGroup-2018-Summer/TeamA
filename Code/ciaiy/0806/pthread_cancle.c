#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void clean(void *arg) {
    printf("我就笑笑, mmp!");
    printf("溜了溜了");
}

void *run(void *arg) {
    pthread_cleanup_push(clean, NULL);
    if(arg != NULL) {
        sleep(3);
        printf("哈毛线, 去死吧\n");
        pthread_cancel(*(pthread_t *)arg);
    }else {
        while(1) {
            printf("哈哈哈哈\n");
            sleep(1);
        }
        pthread_exit((void *)2);
    }
    pthread_cleanup_pop(0);
}

int main(void) {
    pthread_t thread[2];
    int err;
    err = pthread_create(&thread[0], NULL, run, NULL);
    err = pthread_create(&thread[1], NULL, run, (void *)&thread[0]);
    sleep(1000);
    return 0;
}