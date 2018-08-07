#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

pthread_rwlock_t rwlock;

void *run(void *arg) {
    printf("%d 玩家准备加读锁\n", pthread_self());
    // int err = pthread_rwlock_wrlock(&rwlock);
    int err = pthread_rwlock_rdlock(&rwlock);
    if(err != 0) {
        fprintf(stderr, "%s\n", strerror(err));
    }
    printf("%d 加锁完成\n", pthread_self());
}

int  main(int argc, char const *argv[]) {
    pthread_t thread[4];

    pthread_rwlock_init(&rwlock, NULL);
    for(int i = 0; i < 4; i++) {
        pthread_create(&thread[i], NULL, run, NULL);
    }
    
    for(int i = 0; i < 4; i++) {
        pthread_join(thread[i], NULL);
    }
    
    return 0;
}
