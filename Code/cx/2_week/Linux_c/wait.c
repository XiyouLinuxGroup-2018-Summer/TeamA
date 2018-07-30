#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<wait.h>
#include<unistd.h>
int main (int argc ,char * argv[])
{
    pid_t pid;
    int exit_code,k;
    char * msg;
    pid = fork();
    switch(pid)
    {
        case 0:
            k = 5;
            exit_code = 5;
            msg = "child process\n";
            break;
        case -1:
            printf("creat failed\n");
            exit(1);
        default:
            k = 2;
            msg = "parent process\n";
            exit_code = 0;
            break;
    }
    int stat_val;
    pid_t  child_val;
    child_val = wait(&stat_val);
    if(pid != 0)
    {
        printf("child process %d exited",getpid());
        if(WIFEXITED(stat_val))
            printf("child process %d ",WEXITSTATUS(stat_val));
        else
        printf("abnormal\n");
    }
    else
    {
            while(k-- > 0)
            {
                puts(msg);
                sleep(1);
            }
    }
    exit(exit_code);
    return 0;
}