// 学习设置用户ID 和 unlinkhe
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
    int fd;
    if((fd = open("/root/temp", O_WRONLY|O_CREAT, 0644)) == -1) {
        perror("error");
    }
    
    sleep(4);
    unlink("/root/temp");
    
    return 0;
}