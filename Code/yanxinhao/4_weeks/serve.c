#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include<signal.h>
#include<time.h>
#include"cJSON.h"

#define LOGIN 1
#define REGISTER 2

typedef struct user_info{
		char username[20];
		char password[20];
		int status;
		int self_fd;
}USER_INFO;

typedef struct package{
		int type;
		char send_name[20];
		char recv_name[20];
		int send_fd;
		int recv_fd;
		char words[2048];
}PACK;

typedef struct group{
		int peop_num;
		char group_host[20];
}GROPF_INFO;

MYSQL mysql;
int listen_fd,epoll_fd;
short PORT = 1234;

pthread_mutex_t mutex;

int main()
{
	//屏蔽Ctrl+c
	//signal(SIGINT,signal_close);
	//初始化锁
	pthread_mutex_init(&mutex,NULL);

	printf("服务器开始启动...\n");
	epoll_fd = epoll_create(10000);
	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	//设置套接字参数
	int opt = 1;
	setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

	//epoll配置
	struct epoll_event ev,events[2000];
	ev.data.fd = listen_fd;
	ev.events = EPOLLIN;
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_fd,&ev);

	//套接字设置
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		printf("绑定失败\n");
		exit(-1);
	}
	printf("已绑定\n");
	if(listen(listen_fd,20)<0)
	{
		printf("监听失败\n");
		exit(-1);
	}
	printf("监听中\n");
	printf("服务器初始化完成\n");

	int evs;
	int cli_fd;
	int cli_len = sizeof(struct sockaddr_in);
	while(1)
	{
		evs = epoll_wait(epoll_fd,events,2000,1000);
		printf("%d",evs);
		for(int i=0;i<evs;i++)
		{
			if(events[i].data.fd == listen_fd)
			{
				cli_fd = accept(listen_fd,(struct sockaddr*)&server_addr,&cli_len);
				printf("新的连接请求: %d\n",cli_fd);
				//将该套接字设置后注册到epoll中
				ev.data.fd = cli_fd;
				ev.events = EPOLLIN;
				epoll_ctl(epoll_fd,EPOLL_CTL_ADD,cli_fd,&ev);
			}
			else if(events[i].events&EPOLLIN)
			{
						
			}
		}
	}



}


















