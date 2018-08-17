#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<mysql/mysql.h>
#include<pthread.h>
#include<sys/epoll.h>
#include"cJSON.h"
#define PORT 6666
#define BACKLOG 20
#define BUF_SIZE 1024
#define NAME_SIZE  50
#define PEOP_NUM 20
#define DIGIT_SIZE 10
/*用户登录信息*/
struct usr_info{
    char usrname[20];
    char passwd[8];
};  
/*数据包信息*/
typedef struct package{
    char send_name[20];
    char recv_name[20];
    char timep    [20];
    char content  [100];
}PACK;
int usr_num;
typedef struct guy_info{
    char usrname[NAME_SIZE];
    char friends_name[NAME_SIZE];
    int friends_num;
    char group_name[NAME_SIZE];
    int group_num;
    int status;                             //表明用户状态
}GUYS_INFO;
typedef struct group_info{
    char group_name[NAME_SIZE];
    int group_guys;
    char group_guy_name[PEOP_NUM][NAME_SIZE];
}GROUP_INFO;
/*********************数据库的使用**********************/
MYSQL mysql;
MYSQL_RES * res;
MYSQL_ROW  row;
int type;                                     /*判断文件类型*/
char sql_str[100];
void my_error(const char *string,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(string);
    exit(1);
}
int connect_to_mysql(void)
{
    if(NULL == mysql_init(&mysql))
    printf("init:%s\n",mysql_error(&mysql));
    if(NULL == mysql_real_connect(&mysql,"localhost","root","CT1999","chat_room",0,NULL,0))
    {
        printf("connect:%s\n",mysql_error(&mysql));
        return 0;
    }
    printf("Connect Successfuly\n");
    return 1;
}
void save_package_to_mysql(char *string)
{
    cJSON *node = cJSON_Parse(string);
    sprintf(sql_str,"insert into msg_record(send_name,recv_name,time,content) values('%s','%s','%s','%s')",
    cJSON_GetObjectItem(node,"send_name")->valuestring, 
    cJSON_GetObjectItem(node,"recv_name")->valuestring,
    cJSON_GetObjectItem(node,"time")->valuestring,
    cJSON_GetObjectItem(node,"content")->valuestring);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if(ret !=  0)
    printf("insert:%s\n",mysql_error(&mysql));
    cJSON_Delete(node);
}
/**********************cJSON的使用*********************/
struct usr_info ff;
void check_package(char * string,int type)  /*解析数据包*/
{
        if(connect_to_mysql())
        {
            cJSON *node = cJSON_Parse(string);
            if(type == 1)
            {
                printf("%s\n",cJSON_GetObjectItem(node,"usrname")->valuestring);
                printf("%s\n",cJSON_GetObjectItem(node,"passwd")->valuestring);
                sprintf(sql_str,"insert into usr_info(name,passwd) values('%s','%s')",
                cJSON_GetObjectItem(node,"usrname")->valuestring,
                cJSON_GetObjectItem(node,"passwd")->valuestring);
                int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
                if(ret !=  0)
                      printf("insert:%s\n",mysql_error(&mysql));
            }
            cJSON_Delete(node);
        }
}
/**************************线程函数*******************************/
pthread_mutex_t send_mutex;
/*发包函数*/
void * send_package(void * arg)
{
    int client = (int)arg;
   char p[1024];
    strcpy(p,"hello,client");
    int m = strlen(p) + 4;
    strcpy(p+4,"hello,client");
    *(int*)p = strlen(p);
    send(client,&p,m,0);
    return NULL;
}
int main (int argc,char *argv[])
{
    int sockfd;
    char buf[80];
    struct sockaddr_in host_addr; 
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("创建套接字失败\n");
        my_error("socket",__LINE__);
    }
    bzero(&host_addr,sizeof(struct sockaddr_in));
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    struct epoll_event ev, events[50];
    int ret;
    if((ret = bind(sockfd,(struct sockaddr*)&host_addr,sizeof(struct sockaddr))) < 0)
    {
        printf("绑定套接字失败\n");
        my_error("bind",__LINE__);
    }
    ret = listen(sockfd,BACKLOG);
    printf("等待客户端连接.......\n");
    int epollfd = epoll_create(100);
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
    while(1)
    {
        struct sockaddr_in client_addr;
        int ret;
        int length = sizeof(client_addr);
        int num_fd = epoll_wait(epollfd,events,50,1000);
        for(int i = 0;i< num_fd;i++)
        {
            if(events[i].data.fd == sockfd)
            {
                ret = accept(sockfd,(struct sockaddr*)&client_addr,&length);
                printf("成功连接到一个客户端%s\n",inet_ntoa(client_addr.sin_addr));
                pthread_t thid1;
                pthread_create(&thid1,NULL,send_package,(void*)ret);
                pthread_join(thid1,NULL);
                ev.data.fd = ret;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd,EPOLL_CTL_ADD,ret,&ev);
            }
            else
            {
                printf("新消息\n");
                events[i].data.fd = ret;
                if(ret < 0)
                {
                    printf("添加失败\n");
                }
                int number;
                bzero(&client_addr,sizeof(buf));
                int m = recv(ret,&number,13,0);
                if(m == 0) {
                    printf("exit \n");
                    continue;
                }
                char * str = (char *)malloc(sizeof(char)*number);
                recv(ret,str,number,0);
                check_package(str,1);
            }
        }
    }
    close(sockfd);
    return 0;
}
