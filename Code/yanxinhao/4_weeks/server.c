#include"heads.h"


int main()
{
	int sock_fd,cli_fd,err=1;
	struct sockaddr_in serv_addr,cli_addr;
	struct epoll_event ev,events[EPOLL_MAX];
	PACK recv_package;
	PACK * recv_package_t;
	
	printf("数据库启动...\n");
	//初始化MySQL句柄
//	MYSQL mysql;
	if(NULL == mysql_init(&mysql))
			printf("数据库初始化:%s\n",mysql_error(&mysql));
	//建立MySQL连接
	if(NULL == mysql_real_connect(&mysql,"localhost","debian-sys-maint","jOE9r7pWBUZLvHIY","chatroom",0,NULL,0))
			printf("数据库连接:%s\n",mysql_error(&mysql));
	printf("数据库连接成功\n");

	printf("服务器启动中...\n");

	//生成epoll句柄
	int	epollfd = epoll_create(EPOLL_MAX);

	//产生套接字
	if((sock_fd = socket(AF_INET,SOCK_STREAM,0))<0)
		my_err("socket",__LINE__);

	//设置套接字
	if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,(void*)&err,sizeof(int))<0)
			my_err("setsockopt",__LINE__);

	//设置要监视该套接字
	ev.data.fd = sock_fd;
	//设置为有连接请求或有可读数据事件时触发
	ev.events = EPOLLIN;
	//注册一个要处理的套接字
	epoll_ctl(epollfd,EPOLL_CTL_ADD,sock_fd,&ev);

	//设置套接字相关参数
	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//绑定套接字
	if(bind(sock_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))<0)
			my_err("bind",__LINE__);
	
	//设置套接字为监听状态
	if(listen(sock_fd,LISTEN_Q)<0)
			my_err("listen",__LINE__);
	
	printf("服务器启动成功\n");

	int cli_len = sizeof(struct sockaddr_in);
	int wait_fd_num;

	while(1)
	{
		wait_fd_num = epoll_wait(epollfd,events,EPOLL_MAX,1000);
		printf("%d\n",wait_fd_num);
		
		for(int i=0;i<wait_fd_num;i++)
		{
			if(events[i].data.fd == sock_fd)
			{
				cli_fd = accept(sock_fd,(struct sockaddr*)&cli_addr,&cli_len);
				printf("新的连接请求: %d\n",cli_fd);
				//将该套接字设置后注册到epoll中
				ev.data.fd = cli_fd;
				ev.events = EPOLLIN;
				epoll_ctl(epollfd,EPOLL_CTL_ADD,cli_fd,&ev);
			}
			else if(events[i].events & EPOLLIN)
			{
					int ret;
					if((ret = my_read(events[i].data.fd,(void*)&recv_package,sizeof(PACK)))<0)
							my_err("自定义写函数错误",__LINE__);
					recv_package.data.send_fd = events[i].data.fd;
					printf("用户名:%s接收方:%s密码:%s\n",recv_package.data.send_name,recv_package.data.recv_name,recv_package.data.mes);
					//客户端离线
					if(ret==0)
					{
						//状态设置...
						printf("%d下线\n",events[i].data.fd);
						ev.data.fd = events[i].data.fd;
						epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
						close(ev.data.fd);
					}
					
					int type = recv_package.type;
					pthread_t  tid;
					recv_package_t = (PACK*)malloc(sizeof(PACK));
					memcpy(recv_package_t, &recv_package, sizeof(PACK));
					printf("连接方套接字%d\n",recv_package_t->data.send_fd);

					if(pthread_create(&tid,NULL,pack_deal,(void*)recv_package_t)<0)
							my_err("pthread_create",__LINE__);
			//		pthread_join(id,NULL);
					if(type==LOGIN)
					{	
							printf("登录中...\n");
							usleep(10000);
					}


			}

		}

	}
	return 0;

}



