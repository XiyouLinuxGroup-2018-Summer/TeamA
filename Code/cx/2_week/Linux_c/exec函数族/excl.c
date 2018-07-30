#include<stdio.h>
#include<unistd.h>
int main (int argc,char * argv[])
{
    //execl("/bin/ls","/bin/ls","/tmp");
    //execlp("ls","ls","-al","/tmp",(char*)0);
    char *arg [] = {"ls","-al","/tmp",(char*)0};
    execv("/bin/ls",arg);
    return 0;
}
