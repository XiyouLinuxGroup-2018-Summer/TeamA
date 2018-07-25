// 创建一个具有文件空洞的源程序
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
    int fd; 
    int len;
    char inA[10] = "123456789";
    char inB[10] = "987654321";
    
    if((fd = open("fileWithHole", O_WRONLY|O_CREAT, 0644)) == -1) {
        perror("open error");
        exit(1);
    }
    if((len = write(fd, inA, 10)) != 10) {
        perror("write error");
        exit(1);
    }
    if(lseek(fd, 4*1024 - 10, SEEK_SET) == -1) {
        perror("lseek error");
        exit(1);
    }
    if((len = write(fd, inB, 10)) != 10) {
        perror("write error");
        exit(1);
    }
    
    return 0;
}