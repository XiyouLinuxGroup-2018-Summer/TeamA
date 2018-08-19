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
#include<mysql/mysql.h>
#define PORT 6666
#include"cJSON.h"
#define DIGIT_SIZE  10
typedef struct package{
    char name[20];
    char send_name[20];
    char recv_name[20];
    int send_fd;
    int recv_fd;
    char timep    [20];
    char content  [100];
}PACK;
MYSQL mysql;
MYSQL * res;
MYSQL row;
char* sql_str = NULL;
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
/*发包函数，美滋滋*/
void  add_file_size(int fd,char * pass)
{
    int m = strlen(pass) + 16;
    //char *temp_pack =(char*)malloc(sizeof(char)*m);
    char temp_pack[m];     //大小也是个坑
    strcpy(temp_pack+16,pass);
    *(int*)temp_pack = strlen(pass);
    send(fd,&temp_pack,m,0);
    return;
}
void  reg(int sockfd)                 /*注册用户信息*/
{
    //char * pass = (char*)malloc(sizeof(char)*256);
	char usrname [16];
	char passwd[8];
	char st[8];
	char m[9];
	int i,c;
	char ch;
    int id;
    cJSON * json = cJSON_CreateObject();
    printf("please enter your id\n");
    scanf("%d",&id);
    cJSON_AddNumberToObject(json,"id",id);
    printf("Please enter your user name\n");
    scanf("%s",usrname);
    cJSON_AddStringToObject(json,"usrname",usrname);
    printf("Please enter the safe passwds(8 bits)\n");
    getchar();
    for(i = 0;i<8;i++)
    {
        ch  = getch();
        if(passwd[i] == '\n')
        {
        passwd[i] = '\0';
        break;
        }
        passwd[i] = ch;
    }
    passwd[i] = '\0';
    cJSON_AddStringToObject(json,"passwd",passwd);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);		
}
void login(int sockfd)
{
    int id,i;
    char passwd [10];
    cJSON * json = cJSON_CreateObject();
    printf("please enter your id codes\n");
    scanf("%d",&id);
    cJSON_AddNumberToObject(json,"id",id);
    printf("please enter the passwd\n");
    for( i = 0;i< 8;i++)
    {
        char ch = getch();
        if(passwd[i] == '\n')
        {
            passwd[i] = '\0';
            break;
        }
        passwd[i] = ch;
    }
    passwd[i] = '\0';
    cJSON_AddStringToObject(json,"passwd",passwd);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
}
void back_passwd(int sockfd)
{
    cJSON * json = cJSON_CreateObject();
    int id;
    printf("请输入你的账号\n");
    scanf("%d",&id);
    cJSON_AddNumberToObject(json,"id",id);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
}
void chat_private(int sockfd,PACK * msg)
{
    printf("\t\t请输入要发送的对象:\n");
    fgets(msg->recv_name,20,stdin);
    int len_name = strlen(msg->recv_name);
    msg->recv_name[len_name - 1] = '\0';
    cJSON * json = cJSON_CreateObject();
    cJSON_AddStringToObject(json,"recv_name",msg->recv_name);
    printf("请输入要发送的内容\n");
    fgets(msg->content,100,stdin);
    len_name = strlen(msg->content);
    msg->content[len_name - 1] = '\0';
    cJSON_AddStringToObject(json,"content",msg->content);
    strcpy(msg->send_name,msg->name);
    cJSON_AddStringToObject(json,"send_name",msg->send_name);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
} 
int menu(int sockfd)
{
    printf("\t\t\t*********1.login*************\t\t\t\n");
    printf("\t\t\t*********2.register**********\t\t\t\n");
    printf("\t\t\t*********3.back_passwd**************\t\t\t\n");
    printf("\t\t\t*********4.exit**************\t\t\t\n");
    int choice;
    printf("\t\t\t请输入你的选择:");
    scanf("%d",&choice);
    char *p = NULL;
    switch(choice)
    {
        case 1:
             login(sockfd);
             break;
        case 2:
             reg(sockfd);
             back_passwd(sockfd);
              break;
        case 3:
              back_passwd(sockfd);
              break;
        case 4:
             printf("退出中，请稍后......\n");
             sleep(4);
             exit(0);
             break;
        default:
              printf("输入错误，请重新输入\n");
              exit(1);
              break;
    }
    
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
          menu(sockfd);
          int number;
          int m = recv(sockfd,&number,16,0);
         // char *p = (char*)malloc(sizeof(char)*number);
         /****************************************/
          char p[number];
          bzero(&p,number);
          recv(sockfd,p,number,0);
          cJSON * node = cJSON_Parse(p);
          printf("%s\n",cJSON_GetObjectItem(node,"passwd")->valuestring);
        /*******************************************/
         // free(p);
       // bzero(&sever_addr,sizeof(p));
       // chat_private(sockfd,&ff);
       /* pthread_t thid;
        pthread_create(&thid,NULL,recv_package,(void*)sockfd);
        pthread_join(thid,NULL);*/
    }
    return 0;
}