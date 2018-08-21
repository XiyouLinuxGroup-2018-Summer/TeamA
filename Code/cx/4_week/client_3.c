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
#define CHAT_PRI 4
pthread_mutex_t  mutex;
typedef struct package{
    char name[20];
    char send_name[20];
    char recv_name[20];
    int send_fd;
    int recv_fd;
    char timep    [20];
    char content  [100];
}PACK;
typedef struct friend_info{
    int id;
    int pid;
    int status;
}FRIEND_INFO;
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
void delete_usr(int sockfd)
{
    cJSON * json = cJSON_CreateObject();
    int id;
    printf("please enter the id that you want to delete\n");
    scanf("%d",&id);
    cJSON_AddNumberToObject(json,"id",id);
    char *pass = cJSON_Print(json);
    add_file_size(sockfd,pass);
}
void add_friends(int sockfd)
{
    cJSON * json = cJSON_CreateObject();
    int id;
    printf("please enter the id of your friend\n");
    scanf("%d",&id);
    cJSON_AddNumberToObject(json,"id",id);
    char * pass = cJSON_PrintUnformatted(json);
    printf("%s\n",pass);
    add_file_size(sockfd,pass);
}
cJSON * analysis_package(int sockfd)
{
    int number;
    int m = recv(sockfd,&number,16,0);
    printf("%d\n",number);
    char * temp = (char *)malloc(sizeof(char)*number);
    recv(sockfd,temp,number,0);
    printf("%s\n",temp);
    cJSON*node = cJSON_Parse(temp);
    free(temp);
    return node;
}
void get_friend_list(int sockfd)
{
    cJSON * json = cJSON_CreateObject();
    cJSON_AddStringToObject(json,"signal","start");
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
    printf("no\n");
    cJSON *node = analysis_package(sockfd);
    printf("%s\n",cJSON_GetObjectItem(node,"signal")->valuestring);
    cJSON_Delete(node);
}
void analysis_array(int sockfd)
{
    cJSON * node = analysis_package(sockfd);
    cJSON * tnode  = cJSON_GetObjectItem(node,"friend");
    cJSON * root = NULL;
    if(tnode->type == cJSON_Array)
    {
        cJSON_ArrayForEach(root,tnode)
        {
            if(root->type == cJSON_Number)
                printf("%d\t",root->valueint);
        }
    }
}
void chat_private(int sockfd,int id)
{
    cJSON * json = cJSON_CreateObject();
    char msg[1024];
    int pid;
   // get_friend_list(sockfd);
    cJSON_AddNumberToObject(json,"signal",CHAT_PRI);
    cJSON_AddNumberToObject(json,"send_fd",id);
    printf("please enter the id that you want to chat\n");
    scanf("%d",&pid);
    cJSON_AddNumberToObject(json,"recv_fd",pid);
    printf("please enter the msg\n");
    scanf("%s",msg);
    cJSON_AddStringToObject(json,"content",msg);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
}
void menu_select(int sockfd,int id)
{
    printf("\t\t\t*********1.delete usr********\t\t\t\n");
    printf("\t\t\t*********2.add friends*******\t\t\t\n");
    printf("\t\t\t*********3.get_friend_list***\t\t\t\n");
    printf("\t\t\t*********4.chat private*******\t\t\t\n");
    printf("\t\t\t*********5.main_menu*******\t\t\t\n");
    int choice;
    printf("\t\t\t请输入你的选择:");
    scanf("%d",&choice);
     switch(choice)
    {
        case 1:
             delete_usr(sockfd);
             break;
        case 2:
             add_friends(sockfd);
              break;
        case 3:
              get_friend_list(sockfd);
              break;
        case 4:
              chat_private(sockfd,id);
              break;
        case 5:
            // menu_main(sockfd);
             break;
        default:
              printf("输入错误，请重新输入\n");
              break;
    }
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
    while(getchar() != '\n')              //又是一个坑ｓｃａｎｆ为什么会有这么多的坑
    continue;
    for( i = 0;i< 8;i++)
        passwd[i] = getch();
    passwd[i] = '\0';
    cJSON_AddStringToObject(json,"passwd",passwd);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
    int number;
    int m = recv(sockfd,&number,16,0);
    printf("%d\n",number);
    char * temp = (char *)malloc(sizeof(char)*number);
    recv(sockfd,temp,number,0);
    printf("%s\n",temp);
    if(strcmp(temp,"login successfully!!") == 0)
    menu_select(sockfd,id);
    return;
}
void  menu_main(int sockfd)
{
    //int sockfd = (int)arg;
    printf("\t\t\t*********1.login*************\t\t\t\n");
    printf("\t\t\t*********2.register**********\t\t\t\n");
    printf("\t\t\t*********3.back_passwd*******\t\t\t\n");
    printf("\t\t\t*********4.exit**************\t\t\t\n");
    int choice;
    printf("\t\t\t请输入你的选择:");
    scanf("%d",&choice);
    //pthread_mutex_lock(&mutex);
    switch(choice)
    {
        case 1:
             login(sockfd);
             break;
        case 2:
             reg(sockfd);
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
  //  pthread_mutex_unlock(&mutex);
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
         
          menu_main(sockfd);
         /* cJSON * node = analysis_package(sockfd);
          printf("%s\n",cJSON_GetObjectItem(node,"content")->valuestring);
          cJSON_Delete(node);*/
      //   cJSON * node = analysis_package(sockfd);
    //     printf("%s\n",cJSON_GetObjectItem(node,"signal"));
      //   printf("%s\n",cJSON_GetObjectItem(node,"content")->valuestring);
        /*pthread_t thid1,thid2;
        pthread_create(&thid1,NULL,menu,(void*)sockfd);
        pthread_create(&thid2,NULL,)*/
    }
    return 0;
}