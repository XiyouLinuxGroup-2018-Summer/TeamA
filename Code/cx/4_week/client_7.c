#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include <fcntl.h>
#define PORT 6666
#include "cJSON.h"
#define CHAT_PRI 4
#define LOGIN 5
#define REGISTER 6
#define GET_GROUP_LIST 7
#define CREAT_GROUP 8
#define ADD_MEMBER 9
#define ADD_FRIEND 10
#define AGREE 11
#define DISAGREE 12
#define FRIEND_REPLY 13
#define NO_SPEAKING 14  //禁言　　　
#define CHAT_GROUP 15
#define SEND_FILE 16 //上传文件
#define SPEAKING  17      //解除禁言
char friend[30];
pthread_mutex_t mutex;
typedef struct package
{
    char name[20];
    char send_name[20];
    char recv_name[20];
    int send_fd;
    int recv_fd;
    char timep[20];
    char content[100];
} PACK;
typedef struct friend_info
{
    int id;
    int pid;
    int status;
} FRIEND_INFO;
MYSQL mysql;
MYSQL *res;
MYSQL row;
char *sql_str = NULL;
int getch(void)
{
    struct termios tm, tm_old;
    int fd = 0, ch;
    if (tcgetattr(fd, &tm) < 0)
    { //保存现在的终端设置
        return -1;
    }
    tm_old = tm;
    cfmakeraw(&tm); //更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
    if (tcsetattr(fd, TCSANOW, &tm) < 0)
    { //设置上更改之后的设置
        return -1;
    }
    ch = getchar();
    if (tcsetattr(fd, TCSANOW, &tm_old) < 0)
    { //更改设置为最初的样子
        return -1;
    }
    return ch;
}
/*发包函数，美滋滋*/
void add_file_size(int fd, char *pass)
{
    printf("kkk%s\n", pass);
    int m = strlen(pass) + 16;
    //char *temp_pack =(char*)malloc(sizeof(char)*m);
    char temp_pack[m]; //大小也是个坑
    temp_pack[m - 1] = '\0';
    strcpy(temp_pack + 16, pass);
    *(int *)temp_pack = strlen(pass);
    printf("lll%s\n", pass);
    send(fd, &temp_pack, m, 0);
    return;
}

