#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int a = 1;
int main (int argc,char *argv[])
{
    pid_t pid;
      int b = 2,i = 0;
    //pid = fork();
    pid = vfork();
    switch(pid)
    {
        case 0:
        i = 3;
         while(i-- >0)
         {
              printf("child process\n");
             a++;
             b++;
             sleep(3);
         }
         printf("child running %d parent :%d   a:%d  b:%d\n",getpid(),getppid(),a,b);
         printf("%d",b);
         exit(0);
         case -1:
            printf("creat filed");
             exit(1);
        default:
            i = 5;
             while(i-- >0)
             {
                  printf("parent process\n");
               a++;
               b++;
               sleep(3);
               printf("%d\n",b);
            }
             printf("  a:%d  b:%d\n",a,b);
             exit(0);
     }
            printf("\n");
     if(a == 4 && b == 5)
          printf("fork\n");
          if(a == 6 && b == 7)
          printf("fork\n");
          if(a == 9 && b == 10)
          printf("vfork\n");
            return 0;
}
