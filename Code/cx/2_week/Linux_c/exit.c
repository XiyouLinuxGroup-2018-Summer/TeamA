#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<error.h>
int main (int argc,char * argv[])
{
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        printf("wrong\n");
        return 0;
    }
    else if(pid == 0)
    {
        printf("测试数据");
        exit(0);
    }
    else
    {
        sleep(3);
        printf("测试数据");
        _exit(0);
    }
    return 0;
}