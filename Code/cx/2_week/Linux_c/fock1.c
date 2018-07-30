#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main (int argc ,char *argv[])
{
    pid_t pid;
    pid = fork();
    printf("%d\n",pid);
    int i = 0;
    switch (pid)
    {
        case 0:
              //  while(i++<5)
             //   {
                printf("child process %d  parent : %d\n",getpid(),getppid());
              //  sleep(3);
              //  }
            break;
        case -1:
            printf("process failure\n");
            break;
        default:
                printf("parent %d\n",getpid());
                exit(1);
        printf("\n");
    }
    return 0;
}