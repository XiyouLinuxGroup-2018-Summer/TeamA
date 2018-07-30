#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main (int argc,char * argv[])
{
    pid_t pid;
    pid = fork();
    char *msg;
    int k = 0,i = 0;
    switch(pid)
    {
        case 0:
           // printf("child pid %d,parent : %d\n",pid,getppid());
            msg = "child process runing\n";
            k = 2;
            break;
        case -1:
            printf("process failure\n");
            break;
        default:
           // printf("parent process runing current:%d,parent process %d\n",pid,getpid());
           msg = "parent process runing\n";
            k = 3;
            break;
            printf("\n");
    }
    while(i++ < k)
    {  
        puts(msg);
        sleep(1);
    }
    return 0;
}
