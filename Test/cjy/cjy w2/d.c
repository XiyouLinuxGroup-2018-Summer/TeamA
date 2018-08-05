/*************************************************************************
	> File Name: d.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月04日 星期六 17时15分01秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>  
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
	printf("Hello, my pid is %d\n", getpid());
	pid_t pid = fork();
if (pid == 0)
{
	printf("I was forked! :D %d\n", getpid());
}
else
{
	waitpid(pid, NULL, 0);
	printf("%d was forked!\n", pid);
}
return 0;
}



/*
输出是Hello, my pid is 11685
I was forked! :D 11686
11686 was forked!
./test | cat 显示的是进程完全执行的东西
*/



