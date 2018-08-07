#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void *run(void *arg) {
    printf("do something\n");
    sleep(3);
    pthread_exit((void *)520);
}

int main(void) {
    pthread_t pthread1;
    int err;
    void *val;
    err = pthread_create(&pthread1, NULL, run, NULL);
    if(err != 0) {
        fprintf(stderr, "%s", strerror(err));
    }

    printf("我正在等子线程\n");
    pthread_join(pthread1, &val);
    printf("收到了%d 嘿嘿嘿\n", (int)val);
    printf("等待结束, 退出~\n");
    exit(0);
}