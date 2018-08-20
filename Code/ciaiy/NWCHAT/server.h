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

void start(int port);
void setNonblock(int serverSocket);
void analysis(cJSON *root, int fd);
void registerID(cJSON *root, int fd);
void err(char *msg, int len);
void serr(MYSQL *sql, const char *msg, int line);
MYSQL_RES* sql_run(MYSQL *sql, int flag, const char *sqlMsg);
int cJSON_ToPackage(cJSON *root, char **sendPack);
void *addSendQue(void *data);
int sql_get_userID(int dataNum); 
void sql_add_onlineList(int ID, int fd);
int sql_is_register(void);
int sql_add_userID(int ID, char *passwd, char *name);
void sql_into_onlineList(int ID, int fd);
void login(cJSON *root, int fd);
int sql_is_online(int ID);
char *sql_get_name(int ID);
int sql_verify_passwd(cJSON *root, int userID);

/* 服务器的一些宏参数 */
#define EPOLL_WAIT_MAX 10000
#define SERVER_WAIT_MAX 10000

/* 服务器接受包的类型 */
/*  一律用正数来标识  */
#define REGISTER                1
#define LOGIN                   2  
/* 客户端接受包的类型 */
/*  一律用负数来标识  */
#define REGISTER_SUCCESS        -1


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