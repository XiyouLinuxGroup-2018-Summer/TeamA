#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#define MASK S_IRUSR | S_IRGRP | S_IROTH
int main(int argc,char*argv[])
{
    int fd;
    mode_t mask = umask(MASK);
    if(argc != 2)
    {
        printf("wrong\n");
        exit(1);
    }
    if((fd = open(*(argv+1),O_CREAT|O_RDWR|O_EXCL,0777)) == -1)
    {
        printf("OPEN FAILED\n");
        exit(1);
    } 
    return 0;
}
