/*************************************************************************
	> File Name: H.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月25日 星期三 09时08分58秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main(void)
{
    char buf[100];

    getcwd(buf,100);

    puts(buf);
    
    return 0;
}
