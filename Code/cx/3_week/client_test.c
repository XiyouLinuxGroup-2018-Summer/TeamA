#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<termios.h>
#include<pthread.h>
#define PORT 6666
#include"cJSON.h"
#define DIGIT_SIZE  10
int getch(void)
{
     struct termios tm, tm_old;
     int fd = 0, ch;

     if (tcgetattr(fd, &tm) < 0) {                  //保存现在的终端设置
          return -1;
     }

     tm_old = tm;
     cfmakeraw(&tm);                                //更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
     if (tcsetattr(fd, TCSANOW, &tm) < 0) {          //设置上更改之后的设置
          return -1;
     }

     ch = getchar();
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {      //更改设置为最初的样子
          return -1;
     }

     return ch;
}
char *  login(void)                 /*注册用户信息*/
{
    //char * pass = (char*)malloc(sizeof(char)*256);
	char usrname[16];
	char passwd[8];
	char st[8];
	char m[9];
	int i,c;
	char ch;
    cJSON * json = cJSON_CreateObject();
    printf("Please enter your user name\n");
    scanf("%s",usrname);
    cJSON_AddStringToObject(json,"usrname",usrname);
    printf("Please enter the safe passwds(8 bits)\n");
    getchar();
    for(i = 0;i<8;i++)
    {
        ch  = getch();
        if(passwd[i] == '\x0d')
        {
        passwd[i] = '\0';
        break;
        }
        passwd[i] = ch;
    }
    passwd[i] = '\0';
    cJSON_AddStringToObject(json,"passwd",passwd);
    //strcpy(pass,cJSON_PrintUnformatted(json));
    char *pass = cJSON_PrintUnformatted(json);
    return pass;		
}
void * recv_package(void * arg)
{
    int sever = (int)arg;
    int number = 0;
    recv(sever,&number,4,0);
    char *p = (char*)malloc(sizeof(char)*number);
    recv(sever,p,number,0);
    printf("%s\n",p);
    free(p);
    return NULL;
}
int main (int argc,char * argv[])
{
    struct sockaddr_in sever_addr;
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("创建套接字失败\n");
        exit(1);
    }
    bzero(&sever_addr,sizeof(struct sockaddr_in));
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_port = htons(PORT);
    sever_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = connect(sockfd,(struct sockaddr*)&sever_addr,sizeof(sever_addr));
    if(ret == -1)
    {
        printf("连接失败\n");
        exit(1);
    }
    while(1)
    {
        char *p = (char*)malloc(sizeof(char)*80);
        bzero(&sever_addr,sizeof(p));
        p = login();
        int len = strlen(p) + 13;
        char temp_pack[len];
        strcpy(temp_pack+13,p);
        *(int*)temp_pack = strlen(p);
        free(p);
        pthread_t thid;
        send(sockfd,&temp_pack,len,0); //巨坑
        pthread_create(&thid,NULL,recv_package,(void*)sockfd);
        pthread_join(thid,NULL);
    }
    return 0;
}