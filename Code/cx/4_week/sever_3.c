#include "student.h"
extern pthread_mutex_t mutex;
extern int usr_number;
extern int uid;
extern struct usr_info usr[USR_NUM];
//extern FRIEND_INFO * ff = NULL;
/*********************数据库的使用**********************/
MYSQL mysql;
int type; /*判断文件类型*/
char sql_str[100];
void my_error(const char *string, int line)
{
    fprintf(stderr, "line:%d", line);
    perror(string);
    exit(1);
}
int connect_to_mysql(void)
{
    if (NULL == mysql_init(&mysql))
        printf("init:%s\n", mysql_error(&mysql));
    if (NULL == mysql_real_connect(&mysql, "localhost", "root", "CT1999", "chat_room", 0, NULL, 0))
    {
        printf("connect:%s\n", mysql_error(&mysql));
        return 0;
    }
    printf("ｃｏｎｎｃｅｔ\n");
    return 1;
}
/***********************快递员********************************/
int add_file_size(int fd, char *pass) //防止粘包现象的发生
{
    int m = strlen(pass) + 16;
    //  char *temp_pack =(char*)malloc(sizeof(char)*m);
    char temp_pack[m]; //大小也是个坑
    strcpy(temp_pack + 16, pass);
    *(int *)temp_pack = strlen(pass);
    printf("%s\n", pass);
    if(send(fd, &temp_pack, m, 0) <= 0)
      return 0;
    // free(temp_pack);
    return 1;
}
/*保存数据到数据库*/
void save_package_to_mysql(char *string)
{
    cJSON *node = cJSON_Parse(string);
    sprintf(sql_str, "insert into pack_info(send_fd,recv_fd,content,time) values('%d','%d',%s','%s')",
            cJSON_GetObjectItem(node, "send_fd")->valueint,
            cJSON_GetObjectItem(node, "recv_fd")->valueint,
            cJSON_GetObjectItem(node, "content")->valuestring,
            cJSON_GetObjectItem(node, "time")->valuestring);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
        printf("insert:%s\n", mysql_error(&mysql));
    cJSON_Delete(node);
}
/*判断重复用户*/
int find_usr(int id)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql_str, "select * from usr_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("find:%s\n", mysql_error(&mysql));
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m = 1;
    if (row)
        m = 0;
    mysql_free_result(res);
    return m;
}
/*修改用户状态*/
int modify_status(int id, int status)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "update usr_info set status='%d' where id='%d'", status, id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("check_status:%s\n", mysql_error(&mysql));
        return 0;
    }
    printf("更新成功\n");
    return 1;
}
/*密码找回*/
char *back_passwd(int id) /*粗糙版本*/
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *str = (char *)malloc(sizeof(char) * 10);
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from usr_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("back_passwd:%s\n", mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    if (row)
        strcpy(str, row[2]);
    mysql_free_result(res);
    printf("%s\n", str);
    return str;
}
/*验证登录*/
int check_login_passwd(int id, char *passwd)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from usr_info where (id = '%d')", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("check_login:%s\n", mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m = 0;
    if (row == NULL) //不是很清真
        m = 0;
    else
    {
        if (strcmp(passwd, row[2]) == 0)
            m = 1;
    }
    printf("%d\n", m);
    mysql_free_result(res);
    return m;
}
/*判断用户是否存在*/
int check_usr_exist(int id)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from usr_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("check_use_exist:%s\n", mysql_error(&mysql));
        return 0;
    }
    int m;
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    if (row)
        m = 1;
    else
        m = 0;
    mysql_free_result(res);
    return m;
}
/*添加用户*/
int add_usr(int id, char *name, char *passwd)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "insert into usr_info  values ('%d','%s','%s','0','0')", id, name, passwd);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("add_usr:%s\n", mysql_error);
        return 0;
    }
    return 1;
}
/*删除用户*/
int delate_usr(int id)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "delete  from usr_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("delate_usr:%s\n", mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/*添加好友*/
int add_friends(int id, int pid, int status, int flag)
{
    bzero(&sql_str, 100);
    uid = 2;
    id = uid;
    if (check_usr_exist(pid))
    {
        if (flag)
        {
            printf("%d\n", id);
            sprintf(sql_str, "insert into friend_info values ('%d','%d','%d')", id, pid, status);
            int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
            if (ret != 0)
            {
                printf("add_friend:%s\n", mysql_error(&mysql));
                return 0;
            }
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}
/**********************获取好友列表*********************/
int get_friend_list(int id,int sockfd,FRIEND_INFO * ff)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    ff = (FRIEND_INFO *)malloc(sizeof(FRIEND_INFO)*20);
    sprintf(sql_str, "select * from friend_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("get_friend_info:%s\n", mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    
    cJSON * json = cJSON_CreateObject();
    cJSON * friend;
    int i = 0;
    while((row = mysql_fetch_row(res)) != NULL && i< usr_number)
    {

        cJSON_AddItemToObject(json,"friend",friend = cJSON_CreateArray());
        ff[i].id = atoi(row[0]);
        cJSON_AddItemToArray(friend,cJSON_CreateNumber(ff[i].id));
        ff[i].pid = atoi(row[1]);
        cJSON_AddItemToArray(friend,cJSON_CreateNumber(ff[i].pid));
        ff[i].status = atoi(row[2]);
        cJSON_AddItemToArray(friend,cJSON_CreateNumber(ff[i].status));  
        i++;
        char *pass = cJSON_PrintUnformatted(json);
        add_file_size(sockfd, pass);
    }
    return 1;
}
/*获得好友的套接字*/
int get_guys_sockfd(int recv_id)
{
    int sockfd;
    for(int i = 0;i< usr_number;i++)
    {
        if(usr->id == recv_id)
            sockfd = usr->sockfd;
    }
    return sockfd;
}
/*梦寐中的私聊（测试版）*/
int chat_private(int sockfd,char * string)
{
    int send_fd,recv_fd;
    char save_time[20],msg[100];
    cJSON * node = cJSON_Parse(string);
    send_fd = cJSON_GetObjectItem(node,"send_fd")->valueint;
    recv_fd = cJSON_GetObjectItem(node,"recv_fd")->valueint;
    time_t timep;
    time(&timep);
    strcpy(save_time,ctime(&timep));
    int len = strlen(save_time);
    save_time[len - 1] = '\0';
    cJSON_AddStringToObject(node,"time",save_time);
    char * pass  = cJSON_PrintUnformatted(node);
    if(record_private(send_fd,recv_fd,pass,1))
        printf("记录保存成功\n");
    else
    {
        printf("保存失败\n");
        return 0;
    }
    int f_sock = get_guys_sockfd(recv_fd);
    add_file_size(f_sock,pass);
    cJSON_Delete(node);
    return 1;
}
/**********************私聊记录保存(离线还是在线)*********************/
int record_private(int send_fd, int recv_fd, char *msg, int status)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "insert into record_private  values('%d','%d','%s','%d')", send_fd, recv_fd, msg, status);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("record_private:%s\n", mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/*获得好友的在线状态*/
int get_friend_status(int id,int status,int sockfd)
{
    if(status)
    {
        for(int i = 0;i< usr_number;i++)
        {
            if(id == usr->id)
             usr->sockfd = sockfd;
            if(modify_status(id,status))
             return 1;
             /*一首凉凉送给自己*/

        }
    }
}
/*发送给在线好友*/
int send_online(int id,int status)
{
    int sockfd;
    FRIEND_INFO ff;
    get_friend_list(id,status,&ff);
    for(int i = 0;i< usr_number;i++)
    {
        if(status)
        {
            sockfd = get_guys_sockfd(id);
            if(sockfd == -1)
                return 0;
            cJSON * json = cJSON_CreateObject();
            cJSON_AddNumberToObject(json,"signal",SEND_LINE);
            cJSON_AddNumberToObject(json,"pid",id);
            cJSON_AddNumberToObject(json,"status",status);
            char * pass = cJSON_PrintUnformatted(json);
            cJSON_Delete(json);
            add_file_size(sockfd,pass);
        }
    }
}
/*发送离线消息*/
int send_logout_out_message(int sockfd)
{

}
/**********************群聊记录*************************************/
int record_group(int group_fd, char *msg)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "insert into record_group  values('%d','0','%s','0')", group_fd, msg);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("record_private:%s\n", mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/**********************注册********************/
void reg(int client, char *string)
{
    cJSON *node = cJSON_Parse(string);
    if (connect_to_mysql())
    {
        printf("%s\n", cJSON_Print(node));
        int m = cJSON_GetObjectItem(node, "id")->valueint;
        if (find_usr(m))
        {
            printf("hello\n");
           /* bzero(&sql_str, 100);
            sprintf(sql_str, "insert into usr_info(id,name,passwd) values(%d,'%s','%s')",
                    cJSON_GetObjectItem(node, "id")->valueint,
                    cJSON_GetObjectItem(node, "usrname")->valuestring,
                    cJSON_GetObjectItem(node, "passwd")->valuestring);
            int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
            if (ret != 0)
            {
                printf("insert:%s\n", mysql_error(&mysql));
                char str[50];
                bzero(&str, 50);
                strcpy(str, "该用户已被注册，请重新输入");
                add_file_size(client, str);
            }
            else
            {*/
            if(add_usr(m,cJSON_GetObjectItem(node,"name")->valuestring,
            cJSON_GetObjectItem(node,"passwd")->valuestring)){
                char str[50];
                bzero(&str, 50);
                strcpy(str, "You Get It!!\n");
                add_file_size(client, str);
            }
           // }
        }
        else
        {
            char str[50];
            bzero(&str, 50);
            strcpy(str, "该用户已被注册，请重新输入");
            add_file_size(client, str);
        }
    }
    cJSON_Delete(node);
}
/***************************登录***************************/
void login(int client, char *string)             //登录必须与连接放在一块不然肯定会出现用户数量错误
{  
    cJSON *node = cJSON_Parse(string);
    int item = cJSON_GetObjectItem(node, "id")->valueint;
    usr[usr_number].id = item;
    strcpy(usr[usr_number].usrname,cJSON_GetObjectItem(node,"name")->valuestring);
    strcpy(usr[usr_number].passwd,cJSON_GetObjectItem(node,"passwd")->valuestring);
   // cJSON_AddNumberToObject(node,"status",ON_LINE);
    modify_status(item,ON_LINE);
    usr_number++;                              //登陆之后让用户数量加一
    uid = item;                               //之前测试所用，先保留
    char temp[15];
    strcmp(temp, cJSON_GetObjectItem(node, "passwd")->valuestring);
    if (connect_to_mysql())
    {
        if (check_usr_exist(item))
        {
            if (check_login_passwd(item, temp))
            {
                char buf[30];
                strcpy(buf, "login successfully!!");
                add_file_size(client, buf);
            }
            else
            {
                char buf[30];
                strcpy(buf, "密码错误!!");
                add_file_size(client, buf);
            }
        }
        else
        {
            char buf[30];
            strcpy(buf, "id不存在!!");
            add_file_size(client, buf);
        }
    }
}
int main(int argc, char *argv[])
{
    int sockfd;
    char buf[80];
    struct sockaddr_in host_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("创建套接字失败\n");
        my_error("socket", __LINE__);
    }
    bzero(&host_addr, sizeof(struct sockaddr_in));
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    struct epoll_event ev, events[50];
    int ret;
    if ((ret = bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))) < 0)
    {
        printf("绑定套接字失败\n");
        my_error("bind", __LINE__);
    }
    ret = listen(sockfd, BACKLOG);
    printf("等待客户端连接.......\n");
    int epollfd = epoll_create(100);
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
    connect_to_mysql();
    int j = 0;
    while (1)
    {
        struct sockaddr_in client_addr;
        int ret;
        int length = sizeof(client_addr);
        int num_fd = epoll_wait(epollfd, events, 50, 1000);
        for (int i = 0; i < num_fd; i++)
        {
            if (events[i].data.fd == sockfd)
            {
                usr[usr_number].sockfd = events[i].data.fd;         //获得并保存用户的套接字
                ret = accept(sockfd, (struct sockaddr *)&client_addr, &length);
                printf("成功连接到一个客户端%s\n", inet_ntoa(client_addr.sin_addr));
                ev.data.fd = ret;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, ret, &ev);
            }
            else
            {
                printf("新消息\n");
                events[i].data.fd = ret;
                if (ret < 0)
                {
                    printf("添加失败\n");
                }
                int number;
                bzero(&client_addr, sizeof(buf));
                int m = recv(ret, &number, 16, 0);
                if (m == 0)
                {
                    printf("客户端退出\n");
                    continue;
                }
                char *str = (char *)malloc(sizeof(char) * number);
                int c = recv(ret, str, number, 0);
                connect_to_mysql();
                cJSON *node = cJSON_Parse(str);
                FRIEND_INFO ff;
                cJSON * json = cJSON_CreateObject();
                if(strcmp("start",cJSON_GetObjectItem(node,"signal")->valuestring) == 0)
                {
                    if(get_friend_list(2,ret,&ff))
                    {
                    /* cJSON_AddStringToObject(json,"signal","you are lucky");
                        char * pass = cJSON_PrintUnformatted(json);
                        add_file_size(sockfd,pass);*/
                        printf("good\n");
                    }
                    else
                    printf("stupid\n");
                }
                else
                printf("stulid2\n");
                /*   int item = cJSON_GetObjectItem(node,"id")->valueint;
                add_friends(uid,item,1,1);*/
                // printf("%s\n",cJSON_GetObjectItem(node,"passwd")->valuestring);
                //char * st = (char *)malloc(sizeof(char)*30);
                /******************一个不清真的问题**************************/
                /*
                if (check_login_passwd(cJSON_GetObjectItem(node, "id")->valueint, cJSON_GetObjectItem(node, "passwd")->valuestring))
                {
                    cJSON *json = cJSON_CreateObject();
                    cJSON_AddStringToObject(json, "sinal", "登录成功");
                    char *pass = cJSON_Print(json);
                    add_file_size(ret, pass);
                }
                else
                {
                    cJSON *json = cJSON_CreateObject();
                    cJSON_AddStringToObject(json, "sinal", "登录失败");
                    char *pass = cJSON_Print(json);
                    add_file_size(ret, pass);
                }
                */
                /*******************************************************/
            }
        }
    }
    close(sockfd);
    return 0;
}
