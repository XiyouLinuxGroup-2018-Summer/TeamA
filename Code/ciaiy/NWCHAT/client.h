#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "cJSON.h"

/* 服务器接受包的类型 */
/*  一律用正数来标识  */
#define REGISTER                1

void start(int port);
void err(char *msg, int len);

/* 客户端接受包的类型 */
/*  一律用负数来标识  */
#define REGISTER_SUCCESS        -1

/* 一些关于socket的变量 */
int saLen;
int clientSocket;
struct sockaddr_in addr;
