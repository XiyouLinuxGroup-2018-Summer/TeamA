#include"student.h"
extern pthread_mutex_t mutex;
extern int usr_count;
/*********************数据库的使用**********************/
MYSQL mysql;
MYSQL_RES * res;
MYSQL_ROW  row;
int type;                                     /*判断文件类型*/
char sql_str[100];
void my_error(const char *string,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(string);
    exit(1);
}
int connect_to_mysql(void)
{
    if(NULL == mysql_init(&mysql))
    printf("init:%s\n",mysql_error(&mysql));
    if(NULL == mysql_real_connect(&mysql,"localhost","root","CT1999","chat_room",0,NULL,0))
    {
        printf("connect:%s\n",mysql_error(&mysql));
        return 0;
    }
    printf("ｃｏｎｎｃｅｔ\n");
    return 1;
}
/***********************快递员********************************/
void  add_file_size(int fd,char * pass)       //防止粘包现象的发生
{ 
    int m = strlen(pass) + 16;
  //  char *temp_pack =(char*)malloc(sizeof(char)*m);
    char temp_pack[m];                          //大小也是个坑
    strcpy(temp_pack+16,pass);
    *(int*)temp_pack = strlen(pass);
    printf("%s\n",pass);
    send(fd,&temp_pack,m,0);
   // free(temp_pack);
    return;
}
/*保存数据到数据库*/
void save_package_to_mysql(char *string)
{
    cJSON *node = cJSON_Parse(string);
    sprintf(sql_str,"insert into pack_info(send_name,recv_name,send_fd,recv_fd,time,content) values('%s','%s','%d','%d',%s','%s')",
    cJSON_GetObjectItem(node,"send_name")->valuestring, 
    cJSON_GetObjectItem(node,"recv_name")->valuestring,
    cJSON_GetObjectItem(node,"send_fd")->valueint,
    cJSON_GetObjectItem(node,"recv_fd")->valueint,
    cJSON_GetObjectItem(node,"time")->valuestring,
    cJSON_GetObjectItem(node,"content")->valuestring);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if(ret !=  0)
    printf("insert:%s\n",mysql_error(&mysql));
    cJSON_Delete(node);
}
/*判断重复用户*/
int find_usr(int id)
{
    sprintf(sql_str,"select * from usr_info where id='%d'",id);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if(ret != 0)
    {
        printf("find:%s\n",mysql_error(&mysql));
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m  = 1;
    if(row)
       m = 0;
    mysql_free_result(res);
    return m;
}
/*修改用户状态*/
int check_status(int id,int status)
{
    bzero(&sql_str,100);
    sprintf(sql_str,"update usr_info set status='%d' where id='%d'",status,id);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if(ret != 0)
    {
       printf("check_status:%s\n",mysql_error(&mysql));
       return 0;
    }
   printf("更新成功\n");
   return 1;
}
/*密码找回*/
char *  back_passwd(int id)                 /*粗糙版本*/
{
    char * str = (char *)malloc(sizeof(char)*10);
    bzero(&sql_str,100);
    sprintf(sql_str,"select * from usr_info where id='%d'",id);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if( ret != 0)
    {
        printf("back_passwd:%s\n",mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    if(row)
    strcpy(str,row[2]);
    mysql_free_result(res);
    printf("%s\n",str);
    return str;
}
/*验证登录*/
int check_login_passwd(int id,char * passwd)
{
    bzero(&sql_str,100);
    sprintf(sql_str,"select * from usr_info where (id = '%d' and passwd = '%s')",id,passwd);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if(ret !=0)
    {
        printf("check_login:%s\n",mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m = 1;
    if(row)
       m = 0;
    mysql_free_result(res);
    return m;
}
/*判断用户是否存在*/
int check_usr_exist(int id)
{
    bzero(&sql_str,100);
    sprintf(sql_str,"select * from usr_info where id='%d'",id);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if(ret != 0)
    {
        printf("check_use_exist:%s\n",mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/*添加用户*/
int add_usr(int id,char *name,char * passwd)
{
    bzero(&sql_str,100);
    sprintf(sql_str,"insert into usr_info  values ('%d','%s','%s','NULL','NUll')",id,name,passwd);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if( ret != 0)
    {
        printf("add_usr:%s\n",mysql_error);
        return 0;
    }
    return 1;
}
/*删除用户*/
int delate_usr(int id)
{
    bzero(&sql_str,100);
    sprintf(sql_str,"delate * from usr_info where id='%d'",id);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if( ret != 0)
    {
        printf("delate_usr:%s\n",mysql_error(&mysql));
        return 0;
    }
    return 1;
}
int add_friends(int id,int pid,int status,int flag)
{
    bzero(&sql_str,100);
    if(flag)
    {
        sprintf(sql_str,"insert into group_info values ('%d','%d','%d')",id,pid,status);
        int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
        if(ret != 0)
        {
            printf("add_friend:%s\n",mysql_error(&mysql));
            return 0;
        }
        printf("已添加对方为好友\n");
        return 1;
    }
    else
    {
        printf("对方拒绝了你的申请，添加失败\n");
        return 0;
    }
}
/**********************私聊记录保存(离线还是在线)*********************/
int record_private(int send_fd,int recv_fd,char *msg,int status)
{
    bzero(&sql_str,100);
    sprintf(sql_str,"insert into record_private  values('%d','%d','%s','%d')",send_fd,recv_fd,msg,status);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if( ret != 0)
    {
        printf("record_private:%s\n",mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/**********************群聊记录*************************************/
int record_private(int group_fd,char *msg)
{
    bzero(&sql_str,100);
    sprintf(sql_str,"insert into record_group  values('%d','0','%s','0')",group_fd,msg);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if( ret != 0)
    {
        printf("record_private:%s\n",mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/**********************注册********************/
void reg(int client,char* string)
{
    cJSON *node = cJSON_Parse(string);
    if(connect_to_mysql())
    {
        printf("%s\n",cJSON_Print(node));
        int m = cJSON_GetObjectItem(node,"id")->valueint;
        if(find_usr(m))
        {
            printf("hello\n");
            bzero(&sql_str,100);
            sprintf(sql_str,"insert into usr_info(id,name,passwd) values(%d,'%s','%s')",
            cJSON_GetObjectItem(node,"id")->valueint,
            cJSON_GetObjectItem(node,"usrname")->valuestring,
            cJSON_GetObjectItem(node,"passwd")->valuestring);
            int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
            if(ret !=  0)
            {
                printf("insert:%s\n",mysql_error(&mysql));
                char str[50];
                bzero(&str,50);
                strcpy(str,"该用户已被注册，请重新输入");
                add_file_size(client,str);
                
            }
            else
            {
                char str[50];
                bzero(&str,50);
                strcpy(str,"You Get It!!\n");
                add_file_size(client,str);
            }
        }
        else
        {
            char str[50];
            bzero(&str,50);
            strcpy(str,"该用户已被注册，请重新输入");
            add_file_size(client,str);
        }
    }
    cJSON_Delete(node);
}
/***************************登录***************************/
void login(int client,char * string)
{
    cJSON * node  = cJSON_Parse(string);
    int item = cJSON_GetObjectItem(node,"id")->valueint;
    char temp[15];
    strcmp(temp,cJSON_GetObjectItem(node,"passwd")->valuestring);
    if(connect_to_mysql())
    {
        if(check_usr_exist(item))
        {
            if(check_login_passwd(item,temp))
            {
                char buf[30];
                strcpy(buf,"login successfully!!");
                add_file_size(client,buf);
            }
            else
            {
                char buf[30];
                strcpy(buf,"密码错误!!");
                add_file_size(client,buf);
            }
        }
        else
        {
            char buf[30];
            strcpy(buf,"id不存在!!");
            add_file_size(client,buf);
        }
    }
}
int main (int argc,char *argv[])
{
    int sockfd;
    char buf[80];
    struct sockaddr_in host_addr; 
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("创建套接字失败\n");
        my_error("socket",__LINE__);
    }
    bzero(&host_addr,sizeof(struct sockaddr_in));
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    struct epoll_event ev, events[50];
    int ret;
    if((ret = bind(sockfd,(struct sockaddr*)&host_addr,sizeof(struct sockaddr))) < 0)
    {
        printf("绑定套接字失败\n");
        my_error("bind",__LINE__);
    }
    ret = listen(sockfd,BACKLOG);
    printf("等待客户端连接.......\n");
    int epollfd = epoll_create(100);
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
    connect_to_mysql();
    while(1)
    {
        struct sockaddr_in client_addr;
        int ret;
        int length = sizeof(client_addr);
        int num_fd = epoll_wait(epollfd,events,50,1000);
        for(int i = 0;i< num_fd;i++)
        {
            if(events[i].data.fd == sockfd)
            {
                ret = accept(sockfd,(struct sockaddr*)&client_addr,&length);
                printf("成功连接到一个客户端%s\n",inet_ntoa(client_addr.sin_addr));
                /*pthread_t thid1;
                pthread_create(&thid1,NULL,send_package,(void*)ret);
                pthread_join(thid1,NULL);*/
                ev.data.fd = ret;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd,EPOLL_CTL_ADD,ret,&ev);
            }
            else
            {
                printf("新消息\n");
                events[i].data.fd = ret;
                if(ret < 0)
                {
                    printf("添加失败\n");
                }
                int number;
                bzero(&client_addr,sizeof(buf));
                int m = recv(ret,&number,16,0);
                if(m == 0) {
                    printf("客户端退出\n");
                    continue;
                }
                char * str = (char *)malloc(sizeof(char)*number);
                int c =  recv(ret,str,number,0);
                //reg(ret,str);
                login(ret,str);
               /* printf("%d,%d\n",number,c);
                printf("%s\n",str);
                printf("kk\n");
                cJSON * node = cJSON_Parse(str);
                char * st = (char *)malloc(sizeof(char)*30);
                st = back_passwd(cJSON_GetObjectItem(node,"id")->valueint);
                cJSON * json = cJSON_CreateObject();
                cJSON_AddStringToObject(json,"passwd",st);
                free(st);
                char * pass = cJSON_Print(json);
                add_file_size(ret,pass);*/
            }
        }
    }
    close(sockfd);
    return 0;
}
