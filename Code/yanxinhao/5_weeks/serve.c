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

#define LOGIN			1	 
#define REGISTER		2
#define	DOWNLINE		3
#define ONLINE			4
#define FRIEND_ADD		5
#define FRIEND_ADD_RECV	6
#define EXIT		   -1


//命名:_t结尾为结构体,_p结尾为指针,否则为普通变量
//
typedef struct user_info{
		char username[20];
		char password[20];
		int statu;
		int fd;
		int friend_num;
		char friend[100][20];
}USER_INFO;

typedef struct package{
		int type;
		char send_name[20];
		char recv_name[20];
		int send_fd;
		int recv_fd;
		char words[1024];
}PACK;


//声明
void *deal_pack(void *recv_pack_p_p);
void registe(PACK *recv_pack_p);
void login(PACK *recv_pack_p);
int find_user(char *username);
void send_pack(PACK *pack_send_p);
void my_err(const char * err_string,int line);
int words_read(int fd,PACK *revc_pack_p,int len);
int words_send(int fd,PACK *send_pack,int len);
void *pack_send_thread(void *arg);
void init();








MYSQL mysql;
int listen_fd,epoll_fd;
short PORT = 1234;
USER_INFO all_user_infor[1000];
int all_user_num;
PACK all_pack_send[1000];
int all_pack_num;

pthread_mutex_t mutex;

int main()
{
	//屏蔽Ctrl+c
	//signal(SIGINT,signal_close);
	init();
	//初始化锁
	pthread_mutex_init(&mutex,NULL);
	pthread_t serve_tid;
	if(pthread_create(&serve_tid,NULL,pack_send_thread,NULL) != 0)
			my_err("pthread_create",__LINE__);

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
		printf("当前循环有%d个事件\n",evs);
		for(int i=0;i<evs;i++)
		{
			if(events[i].data.fd == listen_fd)
			{
				cli_fd = accept(listen_fd,(struct sockaddr*)&server_addr,&cli_len);
				printf("新的套接字连接请求: %d\n",cli_fd);
				//将该套接字设置后注册到epoll中
				ev.data.fd = cli_fd;
				ev.events = EPOLLIN;
				epoll_ctl(epoll_fd,EPOLL_CTL_ADD,cli_fd,&ev);
			}
			else if(events[i].events&EPOLLIN)
			{
				int ret;
				printf("******************\n");
				PACK recv_pack_t;
				ret =read(events[i].data.fd,&recv_pack_t,sizeof(PACK));
				recv_pack_t.send_fd = events[i].data.fd;
				printf("接受了%d字节\n",ret);
				//测试
				printf("\n\n\n\033[;31m ***PACK***\033[0m\n");
                printf("\033[;31m*\033[0m type     : %d\n", recv_pack_t.type);
                printf("\033[;31m*\033[0m send_name: %s\n", recv_pack_t.send_name);
                printf("\033[;31m*\033[0m recv_name: %s\n",recv_pack_t.recv_name);
                printf("\033[;31m*\033[0m mes      : %s\n", recv_pack_t.words);
                printf("\033[;31m*\033[0m send_fd   : %d\n", recv_pack_t.send_fd);
                printf("\033[;31m*\033[0m recv_fd   : %d\n", recv_pack_t.recv_fd);
                printf("\033[;31m*\033[0m send_pack_num:%d\n", all_pack_num);
                printf("\033[;31m*************\033[0m\n\n\n");

				if(ret<0)
				{
					close(events[i].data.fd);
					printf("收包读错误\n");
					continue;
				}
				else if(ret==0)
				{
					for(int j=1;j<=all_user_num;j++)
						if(events[i].data.fd==all_user_infor[j].fd)
						{
							printf("用户%s已下线\n",all_user_infor[j].username);
							all_user_infor[j].statu = DOWNLINE;
							break;
						}
					ev.data.fd = events[i].data.fd;
					epoll_ctl(epoll_fd,EPOLL_CTL_DEL,events[i].data.fd,&ev);
					close(events[i].data.fd);

					continue;
				}
				PACK* recv_pack_p = (PACK*)malloc(sizeof(PACK));
				memcpy(recv_pack_p,&recv_pack_t,sizeof(PACK));

				pthread_t tid;
				if(pthread_create(&tid,NULL,deal_pack,(void *)recv_pack_p) != 0)
                    my_err("pthread_create",__LINE__);
				pthread_join(tid,NULL);

			}
		}
	}

	return 0;

}




