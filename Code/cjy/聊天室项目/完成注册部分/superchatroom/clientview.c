/*************************************************************************
	> File Name: clientview.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 14时30分25秒
 ************************************************************************/

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
#include"clientfun.h"


int fd;
char name[50];
char *IP = "127.0.0.1";
short PORT = 8080;

void* task(void* p);

void fa(int signo)
{
   printf("正在退出...");
   sleep(1);
   close(fd);
   exit(0);
}


/*线程函数，用于读服务器发送过来的数据*/
void* task(void* p)
{
   while(1){
      char buf[100] = {};
      if(read(fd,buf,sizeof(buf))<=0)
         return;
      printf("%s\n",buf);
      memset(buf,0,sizeof(buf));
   }
}



void view(int fd)
{
	printf("请选择\n");

	printf("\n\n\n\n");
	printf("[A]登录 | [B]注册\n");
	char ch;

	printf("请输入你的选择:");

	scanf("%c",&ch);
	getchar();
	if(ch=='B'||ch=='b')
		zhuce_cl(fd);
	//else if(ch=='B'||ch=='b')
		//denglu();
}

/*
void creatjson()
{
	
}
*/

int main()
{
	signal(SIGINT,fa);        //将信号2注册为自定义函数，用于退出
	printf("正在启动...\n");
	//sleep(3);
	printf("启动成功，请输入用户名:");
	//scanf("%s",name);

	   /*步骤和服务器端差不多*/
	fd = socket(AF_INET,SOCK_STREAM,0);
	if (fd==-1) perror("socket"),exit(-1);
	/*创建通信地址*/
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	/*连接服务器*/
	int res = connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(res == -1) perror("connect"),exit(-1);
	printf("连接服务器成功！\n");

	//进入登录界面
	view(fd);

	/*
	write(fd,name,strlen(name));
		/*客户端除了要给服务器发送数据外，还要接收服务器的数据
	而这两个步骤是并行的，所以我们在主线程中写，开辟的线程中读*/
	/*
	pthread_t pid;
	pthread_create(&pid,0,task,0);
	char msg[100] = {};
	while(1){
		scanf("%s",msg);
		write(fd,msg,strlen(msg));
		/*必须把缓冲区清空*
		memset(msg,0,strlen(msg));
	}
	*/
}

