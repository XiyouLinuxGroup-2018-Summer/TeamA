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

void menu(void);
void choose(void);

char *IP = "127.0.0.1";
short PORT  = 1234;
int sock_fd;

int main()
{
	struct sockaddr_in serv_addr;

	sock_fd = socket(AF_INET,SOCK_STREAM,0);

	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(IP);

	printf("客户端启动中...\n");

	if(connect(sock_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
			printf("链接服务时错误\n");
			exit(-1);
	}

	printf("客户端已连接至服务端\n");
	menu();
	choose();
}
void menu()
{
	fflush(stdout);
	printf("###############\n");
	printf("\t1.注册\n");
	printf("\t2.登录\n");
	printf("\t0.退出\n");
	printf("###############\n");
	printf("请输入选择:");
	fflush(stdin);
}
void choose(void)
{
	int n;
	scanf("%d",&n);
	switch(n)
	{
		case 1:
			;
			break;
		case 2:
			;
			break;
		case 0:
			exit(0);
			break;
		default:
			printf("输入错误,请重新输入!\n");
			menu();
			choose();
	}
}


//注册
void reg(void)
{
	char username[20];
	char passeord[20];
	printf("\t请输入用户名:");
	scanf("%s",username);
	printf("\t请输入密码:");
	scanf("%s",password);
	PACK pack;
	memset(&pack,0,sizeof(PACK));
	cJSON * packjson = cJSON_CreateObject();
	cJSON_AddNumberToObject(packjson,"type",REGISTER);
	cJSON_AddStringToObject(packjson,"username",username);
	cJSON_AddStringToObject(packjson,"password",password);
	char *out = cJSON_PrintUnformatted(packjson);
	int ret = strlen(out);
	int nsend=0,nleft=ret;
	while(nleft!=0)
	{
		nsend = send(sock_fd,out,ret,0);
		nleft-=nsend;
	}
	free(out);
	cJSON_Delete(packjson);
	

}
