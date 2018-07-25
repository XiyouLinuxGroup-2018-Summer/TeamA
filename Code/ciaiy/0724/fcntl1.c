#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int val;    // 接受fcntl函数的返回值

    if(argc != 2) {
        fprintf(stderr, "error : fcntl1 <fd> \n");
        exit(1);
    }

    if((val = fcntl(atoi(argv[1]), F_GETFL)) == -1) {   // 获取文件描述符标识的信息
        perror("fcntl error");
        exit(1);
    }

    // 分析文件描述符标识

    switch(val & O_ACCMODE) {   // RD 00 WR 01 WRRD 10 O_ACCMODE 11 因此只能使用O_ACCMODE屏蔽字
        case O_RDONLY : 
            printf("read");
            break;
        case O_WRONLY :
            printf("write");
            break;
        case O_RDWR :
            printf("read & write");
            break;
        default :
            fprintf(stderr, "unknown access mode");
            exit(1);        
    }
    
    if(val & O_APPEND) {
        printf(", append");
    }
    if(val & O_NONBLOCK) {
        printf(", nonblocking");
    }
    if(val & O_SYNC) {
        printf(", synchronous writes"); 
    }
    printf("\n");

    return  0;
}