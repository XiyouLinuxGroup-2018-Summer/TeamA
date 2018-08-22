#include <mysql/mysql.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "cJSON.h"

typedef struct analysisArg
{
    cJSON *data;
    int fd;
} analysisArg_t;

void start(int port);
void setNonblock(int serverSocket);
void *analysis(void *data);
void registerID(cJSON *root, int fd);
void err(char *msg, int len);
int cJSON_ToPackage(cJSON *root, char **sendPack);
void addSendQue(cJSON *data);
void sendInitInfo(int userID);
void login(cJSON *root, int fd);
void addFrd(cJSON *root);
void retAddFrd(cJSON *root);
void sendFrdOnline(int userID);

/* 封装的数据库函数 */
void serr(MYSQL *sql, const char *msg, int line);
MYSQL_RES *sql_run(MYSQL *sql, int flag, const char *sqlMsg);
int sql_get_userID(int dataNum);
void sql_add_onlineList(int ID, int fd);
int sql_is_register(void);
int sql_add_userID(int ID, char *passwd, char *name);
void sql_into_onlineList(int ID, int fd);
int sql_is_online(int ID);
char *sql_get_name(int ID);
int sql_verify_passwd(cJSON *root, int userID);
int sql_get_frdList(cJSON *arr, int userID);
int sql_get_grpList(cJSON *arr, int userID);
int sql_get_status(int userID, int groupID, int ctlID);
cJSON *sql_get_memList(int grpID);
void sql_be_frd(int AID, int BID);
cJSON *sql_get_info(int userID, int groupID, int ctlID);
int sql_get_onlineFrd(int userID, int groupID, int **arr);
int sql_get_ID_by_fd(int fd);

#define UNBLOCK 0
#define BLOCK 1
#define SUCCESS 1
#define FAILED 0

/* 服务器的一些宏参数 */
#define EPOLL_WAIT_MAX 10000
#define SERVER_WAIT_MAX 10000
#define FRD_MAX 32
#define GRP_MAX 3
#define MEM_MAX 6

/* 服务器接受包的类型 */
/*  一律用正数来标识  */
#define REGISTER 1
#define LOGIN 2
#define REQUEST_ADD_FRD 3
#define RETURN_ADD_FRD 4
#define PRIVATE_MSG 5

/* 客户端接受包的类型 */
/*  一律用负数来标识  */
#define REGISTER_SUCCESS -1
#define INITFRD -2
#define INITGRP -3
#define FRESHFRD -4
/* 一些使用数据库的变量 */
MYSQL sql;
MYSQL_ROW sqlRow;
MYSQL_RES *sqlRes;
char sqlMsg[512];

/* 一些关于服务器的变量 */
int epfd;
int serverSocket;
struct sockaddr_in serverAddr;
struct epoll_event ev, events[EPOLL_WAIT_MAX];