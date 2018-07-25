/*************************************************************************
	> File Name: J.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月25日 星期三 09时43分56秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>

int my_readir(const char * path)
{
    DIR * dir;
    struct dirent *ptr;


    if((dir=opendir(path))==NULL)
    {
        perror("opendir");
        return -1;
    }

    while((ptr=readdir(dir))!=NULL)
    {
        printf("file name:%s\n",ptr->d_name);
    }
    closedir(dir);

    return 0;
}


int main()
{
    /*if(argc<2)
    {
        printf("listfile<target path>\n");
        exit(1);
    }
    */
    char buf[100];

    getcwd(buf,100);
    
    if(my_readir(buf)<0){
        exit(1);
    }

    return 0;
}


