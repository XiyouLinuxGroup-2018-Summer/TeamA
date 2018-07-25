/*************************************************************************
	> File Name: B.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 06时06分14秒
 ************************************************************************/

#include<stdio.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main()
{
    int te;

    te=creat("ok.c",S_IWUSR);

    if(te==-1)
    {
        printf("文件创建失败\n");
        return 0;
    }

    else
        printf("文件创建成功\n");
    return 0;
}
