/*************************************************************************
	> File Name: A.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 15时33分05秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>


int main(int argc,char *argv[])
{
    int status;
    pid_t pid;

    char *arg[50];

    //char buf[100];
    //getcwd(buf,100);

    arg[0]="./ca";
    //arg[0]="ls";
    //arg[2]="-l";
    //arg[1]="shiyan";
    arg[1]=NULL;

    //rintf("%s\n",buf);

    if((pid = fork())<0)
    {
        printf("fork error\n");
        return 0;
    }
    //pid为0说明是子进程
    if(pid==0)
    {
        printf("已运行\n");
        execvp(arg[0],arg);
        //execl("/ls","ls","-l","/home/chujiangke");
        exit(0);
    }
    
    if(waitpid(pid,&status,0)==-1)
        printf("wait for child process error\n");
    return 0;
}

