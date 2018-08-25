/*************************************************************************
	> File Name: clientview.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月20日 星期一 15时35分47秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <stdio.h>
#include <math.h>
#include "./cJSON.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

#include "clientfun.h"
#include "public.h"

int fd;
char name[50];
char *IP = "128.0.0.101";
short PORT = 8080;

void *task(void *p);

void fa(int signo)
{
	printf("正在退出...\n");
	sleep(1);
	close(fd);
	exit(0);
}


int main()
{
	signal(SIGINT, fa); //将信号2注册为自定义函数，用于退出
	printf("正在启动...\n");
	//sleep(3);
	printf("启动成功，请输入用户名:");
	//scanf("%s",name);

	/*步骤和服务器端差不多*/
	fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("本客户端fd为%d\n", fd);
	if (fd == -1)
		perror("socket"), exit(-1);
	/*创建通信地址*/
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	/*连接服务器*/
	int res = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (res == -1)
		perror("connect"), exit(-1);
	printf("连接服务器成功！\n");
	sleep(1);
	//进入登录界面
	view(fd);

}


