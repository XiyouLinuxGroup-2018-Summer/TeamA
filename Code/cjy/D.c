/*************************************************************************
	> File Name: D.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月25日 星期三 06时50分24秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
//int creat(const char *pathname,mode_t mode);

int main(int argc,char *argv[])
{
    int fd;

    //RWXU  所有者所有权限
    if(fd=creat(argv[1],S_IRWXU)==-1)
    {
        perror("creat");
        exit(1);
    }
    else
    {
        printf("创建成功\n");
    }

    close(fd);
    return 0;
}