void *deal_pack(void *recv_pack_p_p)
{
    int i;
    PACK *recv_pack_p = (PACK *)recv_pack_p_p;
    printf("包类型：%d\n", recv_pack_p->type);

	
    switch(recv_pack_p->type)
    {
        case LOGIN:
            login(recv_pack_p);
            break;
        case REGISTER:
            registe(recv_pack_p);
            break;
       // case FRIEND_ADD:
         //   friend_add(recv_pack_p);
           // break;
       // case CHAT_ONE:
         //   send_mes_to_one(recv_pack_p);
           // break;
        case EXIT:

            break;
    }
}

void registe(PACK *recv_pack_p)
{
    char words[2];

    if(find_user(recv_pack_p->send_name)==0){
        words[0] = '1';
        strcpy(all_user_infor[++all_user_num].username,recv_pack_p->send_name);
        strcpy(all_user_infor[all_user_num].password,recv_pack_p->words);
        all_user_infor[all_user_num].fd = recv_pack_p->send_fd;


        printf(" registe success!\n");
        printf(" username:%s\n", all_user_infor[all_user_num].username);
        printf(" all_user_num:%d\n", all_user_num);
        all_user_infor[all_user_num].statu = DOWNLINE;
		char  sql[1100]; 
		sprintf(sql,"INSERT INTO userlogin(username,password) VALUES('%s','%s')",recv_pack_p->send_name,recv_pack_p->words);
		if(0 != mysql_real_query(&mysql,sql,strlen(sql)))
				printf("query:%s\n",mysql_error(&mysql));
    }
    else
		words[0] = '0';
    words[1] = '\0';
    //包信息赋值
    strcpy(recv_pack_p->recv_name,recv_pack_p->send_name);
    strcpy(recv_pack_p->send_name,"server");
    strcpy(recv_pack_p->words,words);
    recv_pack_p->recv_fd = recv_pack_p->send_fd;
    recv_pack_p->send_fd = listen_fd;

    send_pack(recv_pack_p);
    free(recv_pack_p);
}

void login(PACK *recv_pack_p)
{
    int id=0;
	char words[2];
    if((id=find_user(recv_pack_p->send_name)) == 0){
        words[0] = '1';//do not have the user
    }
    else if (all_user_infor[id].statu == ONLINE)
    {
        words[0] = '2';//user had online
    }
    else if(strcmp(all_user_infor[id].password,recv_pack_p->words)!=0){
        words[0] = '3';//password is wrong

        all_user_infor[id].fd = recv_pack_p->send_fd;//niupi

    }
    else
		words[0] = '0';
    words[1] = '\0';

    //包信息赋值
    strcpy(recv_pack_p->recv_name,recv_pack_p->send_name);
    strcpy(recv_pack_p->send_name,"server");
    strcpy(recv_pack_p->words,words);
    recv_pack_p->recv_fd = recv_pack_p->send_fd;
    recv_pack_p->send_fd = listen_fd;

    send_pack(recv_pack_p);

	usleep(1);
    if(words[0] =='0')
        all_user_infor[id].statu = ONLINE;
    free(recv_pack_p);
}



int find_user(char *username)
{

    int i;
    if(all_user_num == 0)
		return 0;
    for(i=1;i<=all_user_num;i++)
    {
        if(strcmp(all_user_infor[i].username,username) == 0)
            return i;
    }
    if(i == all_user_num+1)
        return 0;
}

void send_pack(PACK *pack_send_p)
{
    pthread_mutex_lock(&mutex);
    memcpy(&(all_pack_send[all_pack_num++]),pack_send_p,sizeof(PACK));
    pthread_mutex_unlock(&mutex);
}


void my_err(const char * err_string,int line)
{
    fprintf(stderr, "line:%d ",line);
    perror(err_string);
    exit(-1);
}


