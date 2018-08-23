#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/epoll.h>
#include <iostream>
#include <vector>
#include <errno.h>
#include <stdlib.h>
#include <map>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "cJSON.h"

#define KEY_ESC -1
#define UNBLOCK 0
#define BLOCK 1
#define FRD_MAX 32
#define GRP_MAX 3
#define MEM_MAX 6
#define INITING 0
#define INITSUCCESS 1
#define PRIVATECHATING 2

/* 服务器接受包的类型 */
/*  一律用正数来标识  */
#define REGISTER 1
#define LOGIN 2
#define REQUEST_ADD_FRD 3
#define RETURN_ADD_FRD 4
#define PRIVATE_MSG 5
#define BLOCK_FRD 6
#define UNBLOCK_FRD 7
#define GROUP_MSG 8

/* 客户端接受包的类型 */
/*  一律用负数来标识  */
#define REGISTER_SUCCESS -1
#define INITFRD -2
#define INITGRP -3
#define FRESHFRD -4
#define FRESH_GRP_MEM -5

void start(char *ch_addr, char *ch_port);
void init(int port);
void err(char *msg, int len);
void registerID();
int getch(void);
int cJSON_ToPackage(cJSON *root, char **sendPack);
void showNotice();
void login();
void addFrd(void);
void analysis(cJSON *root);
void initFrd(cJSON *root);
void *recvFun(void *arg);
cJSON *recvPack(void);
void initGrp(cJSON *root);
void returnRequest(int recvID, int sign, int ret);
void frdFun(void);
void freshfrd(cJSON *root);
void sendMsg(int ctlID);
void ctlBlockFrd(int ctlID, int flag);
/* 一些关于socket的变量 */
int saLen;
int clientSocket;
struct sockaddr_in addr;

typedef struct frdNode
{
    int userID;
    int status;
    int online;
    char name[32];
}frdNode;

typedef struct grpNode
{
    int groupID;
    char name[32];
    frdNode memList[MEM_MAX];
} grpNode;

pthread_rwlock_t statusLock;
pthread_rwlock_t msgBoxLock;
int status = INITING;

/* 一些存放客户端信息 */
frdNode frdList[FRD_MAX];
grpNode grpList[GRP_MAX];

/* 存放自己的信息 */
int myID;
char myName[32];
