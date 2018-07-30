#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<sys/types.h>
#include<sys/resource.h>
int main (int argc,char * argv[])
{
    int stat_val = 0;
    pid_t pid;
    int old,new;
    pid = fork();
    printf("%d\n",pid);
   // wait(&stat_val);
    
    switch(pid)
    {
        case 0:
            printf("child process %d  %d\n",getpid(),getppid());
            old = getpriority(PRIO_PROCESS,0);
            printf("%d\n",old);
            new = nice(2);
            printf("%d",new);
            exit(0);
        case -1:
            printf("creat failed\n");
            break;
        default:
        sleep(3);
            printf("parent process %d\n",getpid()); 
            break;
    }
    wait(&stat_val);
   return 0;
}