int words_read(int fd,PACK *recv_pack_p,int len)
{
    int nread = 0;
    //int nleft = len;
    while(1)
    {
        nread = read(fd,recv_pack_p,len);
        //nleft-=nread;
		if(nread==len)
				break;
    }

    return len;

}

int words_send(int fd,PACK *send_pack,int len)
{
    int nsend = 0;
    //int nleft = len;
    while(1)
    {
        nsend = write(fd,send_pack,len);
        //nleft-=nsend;
		if(nsend==len)
				break;
    }
    return len;
}



void *pack_send_thread(void *arg)
{
    int statu = DOWNLINE;
    int i,recv_fd_t,recv_fd;
    while(1)
    {
        pthread_mutex_lock(&mutex);//线程锁

        statu = DOWNLINE;
        for(i = all_pack_num-1;i>=0;i--)
        {
            for(int j =1; j <= all_user_num ;j++)
            {

                if(strcmp(all_pack_send[i].recv_name,all_user_infor[j].username) == 0)
                {
                    statu = all_user_infor[j].statu;
                    if(statu == ONLINE)
                        recv_fd = all_user_infor[j].fd;
                    break;
                }
            }
            //上线，则发送包
            if(statu == ONLINE || all_pack_send[i].type == LOGIN || all_pack_send[i].type == REGISTER)
            {
                if(statu == ONLINE)
                    recv_fd_t = recv_fd;
                else
                    recv_fd_t = all_pack_send[i].recv_fd;


				printf("测试4send_fd:%d\n",recv_fd_t);	
                if(send(recv_fd_t,&all_pack_send[i],sizeof(PACK),0) < 0){
                    my_err("send",__LINE__);
                }
                all_pack_num-- ;

                for(int j = i;j<=all_pack_num&&all_pack_num>0;j++)
                {
                    all_pack_send[j] = all_pack_send[j+1];
                }
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(10);
    }
}


void init()
{
		if(NULL == mysql_init(&mysql))
				printf("init mysql failed\n");

		if(NULL == mysql_real_connect(&mysql,"localhost","debian-sys-maint","jOE9r7pWBUZLvHIY","chatroom",0,NULL,0))
				printf("connect %s\n",mysql_error(&mysql));

		char * sql = "SELECT username,password FROM userlogin";
		if(0 != mysql_real_query(&mysql,sql,strlen(sql)))
				printf("query:%s\n",mysql_error(&mysql));
		MYSQL_RES * res = mysql_store_result(&mysql);
		MYSQL_ROW row;
		while((row = mysql_fetch_row(res)))
		{
			strcpy(all_user_infor[++all_user_num].username,row[0]);
			strcpy(all_user_infor[all_user_num].password,row[1]);
		}
		mysql_free_result(res);
}


void frined_add(PACK* recv_pack_p)
{
	int ids,idf,flag=0;
	if((ids = find_user(recv_pack_p->recv_name))>0)
	{
		for(int i=1;i<=all_user_infor[ids].friend_num;i++)
		{
			if(strcmp(all_user_infor[ids].friend[i],recv_pack_p->send_name)==0)
			{
				flag = 1;
				printf("该好友已存在!\n");
				recv_pack_p->words[0] = '1';
				recv_pack_p->words[1] = '\0';
				strcpy(recv_pack_p->recv_name,recv_pack_p->send_name);
				recv_pack_p->recv_fd = recv_pack_p->send_fd;
				strcpy(recv_pack_p->send_name,"server");
			}
		}
	}
	else
	{
		printf("没有这个人\n");
		recv_pack_p->words[0] = '0';
		recv_pack_p->words[1] = '\0';
		strcpy(recv_pack_p->recv_name,recv_pack_p->send_name);
		recv_pack_p->recv_fd = recv_pack_p->send_fd;
		strcpy(recv_pack_p->send_name,"server");
	}
	if(flag ==0 )
	{
		printf("请求发送中...\n");
		recv_pack_p->recv_fd = all_user_infor[ids].fd;
	}
	send_pack(recv_pack_p);
}

