void reg(int sockfd) /*注册用户信息*/
{
    //char * pass = (char*)malloc(sizeof(char)*256);
    char usrname[16];
    char passwd[8];
    char st[8];
    char m[9];
    int i, c;
    char ch;
    int id;
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", REGISTER);
    printf("please enter your id\n");
    scanf("%d", &id);
    cJSON_AddNumberToObject(json, "id", id);
    printf("Please enter your user name\n");
    scanf("%s", usrname);
    cJSON_AddStringToObject(json, "usrname", usrname);
    printf("Please enter the safe passwds(8 bits)\n");
    getchar();
    for (i = 0; i < 8; i++)
    {
        ch = getch();
        if (passwd[i] == '\n')
        {
            passwd[i] = '\0';
            break;
        }
        passwd[i] = ch;
    }
    passwd[i] = '\0';
    cJSON_AddStringToObject(json, "passwd", passwd);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
}
/*找回密码*/
void back_passwd(int sockfd)
{
    cJSON *json = cJSON_CreateObject();
    int id;
    printf("请输入你的账号\n");
    scanf("%d", &id);
    cJSON_AddNumberToObject(json, "id", id);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
}
/*删除用户*/
void delete_usr(int sockfd)
{
    cJSON *json = cJSON_CreateObject();
    int id;
    printf("please enter the id that you want to delete\n");
    scanf("%d", &id);
    cJSON_AddNumberToObject(json, "id", id);
    char *pass = cJSON_Print(json);
    add_file_size(sockfd, pass);
}
/*添加好友*/
void add_friends(int sockfd, int uid)
{
    cJSON *json = cJSON_CreateObject();
    int pid;
    cJSON_AddNumberToObject(json, "signal", ADD_FRIEND);
    cJSON_AddNumberToObject(json, "uid", uid);
    printf("please enter the id of your friend\n");
    scanf("%d", &pid);
    cJSON_AddNumberToObject(json, "pid", pid);
    char *pass = cJSON_PrintUnformatted(json);
    printf("%s\n", pass);
    add_file_size(sockfd, pass);
}
int add_friends_reply(int sockfd, char *string)
{
    printf("%s\n", string);
    cJSON *node = cJSON_Parse(string);
    printf("%d请求添加你为好友(Y or N)\n", cJSON_GetObjectItem(node, "uid")->valueint);
    char ch;
    /*do{
       ch = getchar();
       if(ch != 'Y' || ch != 'N')
         continue;
         else
         break;
   }while(1);*/
    ch = 'Y';
    // cJSON_Delete(node);
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", FRIEND_REPLY);
    cJSON_AddNumberToObject(json, "uid", cJSON_GetObjectItem(node, "uid")->valueint);
    if (ch == 'Y')
        cJSON_AddNumberToObject(json, "tip", AGREE);
    else
        cJSON_AddNumberToObject(json, "tip", DISAGREE);
    cJSON_AddNumberToObject(json, "pid", cJSON_GetObjectItem(node, "pid")->valueint);
    char *pass = cJSON_PrintUnformatted(json);
    cJSON_Delete(node);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*原始的分包函数*/
cJSON *analysis_package(int sockfd)
{
    int number;
    int m = recv(sockfd, &number, 16, 0);
    printf("%d\n", number);
    char *temp = (char *)malloc(sizeof(char) * number);
    recv(sockfd, temp, number, 0);
    printf("%s\n", temp);
    cJSON *node = cJSON_Parse(temp);
    free(temp);
    return node;
}
/*获取好友列表*/
void get_friend_list(int sockfd)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "signal", "start");
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
    printf("no\n");
    cJSON *node = analysis_package(sockfd);
    printf("%s\n", cJSON_GetObjectItem(node, "signal")->valuestring);
    cJSON_Delete(node);
}
/*创建群聊*/
void create_group(int sockfd, int id)
{
    cJSON *json = cJSON_CreateObject();
    int gid;
    char buf[30];
    cJSON_AddNumberToObject(json, "signal", CREAT_GROUP);
    printf("please enter the group id\n");
    scanf("%d", &gid);
    cJSON_AddNumberToObject(json, "gid", gid);
    printf("please enter the group name\n");
    scanf("%s", buf);
    cJSON_AddStringToObject(json, "g_name", buf);
    cJSON_AddNumberToObject(json, "id", id);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*添加群成员*/
void add_group_member(int sockfd, int uid)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", ADD_MEMBER);
    printf("please enter the id of your friend\n");
    scanf("%d", &uid);
    cJSON_AddNumberToObject(json, "id", uid);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*分析ＪＳＯＮ数组*/
void analysis_array(int sockfd)
{
    cJSON *node = analysis_package(sockfd);
    cJSON *tnode = cJSON_GetObjectItem(node, "member");
    cJSON *root = NULL;
    if (tnode->type == cJSON_Array)
    {
        cJSON_ArrayForEach(root, tnode) if (root->type == cJSON_Number)
            printf("%d\t", root->valueint);
    }
    printf("\n");
}
/*私聊*/
void chat_private(int sockfd, int id)
{
    char msg[1024];
    int pid;
    int count = 0;
    // get_friend_list(sockfd);
    while (1)
    {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "signal", CHAT_PRI);
        cJSON_AddNumberToObject(json, "send_fd", id);
        if (count == 0)
        {
            printf("please enter the id that you want to chat\n");
            scanf("%d", &pid);
        }
        cJSON_AddNumberToObject(json, "recv_fd", pid);
        if (count == 0)
            printf("please enter the msg\n");
        //fgets(msg,1024,stdin);
        scanf("%s", msg);
        cJSON_AddStringToObject(json, "content", msg);
        char *pass = cJSON_PrintUnformatted(json);
        add_file_size(sockfd, pass);
        cJSON_Delete(json);
        if (strcmp(msg, "bye") == 0)
            break;
        count++;
    }
}
/*群聊*/
void chat_group(int sockfd, int uid)
{
    int gid;
    int count = 0;
    while (1)
    {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "signal", CHAT_GROUP);
        cJSON_AddNumberToObject(json, "uid", uid);
        if (count == 0)
        {
            printf("please enter the group order\n");
            scanf("%d", &gid);
        }
        cJSON_AddNumberToObject(json, "gid", gid);
        char buf[30];
        printf("please enter the content\n");
        scanf("%s", buf);
        cJSON_AddStringToObject(json, "content", buf);
        count++;
        char *pass = cJSON_PrintUnformatted(json);
        cJSON_Delete(json);
        printf("chat_group:%s\n", pass);
        add_file_size(sockfd, pass);
        if (strcmp(buf, "bye") == 0)
            break;
    }
}
/*如果是管理员则禁言某人，否则返回错误信息*/
void group_set_no_speaking(int sockfd,int uid)
{
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",NO_SPEAKING);
    printf("请输入要禁言人的id\n");
    int id;
    scanf("%d",&id);
    cJSON_AddNumberToObject(json,"uid",id);
    printf("请输入组账号\n");
    int gid;
    scanf("%d",&gid);
    cJSON_AddNumberToObject(json,"gid",gid);
    char * pass= cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
}
/*解除禁言*/
void group_cancle_no_speaking(int sockfd,int uid)
{
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",SPEAKING);
    printf("请输入对方id\n");
    int id;
    scanf("%d",&id);
    cJSON_AddNumberToObject(json,"uid",id);
    printf("please enter the group id\n");
    int gid;
    scanf("%d",&gid);
    cJSON_AddNumberToObject(json,"gid",gid);
    char * pass= cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
}
/*上传文件*/
void send_file(int sockfd, int uid)
{
    int fd;
    char filename[40];
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", SEND_FILE);
    cJSON_AddNumberToObject(json, "uid", uid);
    printf("请输入文件路径\n");
    scanf("%s", &filename);
    if ((fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR)) == -1)
    {
        printf("文件打开失败\n");
        return;
    }
    read(fd, filename, strlen(filename));
    printf("%s", filename);
    cJSON_AddStringToObject(json, "content", filename);
    char *pass = cJSON_PrintUnformatted(json);
    printf("%s\n", pass);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*二十一世纪得分包函数*/
