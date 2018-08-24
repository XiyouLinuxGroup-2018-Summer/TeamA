/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月13日 星期一 14时23分13秒
 ************************************************************************/

//server.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

int sockfd;//服务器socket
int fds[100];//客户端的socketfd,100个元素，fds[0]~fds[99]
int size =100 ;//用来控制进入聊天室的人数为100以内
char *IP = "127.0.0.1";
short PORT = 8080;
typedef struct sockaddr SA;

void init();
void SendMsgToAll(char* msg);
void* service_thread(void* p);
void service();

int main(){
    init();
    service();
}
void init()
{
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    if (sockfd == -1)
	{
        perror("创建socket失败");
        exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if (bind(sockfd,(SA*)&addr,sizeof(addr)) == -1)
	{
        perror("绑定失败");
        exit(-1);
    }
    if (listen(sockfd,100) == -1)
	{
        perror("设置监听失败");
        exit(-1);
    }
}

void SendMsgToAll(char* msg)
{
    int i;
    for (i = 0;i < size;i++)
	{
        if (fds[i] != 0)
		{
            printf("sendto%d\n",fds[i]);
            send(fds[i],msg,strlen(msg),0);
        }
    }
}

void* service_thread(void* p)
{
    int fd = *(int*)p;
    printf("pthread = %d\n",fd);
    while(1)
	{
        char buf[100] = {};
        if (recv(fd,buf,sizeof(buf),0) <= 0)
		{
            int i;
            for (i = 0;i < size;i++)
			{
                if (fd == fds[i])
				{
                    fds[i] = 0;
                    break;
                }
            }
			printf("退出：fd = %dquit\n",fd);
			pthread_exit((void*)i);
        }
        //把服务器接受到的信息发给所有的客户端
        SendMsgToAll(buf);
    }
}

void service()
{
    printf("服务器启动\n");
    while(1)
	{
        struct sockaddr_in fromaddr;
        socklen_t len = sizeof(fromaddr);
        int fd = accept(sockfd,(SA*)&fromaddr,&len);
        if (fd == -1)
		{
            printf("客户端连接出错...\n");
            continue;
        }
        int i = 0;
        for (i = 0;i < size;i++)
		{
            if (fds[i] == 0)
			{
                //记录客户端的socket
                fds[i] = fd;
                printf("fd = %d\n",fd);
                //有客户端连接之后，启动线程给此客户服务
                pthread_t tid;
                pthread_create(&tid,0,service_thread,&fd);
                break;
            }

        if (size == i)
		{
            //发送给客户端说聊天室满了
            char* str = "对不起，聊天室已经满了!";
            send(fd,str,strlen(str),0); 
            close(fd);
        }
		
        }
    }
}
