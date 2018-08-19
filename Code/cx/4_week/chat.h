#ifndef __STUDENT__H__
#define __STUDENT__H__
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
#define USR_NUM 20
pthread_mutex_t mutex;
/*用户登录信息*/
struct usr_info{
    char usrname[20];
    char passwd[8];
    int root;
    int status;
    int sockfd;
};  
struct usr_info usr[USR_NUM];
int usr_count;
/*数据包信息*/
typedef struct package{
    char name[20];
    char send_name[20];
    char recv_name[20];
    int send_fd;
    int recv_fd;
    char timep    [20];
    char content  [100];
}PACK;
typedef struct group_info{
    char group_name[NAME_SIZE];
    int group_guys;
    char group_guy_name[PEOP_NUM][NAME_SIZE];
}GROUP_INFO;
#endif