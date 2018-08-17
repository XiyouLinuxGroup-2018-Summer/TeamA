/*************************************************************************
	> File Name: serverview.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 14时30分01秒
 ************************************************************************/

#include<stdio.h>
#include <math.h>
#include "./cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include<fcntl.h>
#include <errno.h>
#include<mysql/mysql.h>
#include "serverfun.h"

char *IP = "127.0.0.1";     //服务器IP号
short PORT = 8080;          //通信端口号
int fd;

#define  ZH_CE 0;



/*客户端信息的结构*/
typedef struct client1{
   char name[20];
   int sock;
} Client;

typedef struct {
	int  id;                //用户id
	char ac_number[30];     //账号
	char password[30];      //用户密码
	char username[30];      //用户名
} account_t;


Client client[100];         //能同时容纳100个客户上线
int size = 0;               //存放当前在线客户端数目


int main()
{
      int nfds,i,connfd,sockfd;
      char *name;
      ssize_t n;

      struct sockaddr_in from;
      socklen_t len = sizeof(from);
      //signal(SIGINT,fa);
      printf("服务器正在初始化...\n");
      // sleep(3);
      /*创建socket描述符*/
      fd = socket(AF_INET,SOCK_STREAM,0);

      if (fd==-1) perror("socket"),exit(-1);
      /*准备通信地址*/

      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(PORT);
      addr.sin_addr.s_addr = inet_addr(IP);
      /*绑定*/
      int res = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
      if(res==-1) perror("bind"),exit(-1);
      /*监听*/
      listen(fd,100);

      printf("服务器初始化完毕！\n");

      struct epoll_event ev,events[20];

      int epfd=epoll_create(256);

      ev.data.fd=fd;

      ev.events=EPOLLIN|EPOLLET;

      epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);

      while (1) 
      {
            //等待epoll事件的发生
            nfds=epoll_wait(epfd,events,20,500);
            //处理所发生的所有事件

            //printf("初始化成功\n");
            for(i=0;i<nfds;++i)
            {
                  printf("本次共有%d个事件\n", nfds);
                  if(events[i].data.fd==fd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
                  {
                        connfd= accept(fd,(struct sockaddr*)&from,&len);
						
						/*
						枷锁
						list.add(adsf);
						发信号
						jiesuo
						*/
                        //connfd = accept(fd,(sockaddr *)&clientaddr, &clilen);
                        if(connfd<0)
                        {
                              perror("connfd<0");
                              exit(1);
                        }

                        char *str = inet_ntoa(from.sin_addr);
                        //cout << "accapt a connection from " << str << endl;
                        printf("%s 链接到了服务器\n",str);
                        //设置用于读操作的文件描述符
                        ev.data.fd=connfd;
                        //设置用于注测的读操作事件
            //ev.events=EPOLLIN|EPOLLET;
                        ev.events=EPOLLIN;
                        //ev.events=EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
                  }
    

                  else if(events[i].events&EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入。
                  {
                        
                        //cout << "EPOLLIN" << endl;
                        printf("EPOLLIN\n");
                        if ( (sockfd = events[i].data.fd) < 0)
                              continue;

                        int number,tmpnumber,tmpn;
                        recv(sockfd,&number,4,0);
                        tmpnumber=number;

                        printf("number的数值是%d\n",number);
                        name=(char*)malloc(number);
                        n = recv(sockfd,name,number,0);
                        tmpn=n;
                        tmpnumber=number-n;
                        

                        int neitmpn;
                        while(tmpnumber!=0)
                        {     //neitmpn=tmpn;                  
                              tmpn=recv(sockfd,name+tmpn,tmpnumber,0);
                              tmpnumber-=tmpn;
                        }
                        
                        if (n == 0) 
                        {
                              close(sockfd);
                              //踢用户
                              events[i].data.fd = -1;
                        }
                        name[number] = '\0';
                        //cout << "read " << name << endl;
                        //设置用于写操作的文件描述符
				printf("收到消息 ： %s\n", name);

                        analyse(name);
                        /*
                        //解析JSON
                        smg ff;
                        cJSON *user = cJSON_Parse(name);
                        ff.type=cJSON_GetObjectItem(user,"type")->valueint;
                        ff.user_ID = cJSON_GetObjectItem(user,"user_ID")->valueint;
                        strcpy(ff.ac_number,cJSON_GetObjectItem(user,"ac_number")->valuestring);
                        strcpy(ff.user_password,cJSON_GetObjectItem(user,"user_password")->valuestring);
                        strcpy(ff.user_name,cJSON_GetObjectItem(user,"user_name")->valuestring);
                        cJSON_Delete(user);
                        //free(name);
                        //printf("\n解析结果: \n id: %d\n ac_number: %s\n password: %s\n user_name: %s\n",ff.id,ff.ac_number,ff.password,ff.username);

                        database(ff.ac_number,ff.user_password,ff.user_name);
                        */

                        ev.data.fd=sockfd;
                        //设置用于注测的写操作事件
                        // sleep(1);
                     //     pthread_create(analysisThread，NULL, analysis, &name);
                        ev.events=EPOLLOUT|EPOLLET;
                        //修改sockfd上要处理的事件为EPOLLOUT
                        //epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);   
                  }


                  //貌似多余
                  /*
                  else if(events[i].events&EPOLLOUT) // 如果有数据发送
                  {
                        sockfd = events[i].data.fd;
                        write(sockfd, name, n);
                        //设置用于读操作的文件描述符

                        ev.data.fd=sockfd;
                        //设置用于注测的读操作事件

                        ev.events=EPOLLIN|EPOLLET;
                        //修改sockfd上要处理的事件为EPOLIN

                        epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
                  }
                  */
            }
      }
      closeServe();
      return 0;

}