void *analysis_pack(void *arg)
{
    int flag = 1;
    while (1)
    {
        // pthread_mutex_lock(&mutex);
        int sockfd = *(int *)arg;
        int number;
        int m = recv(sockfd, &number, 16, 0);
        printf("ss%d,%d\n", number, m);
        if (m == 0)
            break;
        char *temp = (char *)malloc(sizeof(char) * number);
        int sf;
        if ((sf = recv(sockfd, temp, number, 0)) > 0)
        {
            printf("%s,%d\n", temp, sf);
            cJSON *node = cJSON_Parse(temp);
            int item = cJSON_GetObjectItem(node, "signal")->valueint;
            printf("lll%d\n",item);
            switch (item)
            {
            case CHAT_PRI:
                printf("%s\n", cJSON_GetObjectItem(node, "content")->valuestring);
                break;
            case ADD_FRIEND:
                flag = 0;
                strcpy(friend, temp);
                printf("ss%s", friend);
                printf("well,first hand\n");
                break;
            case FRIEND_REPLY:
                flag = 0;
                if (cJSON_GetObjectItem(node, "tip")->valueint == AGREE)
                    printf("对方已经同意添加你为好友\n");
                break;
            case CHAT_GROUP:
                printf("%d发了一条群消息\n", cJSON_GetObjectItem(node, "uid")->valueint);
                printf("%s\n", cJSON_GetObjectItem(node, "content")->valuestring);
                break;
            case SPEAKING:
                flag = 0;
                printf("您已被管理员解除禁言\n");
                break;
            case NO_SPEAKING:
                 flag = 0;
                 printf("您已被管理员禁言\n");
                 break;
            case CREAT_GROUP:
                 flag = 0;
                 printf("建群成功\n");
                 break;
            default:
                flag = 0;
                printf("好戏即将开始\n");
                break;
            }
            if (flag == 0)
                break;
        }
        // pthread_mutex_unlock(&mutex);
        free(temp);
    }
}
/*菜单选择函数*/
void menu_select(int sockfd, int id)
{
    pthread_t thid1, thid2;
    while (1)
    {
        pthread_create(&thid1, NULL, analysis_pack, (void *)&sockfd);
        printf("\t\t\t*********1.delete usr********\t\t\t\n");
        printf("\t\t\t*********2.add friends*******\t\t\t\n");
        printf("\t\t\t*********3.reply friends*******\t\t\t\n");
        printf("\t\t\t*********4.get_friend_list***\t\t\t\n");
        printf("\t\t\t*********5.chat private*******\t\t\t\n");
        printf("\t\t\t*********6.create_group*******\t\t\t\n");
        printf("\t\t\t*********7.chat group*******\t\t\t\n");
        printf("\t\t\t*********8.set_no_speaking*******\t\t\t\n");
        printf("\t\t\t*********9.cancle_no_speaking*******\t\t\t\n");
        printf("\t\t\t*********10.main_menu*******\t\t\t\n");
        int choice;
        printf("\t\t\t请输入你的选择:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            delete_usr(sockfd);
            break;
        case 2:
            add_friends(sockfd, id);
            //  sleep(100);
            //  pthread_create(&thid1, NULL, analysis_pack, (void *)&sockfd);
            break;
        case 3:
            add_friends_reply(sockfd, friend);
            // pthread_create(&thid1, NULL, analysis_pack, (void *)&sockfd);
            break;
        case 4:
            printf("Nothing is here\n");
            break;
        case 5:
            chat_private(sockfd, id);
            break;
        case 6:
            create_group(sockfd,id);
            break;
        case 7:
            chat_group(sockfd, id);        
            break;
        case 8:
           group_set_no_speaking(sockfd,id);
            break;
        case 9:
            group_cancle_no_speaking(sockfd,id);
            break;
        default:
            printf("输入错误，请重新输入\n");
            break;
        }
    }
}
/*登录函数*/
void login(int sockfd)
{
    int id, i;
    char passwd[10];
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", LOGIN);
    printf("please enter your id codes\n");
    scanf("%d", &id);
    cJSON_AddNumberToObject(json, "id", id);
    printf("please enter the name\n");
    char name[15];
    scanf("%s", name);
    cJSON_AddStringToObject(json, "name", name);
    printf("please enter the passwd\n");
    while (getchar() != '\n') //又是一个坑ｓｃａｎｆ为什么会有这么多的坑
        continue;
    for (i = 0; i < 8; i++)
        passwd[i] = getch();
    passwd[i] = '\0';
    cJSON_AddStringToObject(json, "passwd", passwd);
    char *pass = cJSON_PrintUnformatted(json);
    printf("%s\n", pass);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
    int number;
    int m = recv(sockfd, &number, 16, 0);
    printf("fff\n");
    printf("%d\n", number);
    char *temp = (char *)malloc(sizeof(char) * number);
    recv(sockfd, temp, number, 0);
    printf("%s\n", temp);
    cJSON *node = cJSON_Parse(temp);
    if (strcmp(cJSON_GetObjectItem(json, "flag")->valuestring, "login successfully!!") == 0)
        menu_select(sockfd, id);
    return;
}
/*主菜单*/
void menu_main(int sockfd)
{
    //int sockfd = (int)arg;
    printf("\t\t\t*********1.login*************\t\t\t\n");
    printf("\t\t\t*********2.register**********\t\t\t\n");
    printf("\t\t\t*********3.back_passwd*******\t\t\t\n");
    printf("\t\t\t*********4.exit**************\t\t\t\n");
    int choice;
    printf("\t\t\t请输入你的选择:");
    scanf("%d", &choice);
    //  pthread_mutex_lock(&mutex);
    switch (choice)
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
int main(int argc, char *argv[])
{
    struct sockaddr_in sever_addr;
    pthread_mutex_init(&mutex, NULL);
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("创建套接字失败\n");
        exit(1);
    }
    bzero(&sever_addr, sizeof(struct sockaddr_in));
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_port = htons(PORT);
    sever_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = connect(sockfd, (struct sockaddr *)&sever_addr, sizeof(sever_addr));
    if (ret == -1)
    {
        printf("连接失败\n");
        exit(1);
    }
    while (1)
        menu_main(sockfd);
    return 0;
}