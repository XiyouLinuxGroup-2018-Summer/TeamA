/*************************************************************************
	> File Name: b.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月04日 星期六 16时43分31秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>


int main(int argc, char *argv[])
{
    printf("haha\n");


    for (int i = 0; i < 2; i++)
    {
        pid_t pid = fork();
        if(pid==0)
        {    
            printf("这是i:%d 这是子进程hello, world?  直行至%d 自己的Id是%d 父亲id是%d\n",i,pid,getpid(),getppid());
        }
        else if(pid==-1)
        {
            printf("创建进程失败\n");
        }
        else
        {
            printf("这是i:%d 这是父进程hello,world? 子id是%d 自己的id是%d\n",i,pid,getpid());
        }

    }
    return 0;
}

//一共输出6次hello, world?
//不算自己本身，新产生了3个进程
//最终因为父进程结束，所以剩下的子进程被关闭
