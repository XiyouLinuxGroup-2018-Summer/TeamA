// 学习硬链接时 查看inode
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
    char path[256];
    struct stat buf;
    scanf("%s", path);
    stat(path, &buf);   
    printf("%d\n", buf.st_ino);
    return 0;
}