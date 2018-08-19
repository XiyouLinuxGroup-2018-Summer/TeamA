#include<stdio.h> 
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include <fcntl.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include<signal.h>
#include<time.h>
#include"cJSON.h"

#define EXIT -1
#define REGISTER 1
#define LOGIN 2
#define RESPONSE 3

#define SERV_PORT 8888
#define LISTEN_Q 20
#define EPOLL_MAX 10000

#define CHARMAX 200


typedef struct datas{
	char send_name[CHARMAX];
	char recv_name[CHARMAX];
	int send_fd;
	int recv_fd;
	char mes[1024];
}DATA;

typedef struct package{
	int type;
	DATA data;
}PACK;

//服务端
int my_err(char * err,int line);
void* pack_deal(void*recv_package_t);
int my_write(int fd, void* vptr,int n);        
int my_read(int fd,void *vptr,int n);
void login_deal(PACK*recv_package_t);

//客户端
void choose(int n,int fd);
int serv_login(int fd,int type,char send_name[],char recv_name[],char* mes);
int local_login(int fd);
int menu(void);
int local_login(int fd);
MYSQL mysql;

