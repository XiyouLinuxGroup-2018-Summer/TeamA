#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t time_lock;

int main(void) {
    struct timespec tout;  
    struct tm *tmp;
    char buf[64];

    pthread_mutex_init(&time_lock, NULL);   // 初始化锁
    pthread_mutex_lock(&time_lock); // 上锁
    printf("mutex has locked\n");
    clock_gettime(CLOCK_REALTIME, &tout);   // 得到当前时间信息
    tmp = localtime(&tout.tv_sec);  // 转换成struct tm 格式
    strftime(buf, sizeof(buf), "%r", tmp);  // 得到tmp中时分秒
    printf("current time is %s\n", buf);
    tout.tv_sec += 10;  // 加十秒
    pthread_mutex_timedlock(&time_lock, &tout); // 时间锁, 等到十秒后解锁
    tmp = localtime(&tout.tv_sec);  // 得到加十秒之后的struct tm结构
    strftime(buf, sizeof(buf), "%r", tmp);  // 得到时分秒
    printf("the time is now %s\n", buf);    // 输出
    printf("locked again\n");
    exit(0);
}