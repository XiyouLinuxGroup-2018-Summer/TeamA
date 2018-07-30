#include<stdio.h>
#include<malloc.h>
extern char ** environ;
int main (int argc,char *argv[],char ** env)
{
     int i;
     printf("%d",argc);
     for(i = 0;i< argc;i++)
     {
         printf("%s\t\n",argv[i]);
     }
     i = 0;
     while(env[i] !=  NULL)
     {
      printf("%s\n",env[i]);
      i++;
     }
     /* for(i = 0;environ[i] != NULL;i++)
      printf("%s\n",environ[i]);*/
     return 0;
}