#include"./heads.h"

char *IP = "127.0.0.1";

int main()
{
	int sock_fd;
	struct sockaddr_in serv_addr;

	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd<0)
		my_err("socket",__LINE__);

	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);
	serv_addr.sin_addr.s_addr = inet_addr(IP);

	printf("客户端启动中...\n");

	if(connect(sock_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
			my_err("connect",__LINE__);

	printf("客户端已连接至服务端\n");

	choose(menu(),sock_fd);
}


int menu(void)
{
//	fflush(stdout);
	printf("************************************\n");
	printf("/t/t1.注册\n");
	printf("/t/t2.登录\n");
	printf("/t/t3.Exit\n");
	printf("************************************\n");
	printf("请选择:\n");
	int n;
	scanf("%d",&n);
//	fflush(stdin);
	return n;
}

int local_login(int fd)
{
	char username[CHARMAX],password[CHARMAX];
	int flag;
	printf("请输入用户名:");
	scanf("%s",username);
	printf("请输入密码:");
	scanf("%s",password);
	flag = serv_login(fd,LOGIN,username,"server",password);

	if(flag==-1)
		choose(menu(),fd);
	else if(flag==0)
	{
		printf("%s登陆成功!\n",username);
		//菜单
	}

}

int serv_login(int fd,int type,char send_name[],char recv_name[],char mes[])
{
	PACK package;
	package.type = type;
	strcpy(package.data.send_name,send_name);
	strcpy(package.data.recv_name,recv_name);
	strcpy(package.data.mes,mes);

	if(my_write(fd,(void*)&package,sizeof(PACK))<0)
	{
		printf("自定义写函数错误\n");
		exit(-1);
	}
	if(my_read(fd,(void*)&package,sizeof(PACK))<0)
	{
		printf("自定义读函数错误\n");
		exit(-1);
	}
	if(package.data.mes[0]=='Y')
		return 0;
	else if(package.data.mes[0]=='N')
		return -1;
}

void choose(int n,int sock_fd)
{
	switch (n)
	{
		case 0:
			exit(0);
			break;
		case 1:
			local_login(sock_fd);
			break;
		case 2:
			//注册;
			break;
	}
}


















