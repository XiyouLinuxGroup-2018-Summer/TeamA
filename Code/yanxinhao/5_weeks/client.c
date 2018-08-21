#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <termios.h>

#define LOGIN                   1
#define REGISTER				2
#define DOWNLINE				3 
#define ONLINE					4
#define FRIEND_ADD              5
#define FRIEND_ADD_RECV			6
#define EXIT                   -1


#define MAX_CHAR 20


typedef struct user_infor{
    char        username    [20];
    int         friends_num;
	char		friends[100][20];
    char        group[10][20];
    int         group_num;
}USER_INFO;

typedef struct datas{
	int type;
    char send_name[20];
    char recv_name[20];
    int  send_fd;
    int  recv_fd;
    char  words[1024];
}PACK;


//声明
int login(void);
void registe(void);
int login_menu(void);
int words_read(int fd,PACK *revc_pack_p,int len);
int words_send(int fd,PACK *send_pack_p,int len);
void my_err(const char * err_string,int line);



USER_INFO my_infor;
int new_friend_num;
PACK new_friend_t[100];

pthread_mutex_t  mutex;
char *IP = "127.0.0.1";
short PORT = 1234;
typedef struct sockaddr SA;
int all_print_mes_num;
int sock_fd;


int main()
{

	printf("client start...\n");
    sock_fd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(PORT);
    sock_addr.sin_addr.s_addr = inet_addr(IP);
    if(connect(sock_fd,(SA*)&sock_addr,sizeof(sock_addr))<0){
        printf("connect failed!\n");
        exit(-1);
    }
    printf("connect success!\n");

    if(login_menu() == 0)
        return 0;

   // pthread_t pid_deal_statu,pid_recv;
    //pthread_create(&pid_deal_statu,NULL,deal_statu,NULL);
    //pthread_create(&pid_recv,NULL,clien_recv_thread,NULL);
	printf("welcome %s to my chatroom!\nBye~\n",my_infor.username);
	

}




int login_menu(void)
{
    int choice;
    do
    {
        printf("***************************\n");
        printf("\t1.login    \n");
        printf("\t2.register    \n");
        printf("\t0.exit	\n");
        printf("\t*************************\n");
        printf("\tplease input your choice:");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
                if(login() == 1)
                    return 1;
                break;
            case 2:
                registe();
                break;
            default:
                break;
        }
    }while(choice!=0);
    return 0;
}


int login(void)
{
    char username [20];
    char password [20];
    printf("please input username:\n");
    scanf("%s",username);
    printf("please input password:\n");
    scanf("%s",password);

	PACK send_login_t;
	send_login_t.type = LOGIN;
	strcpy(send_login_t.send_name,username);
	strcpy(send_login_t.words,password);
	strcpy(send_login_t.recv_name,"server");
	send_login_t.recv_fd = sock_fd;

    if(words_send(sock_fd,&send_login_t,sizeof(PACK)) < 0)
        my_err("words_send",__LINE__);
	if(words_read(sock_fd,&send_login_t,sizeof(PACK)) < 0)
		my_err("words_read",__LINE__);

    int flag = send_login_t.words[0] - 48;

    if(flag ==  1){
        printf("无该用户!\n");
        return 0;
    }
    if(flag ==  2 ){
        printf("该用户已经上线\n");
        return 0;
    }
    if(flag == 3) {
        printf("password is not right!\n");
        return 0;
    }
    strcpy(my_infor.username,username);
    printf("login success!\n");
    return 1;
}

void registe(void)
{
    int flag = 0;
    flag = REGISTER;
    char username[MAX_CHAR];
    char password[MAX_CHAR];

    printf("please input new username:\n");
    scanf("%s",username);
    printf("please input your password:\n");
    scanf("%s",password);

    PACK send_registe_t;
	send_registe_t.type = REGISTER;
	strcpy(send_registe_t.send_name,username);
	strcpy(send_registe_t.recv_name,"server");
	strcpy(send_registe_t.words,password);
	send_registe_t.recv_fd = sock_fd;
	if(words_send(sock_fd,&send_registe_t,sizeof(PACK)) < 0)
        my_err("words_send",__LINE__);
	printf("ar\n");
	if(words_read(sock_fd,&send_registe_t,sizeof(PACK)) < 0)
		my_err("words_read",__LINE__);
	printf("ne\n");

    if(send_registe_t.words[0] - 48)
        printf("register success,please login again!\n");
    else
        printf("username had live!\n");
}

int words_read(int fd,PACK *recv_pack_p,int len)
{
    int nread = 0;
    //int nleft = len;
    while(1)
    {
        nread = read(fd,recv_pack_p,len);
        //nleft-=nread;
		if(nread==len)
			break;
    }

    return len;

}

int words_send(int fd,PACK *send_pack_p,int len)
{
    int nsend = 0;
  //int nleft = len;
    while(1)
    {
        nsend = write(fd,send_pack_p,len);
        //nleft-=nsend;
		if(nsend==len)
				break;
    }
    return len;
}
void my_err(const char * err_string,int line)
{
    fprintf(stderr, "line:%d ",line);
    perror(err_string);
    exit(-1);
}

void main_menu(void)
{
	printf("\033[2J");
	printf("********************************\n");
	printf("*******welcome to newyork*******\n");
	printf("********************************\n");
	printf("\t1.新朋友\n");
	printf("\t2.添加好友\n");
//	printf("\t3.私聊\n");
	printf("********************************\n");
	int n;
	printf("\tplease input your chioce:\n");
	scanf("%d",&n);
	//fflush(stdin);
	switch (n)
	{
		case 1:
			//	new_friend();
				break;
		case 2:
			//	friend_add();
				break;
	}
}

void friend_add()
{
	printf("\033[2J");
	char username[20];
	printf("请输入要添加的用户名:\n");
	scanf("%s",username);
	PACK send_pack_t;
	send_pack_t.type = FRIEND_ADD;
	strcpy(send_pack_t.recv_name,username);
	strcpy(send_pack_t.send_name,my_infor.username);
	printf("请输入验证消息:\n");
	scanf("%s",send_pack_t.words);
	if(words_send(sock_fd,&send_pack_t,sizeof(PACK))<=0)
			my_err("words_send",__LINE__);
	printf("请求发送成功!\n");

}

void re_friend_add(PACK*recv_pack_p)
{
		
}

void * deal_pack(PACK*recv_pack_p)
{
	switch (recv_pack_p->type)
	{
		case FRIEND_ADD:
			;
			break;
		case FRIEND_ADD_RECV:
			;
			break;
	}
}

void * recv_pack_thread()
{
		PACK recv_pack_t;
		int ret;
		while(1)
		{
			ret = words_read(sock_fd,&recv_pack_t,sizeof(PACK));
			if(ret<=0)
					my_err("words_read",__LINE__);
			
		}
}








