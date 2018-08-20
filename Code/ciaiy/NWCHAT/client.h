#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "cJSON.h"

#define KEY_ESC -1
#define FRD_MAX 32
#define GRP_MAX 3
#define MEM_MAX 6

/* 服务器接受包的类型 */
/*  一律用正数来标识  */
#define REGISTER                1
#define LOGIN                   2
/* 客户端接受包的类型 */
/*  一律用负数来标识  */
#define REGISTER_SUCCESS        -1

void init(int port);
void err(char *msg, int len);
void registerID();
int getch(void);
int cJSON_ToPackage(cJSON *root, char **sendPack);
void login();

/* 一些关于socket的变量 */
int saLen;
int clientSocket;
struct sockaddr_in addr;

typedef struct frdNode {
    int userID;
    int status;
    char name[32];
}frdNode;

typedef struct grpNode {
    int groupID;
    char name[32];
    frdNode memList[MEM_MAX];
}grpNode;

/* 一些存放客户端消息变量 */
frdNode frdList[FRD_MAX];
grpNode grpList[GRP_MAX];

/* 存放自己的信息 */
    int myID;
    char myName[32];