/*************************************************************************
	> File Name: E.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月25日 星期三 07时05分50秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
//int stat(const char *file_name,struct stat*buf)



int main(int argc,char ** argv)

{
    struct stat buf;
    int te;

    if(argc!=2)
    {
        printf("请输入文件名\n");
        exit(0);
    }

    if(te=stat(argv[1],&buf)==-1)
    {
        printf("获取失败\n");
        perror("stat:");
        exit(1);
    }

    printf("%d\n",buf.st_mode);
    printf("%d\n",buf.st_uid);

    return 0;

}

