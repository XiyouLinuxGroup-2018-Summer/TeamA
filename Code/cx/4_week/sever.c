#include "student.h"
extern pthread_mutex_t mutex;
extern int usr_number;
extern int uid;
extern struct usr_info usr[USR_NUM];
MYSQL mysql;
int type; /*判断文件类型*/
char sql_str[100];
/***********************快递员********************************/
int add_file_size(int fd, char *pass) //防止粘包现象的发生
{
    int m = strlen(pass) + 16;
    char temp_pack[m+1];
    temp_pack[m] = 0;
    strcpy(temp_pack + 16, pass);
    *(int *)temp_pack = strlen(pass);
    printf("%s\n",pass);
    send(fd, &temp_pack, m, 0);
    // free(temp_pack);
    return 1;
}
/*获得好友套接字*/
char *get_guys_sockfd(int recv_id)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select sockfd from usr_info where id='%d'", recv_id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("get_guys_sockfd:%s\n", mysql_error(&mysql));
        return NULL;
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    cJSON *json = cJSON_CreateObject();
    char *pass = NULL;
    if (row)
    {
        printf("sock:%d", atoi(row[0]));
        cJSON_AddNumberToObject(json, "sockfd", atoi(row[0]));
        pass = cJSON_PrintUnformatted(json);
    }
    else
        printf("获取套接字失败\n");
    cJSON_Delete(json);
    mysql_free_result(res);
    return pass;
}
/*解码*/
inline int num_strchr(const char *str, char c)
{
	const char *pindex = strchr(str, c);
	if (NULL == pindex)
	{
		return -1;
	}
	return pindex - str;
}
//发送文件信息回馈函数
void send_file_re(char * string)
{
	//now_path = TS_PATH;
    cJSON *json= cJSON_Parse(string);
	int from_id = cJSON_GetObjectItem(json, "from_id")->valueint;
	int to_id = cJSON_GetObjectItem(json, "to_id")->valueint;
    char *recv = get_guys_sockfd(to_id);
    cJSON *tnode = cJSON_Parse(recv);
    int f_sock = cJSON_GetObjectItem(tnode, "sockfd")->valueint; //获得好友的套接字
    cJSON_Delete(tnode);
    cJSON_Delete(json);
	printf("用户为%d的用户想向您传输文件(Y or N)\n",from_id);
	char ch;
	scanf("%c", &ch);
	while ('\n' != (getchar()));
	cJSON *node = cJSON_CreateObject();
	cJSON_AddNumberToObject(node, "signal",SEND_BACK);
	cJSON_AddNumberToObject(node, "to_id", to_id);
	cJSON_AddNumberToObject(node, "from_id",from_id);
	if (ch == 'y' || ch == 'Y')
	{
		cJSON_AddNumberToObject(node, "send_f_r", 1);
	}
	else if (ch == 'n' || ch == 'N')
	{
		cJSON_AddNumberToObject(node, "send_f_r", 0);
	}
    char * pass = cJSON_PrintUnformatted(node);
    cJSON_Delete(node);
	add_file_size(f_sock,pass);
}
/*********************数据库的使用**********************/
void my_error(const char *string, int line)
{
    fprintf(stderr, "line:%d", line);
    perror(string);
    exit(1);
}
cJSON *analysis_pack(int client, int number)
{
    char *temp = (char *)malloc(sizeof(char) * number + 1);
    temp[number] = 0;
    recv(client, temp, number, 0);
    printf("%s\n", temp);
    cJSON *node = cJSON_Parse(temp);
    free(temp);
    return node;
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
/*修改用户套接字*/
int modify_sock(int id, int sockfd)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "update usr_info set sockfd='%d' where id ='%d'", sockfd, id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("update sockfd:%s\n", mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/*获取好友状态*/
int get_status(int id)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select status from usr_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("get_status:%s\n", mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m;
    if (row)
    {
        if (atoi(row[4]) == 1)
            m = 1;
        else
            m = 0;
    }
    printf("status:%d\n", m);
    mysql_free_result(res);
    return m;
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
    if (row != NULL) //不是很清真
    {
        if (strcmp(passwd, row[2]) == 0)
            m = 1;
    }
    //  printf("%d\n", m);
    // printf("%s\n",row[2]);
    // printf("%s\n",passwd);
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
int add_usr(int id, char *name, char *passwd, int sockfd)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "insert into usr_info  values ('%d','%s','%s','0','0','%d')", id, name, passwd, sockfd);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("add_usr:%s\n", mysql_error);
        return 0;
    }
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",ADD_USR);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
    return 1;
}
/*删除好友*/
int delate_usr(int id,int sockfd)
{
    bzero(&sql_str, 100);
    printf("haoba\n");
    sprintf(sql_str, "delete  from friend_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("delate_usr:%s\n", mysql_error(&mysql));
        return 0;
    }
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",DELETE_FRIEND);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
    return 1;
}
/*********************************添加好友**************************************/
/*第一次好友添加请求*/
void add_friend_add(int sockfd, char *string)
{
    int id, pid;
    printf("aab\n");
    cJSON *json = cJSON_Parse(string);
    pid = cJSON_GetObjectItem(json, "pid")->valueint;
    cJSON_Delete(json);
    char *recv = get_guys_sockfd(pid);
    cJSON *tnode = cJSON_Parse(recv);
    int f_sock = cJSON_GetObjectItem(tnode, "sockfd")->valueint; //获得好友的套接字
    cJSON_Delete(tnode);
    printf("bbc\n");
    printf("%s\n", string);
    add_file_size(f_sock, string);
}
/*添加好友到数据库*/
int add_friends_to_mysql(int id, int pid, int status, int flag)
{
    bzero(&sql_str, 100);
    uid = 2;
    id = uid;
    if (check_usr_exist(pid))
    {
        if (flag == AGREE)
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
/*第二次好友回复请求*/
void add_friend_reply(int sockfd, char *string)
{
    int uid, pid;
    printf("ccd\n");
    cJSON *json = cJSON_Parse(string);
    printf("dde\n");
    uid = cJSON_GetObjectItem(json, "uid")->valueint;
    pid = cJSON_GetObjectItem(json, "pid")->valueint;
    if (add_friends_to_mysql(pid, uid, -1, cJSON_GetObjectItem(json, "tip")->valueint))
        printf("好友信息保存成功\n");
    else
        printf("好友信息保存失败\n");
    cJSON_Delete(json);
    char *recv = get_guys_sockfd(uid);
    cJSON *tnode = cJSON_Parse(recv);
    int f_sock = cJSON_GetObjectItem(tnode, "sockfd")->valueint; //获得请求方的套接字
    cJSON_Delete(tnode);
    printf("eef\n");
    add_file_size(f_sock, string);
}
/**************************************添加好友********************************/

/*加群*/
int add_group(char * string,int sockfd)
{
    cJSON * node= cJSON_Parse(string);
    cJSON_AddNumberToObject(node,"signal",ADD_GROUP);
    bzero(&sql_str,100);
    int uid = cJSON_GetObjectItem(node,"uid")->valueint;
    int gid = cJSON_GetObjectItem(node,"gid")->valueint;
    sprintf(sql_str,"insert into group_member values('%d','%d','-1')",uid,gid);
    int ret = mysql_real_query(&mysql,sql_str,strlen(sql_str));
    if( ret != 0)
    {
        printf("add_group:%s\n",mysql_error(&mysql));
        return 0;
    }
    cJSON_Delete(node);
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",ADD_GROUP);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
    return 1;
}
/**********************获取好友列表*********************/
int get_friend_list(int id, int sockfd)   
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from friend_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("get_friend_info:%s\n", mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    int i = 0;
    while ((row = mysql_fetch_row(res)))
    {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json,"signal",GET_FRI_LIST);
        cJSON_AddNumberToObject(json,"pid",atoi(row[1]));
        cJSON_AddNumberToObject(json,"status",atoi(row[2]));
        char *pass = cJSON_PrintUnformatted(json);
        add_file_size(sockfd, pass);
        cJSON_Delete(json);
    }
    return 1;
}
/*获得群好友列表*/
int get_group_list(int sockfd,int gid)
{
     MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from  group_info where gid='%d'", gid);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("get_group_list:%s\n",mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    while ((row = mysql_fetch_row(res)))
    {
        cJSON * json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json,"signal",GET_GROUP_LIST);
        cJSON_AddNumberToObject(json,"id",atoi(row[4]));
        char * pass = cJSON_PrintUnformatted(json);
        add_file_size(sockfd,pass);
        cJSON_Delete(json);
    }
    return 1;
}
/*获得群列表*/
int group_usr_lsit(int sockfd,int id)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from  group_info where id='%d'", id);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("get_group_list:%s\n",mysql_error(&mysql));
        return 0;
    }
    res = mysql_store_result(&mysql);
    while ((row = mysql_fetch_row(res)))
    {
        cJSON * json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json,"signal",GROUP_USR_LST);
        cJSON_AddNumberToObject(json,"gid",atoi(row[0]));
        cJSON_AddStringToObject(json,"name",row[1]);
        char * pass = cJSON_PrintUnformatted(json);
        add_file_size(sockfd,pass);
        cJSON_Delete(json);
    }
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
/*梦寐中的私聊（测试版）*/
int chat_private(int sockfd, char *string)
{
    int send_fd, recv_fd;
    char save_time[20], msg[100];
    printf("%s\n", string);
    cJSON *node = cJSON_Parse(string);
    send_fd = cJSON_GetObjectItem(node, "send_fd")->valueint;
    recv_fd = cJSON_GetObjectItem(node, "recv_fd")->valueint;
    printf("cc%d\n", send_fd);
    time_t timep;
    time(&timep);
    strcpy(save_time, ctime(&timep));
    int len = strlen(save_time);
    save_time[len - 1] = '\0';
    cJSON_AddStringToObject(node, "time", save_time);
    char *item = cJSON_GetObjectItem(node, "content")->valuestring;
    char *pass = cJSON_PrintUnformatted(node);
    if (record_private(send_fd, recv_fd, pass, 1))
        printf("记录保存成功\n");
    else
    {
        printf("保存失败\n");
    }
    char *recv = get_guys_sockfd(recv_fd);
    cJSON *tnode = cJSON_Parse(recv);
    int f_sock = cJSON_GetObjectItem(tnode, "sockfd")->valueint;
    cJSON_Delete(tnode);
    printf("recv:%d\n", recv_fd);
    printf("%d\n", f_sock);
    if(strcmp(item,"bye") != 0)
     add_file_size(f_sock, pass);
    cJSON_Delete(node);
    return 1;
}
/*发送给在线好友*/
int send_online(int id, int status)
{
    int sockfd;
    FRIEND_INFO ff;
    cJSON *json = NULL;
   // get_friend_list(id, status, &ff);
    for (int i = 0; i < usr_number; i++)
    {
        if (status)
        {
            char *sock = get_guys_sockfd(id);
            json = cJSON_Parse(sock);
            int sockfd = cJSON_GetObjectItem(json, "sockfd")->valueint;
            if (sockfd == -1)
                return 0;
            cJSON *json = cJSON_CreateObject();
            cJSON_AddNumberToObject(json, "signal", SEND_LINE);
            cJSON_AddNumberToObject(json, "pid", id);
            cJSON_AddNumberToObject(json, "status", status);
            char *pass = cJSON_PrintUnformatted(json);
            cJSON_Delete(json);
            add_file_size(sockfd, pass);
        }
    }
}
/*发送离线消息*/
int send_logout_out_message(int sockfd)
{
}
/**********************建群****************************/
int mysql_create_group(int id, int gid, char *string, int sockfd)
{
    printf("%s\n", string);
    cJSON *node = cJSON_Parse(string);
    bzero(&sql_str, 100);
    sprintf(sql_str, "insert into group_info values('%d','%s','0','%d','%d')",
            cJSON_GetObjectItem(node, "gid")->valueint,
            cJSON_GetObjectItem(node, "g_name")->valuestring,
            ROOT,
            cJSON_GetObjectItem(node, "id")->valueint);
   // cJSON_Delete(node);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("create_mysql_group%s\n", mysql_error(&mysql));
        return 0;
    }
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "signal", CREAT_GROUP);
        cJSON_AddNumberToObject(json, "gid", gid);
        char *pass = cJSON_PrintUnformatted(json);
        printf("ss%s\n", pass);
        add_file_size(sockfd, pass);
        // cJSON_Delete(tnode);
        cJSON_Delete(json);
        return 1;
}
/*判断群是否存在*/
int check_group_exist(int gid)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from group_info where gid = '%d'", gid);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("group_exist:%s\n", mysql_error(&mysql));
        return 0;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m;
    if (row)
        m = 1;
    else
        m = 0;
    mysql_free_result(res);
    printf("group_exist:%d\n", m);
    return m;
}
/*设置管理员*/
int set_group_owner(int id, int gid)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "update group_info set root='%d' where (id = '%d' and gid = '%d')", ROOT, id, gid);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("update root:%s\n", mysql_error(&mysql));
        return 0;
    }
    return 1;
}
/*核对管理员*/
int check_group_owner(int id, int gid)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "select root from group_info where (id = '%d' and gid = '%d')", id, gid);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("check_group_owner:%s\n", mysql_error(&mysql));
        return 0;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m;
    if (!row)
    {
        printf("no\n");
        return 0;
    }
    if (atoi(row[0]) == ROOT)
        m = 1;
    else
        m = 0;
    mysql_free_result(res);
    return m;
}
/*设置禁言*/
int group_set_no_speaking(char *string, int sockfd)
{
    bzero(&sql_str, 100);
    cJSON *node = cJSON_Parse(string);
    int uid = cJSON_GetObjectItem(node, "uid")->valueint;
    int gid = cJSON_GetObjectItem(node, "gid")->valueint;
    // cJSON_Delete(node);
    if (check_group_owner(uid, gid))
    {
        sprintf(sql_str, "update group_member set authority = '%d' where (uid = '%d' and gid = '%d')", NO_SPEAKING, uid, gid);
        int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
        if (ret != 0)
        {
            printf("禁言%s\n", mysql_error(&mysql));
            return 0;
        }
        else
        {
            cJSON *json = cJSON_AddNumberToObject(json, "signal", NO_SPEAKING);
            cJSON_AddNumberToObject(json, "id", uid);
            char *pass = cJSON_PrintUnformatted(json);
            add_file_size(sockfd,pass);
            cJSON_Delete(json);
            return 1;
        }
    }
    else
        return 0;
}
/*核查成员状态是否被禁言*/
int group_check_no_speaking(int id, int gid)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "select authority from group_member where (uid = '%d' and gid='%d')", id, gid);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("check_speaking:%s\n", mysql_error(&mysql));
        return 0;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    int m;
    if (atoi(row[0]) == NO_SPEAKING)
        m = 1;
    else
        m = 0;
    mysql_free_result(res);
    return m;
}
/*取消禁言*/
/*ｓｔｒｉｎg为原始数据包*/
int group_cancle_no_speaking(char *string, int sockfd)
{
    bzero(&sql_str, 100);
    printf("not lucky\n");
    cJSON *node = cJSON_Parse(string);
    int uid = cJSON_GetObjectItem(node, "uid")->valueint;
    int gid = cJSON_GetObjectItem(node, "gid")->valueint;
    cJSON_Delete(node);
    if (check_group_owner(uid, gid))
    {
        sprintf(sql_str, "update group_member set authority = '%d' where (uid = '%d' and gid = '%d')", SPEAKING, uid, gid);
        int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
        if (ret != 0)
        {
            printf("解除禁言%s\n", mysql_error(&mysql));
            return 0;
        }
        else
        {
            cJSON *json = cJSON_CreateObject();
            cJSON_AddNumberToObject(json, "signal", SPEAKING);
            cJSON_AddStringToObject(json, "content", "unlock");
            char *pass = cJSON_PrintUnformatted(json);
            printf("aaa%s\n", pass);
            add_file_size(sockfd, pass);
            cJSON_Delete(json);
            return 1;
        }
    }
    else
        return 0;
}
/*删除群成员*/ //不能直接通过ｕｉｄ来删除==>>退群
int group_delate_member(int uid, int gid,int sockfd)
{
    bzero(&sql_str, 100);
    sprintf(sql_str, "delete from group_member where(uid = '%d' and gid = '%d')", uid, gid);
    if (mysql_real_query(&mysql, sql_str, strlen(sql_str)) != 0)
    {
        printf("add_member:%s\n", mysql_error(&mysql));
        return 0;
    }
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",QUIT_GROUP);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
    return 1;
}
/*退群*/
int group_delate(int uid, int gid,int sockfd)
{
    printf("hehehe\n");
    bzero(&sql_str, 100);
    sprintf(sql_str, "delete from group_info where(id = '%d' and gid = '%d')", uid, gid);
    if (mysql_real_query(&mysql, sql_str, strlen(sql_str)) != 0)
    {
        printf("delete_group1:%s\n", mysql_error(&mysql));
        return 0;
    }
    bzero(&sql_str,100);
    sprintf(sql_str,"delete from group_member where(uid = '%d' and gid = '%d')",uid,gid);
    if (mysql_real_query(&mysql, sql_str, strlen(sql_str)) != 0)
    {
        printf("group_delete:%s\n", mysql_error(&mysql));
        return 0;
    }
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",QUIT_GROUP_T);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
    return 1;
}
/*添加群成员*/
int add_group_member(int id, int gid, char *string)
{
    cJSON *node = cJSON_Parse(string);
    bzero(&sql_str, 100);
    sprintf(sql_str, "insert into group_info values('%d','%s',%d'",
            cJSON_GetObjectItem(node, "id")->valueint,
            cJSON_GetObjectItem(node, "name")->valuestring,
            cJSON_GetObjectItem(node, "gid")->valueint);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("create_mysql_group%s\n", mysql_error(&mysql));
        return 0;
    }
    else
        return 1;
}
/*群聊*/
int chat_group(char *string, int sockfd)
{
    printf("ll%s",string);
    cJSON *node = cJSON_Parse(string);
    int gid = cJSON_GetObjectItem(node, "gid")->valueint;
    bzero(&sql_str, 100);
    sprintf(sql_str, "select * from group_member where gid='%d'", gid);
    int ret = mysql_real_query(&mysql, sql_str, strlen(sql_str));
    if (ret != 0)
    {
        printf("chat_group:%s\n", mysql_error(&mysql));
        return 0;
    }
    MYSQL_RES *res = mysql_store_result(&mysql);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "signal", CHAT_GROUP);
        cJSON_AddItemToObject(json, "uid", cJSON_CreateNumber(atoi(row[1])));
        cJSON_AddStringToObject(json, "content", cJSON_GetObjectItem(node, "content")->valuestring);
        char *pass = cJSON_PrintUnformatted(json);
        cJSON_Delete(json); //不会吧
        printf("send_info:%s\n", pass);
        char *recv = get_guys_sockfd(atoi(row[1]));
        printf("%s\n",recv);
        cJSON *tnode = cJSON_Parse(recv);
        int f_sock = cJSON_GetObjectItem(tnode, "sockfd")->valueint; //获得好友的套接字
        if (strcmp(cJSON_GetObjectItem(node, "content")->valuestring, "bye") == 0)
            break;
        add_file_size(f_sock, pass);
        cJSON_Delete(json);
    }
    cJSON_Delete(node);
    mysql_free_result(res);
    return 1;
}
/*群聊*/
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

/********************************文件传输***********************************/
/*int file_send(char *string)
{
    cJSON *json = cJSON_Parse(string);
    int uid = cJSON_GetObjectItem(json, "uid")->valueint;
    int f_sock = get_guys_sockfd(uid);
    add_file_size(f_sock, string);
    cJSON_Delete(json);
}*/

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
            bzero(&sql_str, 100);
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
                strcpy(str, "该用户已被注册，请重新输入ｌｌｌ");
                 cJSON * json = cJSON_CreateObject();
                cJSON_AddNumberToObject(json,"signal",REGISTER);
                cJSON_AddStringToObject(json,"content",str);
                char* pass = cJSON_PrintUnformatted(json);
                printf("注册%s\n",pass);
                add_file_size(client, pass);
               cJSON_Delete(json);
            }
                char str[50];
                bzero(&str, 50);
                strcpy(str, "You Get It!!");
                cJSON * json = cJSON_CreateObject();
                cJSON_AddNumberToObject(json,"signal",REGISTER);
                cJSON_AddStringToObject(json,"content",str);
                char* pass = cJSON_PrintUnformatted(json);
                printf("注册%s\n",pass);
                add_file_size(client, pass);
               cJSON_Delete(json);
        }
        else
        {
            char str[50];
            bzero(&str, 50);
            strcpy(str, "该用户已被注册，请重新输入;;;");
            cJSON * json = cJSON_CreateObject();
                cJSON_AddNumberToObject(json,"signal",REGISTER);
                cJSON_AddStringToObject(json,"content",str);
                char* pass = cJSON_PrintUnformatted(json);
                printf("注册%s\n",pass);
                add_file_size(client, pass);
               cJSON_Delete(json);
        }
    }

    cJSON_Delete(node);
}
/***************************登录***************************/
void login(int client, char *string) //登录必须与连接放在一块不然肯定会出现用户数量错误
{
    cJSON *node = cJSON_Parse(string);
    printf("%s\n", string);
    int item = cJSON_GetObjectItem(node, "id")->valueint;
    usr[usr_number].id = item;
    printf("login_id:%d\n", usr[usr_number].id);
    strcpy(usr[usr_number].usrname, cJSON_GetObjectItem(node, "name")->valuestring);
    strcpy(usr[usr_number].passwd, cJSON_GetObjectItem(node, "passwd")->valuestring);
    // cJSON_AddNumberToObject(node,"status",ON_LINE);
    usr_number++; //登陆之后让用户数量加一
                  // uid = item;                               //之前测试所用，先保留
    char temp[15];
    strcpy(temp, cJSON_GetObjectItem(node, "passwd")->valuestring);
    printf("%s\n", temp);
    // cJSON_Delete(node);
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", LOGIN);
    if (connect_to_mysql())
    {
        modify_status(item, ON_LINE);
        if (check_usr_exist(item))
        {
            if (check_login_passwd(item, temp))
            {
                modify_sock(item, client);
                cJSON_AddStringToObject(json, "flag", "login successfully!!");
                char *pass = cJSON_PrintUnformatted(json);
                add_file_size(client, pass);
            }
            else
            {
                cJSON_AddStringToObject(json, "flag", "密码错误!!");
                char *pass = cJSON_PrintUnformatted(json);
                add_file_size(client, pass);
            }
        }
        else
        {
            cJSON_AddStringToObject(json, "flag", "密码错误!!");
            char *pass = cJSON_PrintUnformatted(json);
            add_file_size(client, pass);
        }
    }
    cJSON_Delete(json);
}
void *handle(void *arg)
{ //msg->buf 保存原始的数据包
    MSG *msg = (MSG *)malloc(sizeof(MSG));
    msg = (MSG *)arg;
    //    pthread_mutex_lock(&mutex);
    printf("yes\n");
    int item = cJSON_GetObjectItem(msg->node, "signal")->valueint;
    printf("%d\n",item);
    printf("eee\n");
    int m = msg->fd;
    switch (item)
    {
    case LOGIN:
        login(m, msg->buf);
        break;
    case REGISTER:
        reg(m, msg->buf);
        break;
    case DELETE_FRIEND:
         delate_usr(cJSON_GetObjectItem(msg->node,"id")->valueint,m);
        break;
    case CHAT_PRI:
        chat_private(m, msg->buf);
        break;
    case ADD_FRIEND:
        add_friend_add(m, msg->buf);
        break;
    case FRIEND_REPLY:
        add_friend_reply(m, msg->buf);
        break;
    case CREAT_GROUP:
        mysql_create_group(cJSON_GetObjectItem(msg->node, "id")->valueint,
                           cJSON_GetObjectItem(msg->node, "gid")->valueint, msg->buf, m);
        break;
    case CHAT_GROUP:
        chat_group(msg->buf, m);
        break;
    case NO_SPEAKING:
        if (check_group_owner(cJSON_GetObjectItem(msg->node, "id")->valueint,
                              cJSON_GetObjectItem(msg->node, "gid")->valueint))
            group_set_no_speaking(msg->buf,m);
        break;
    case SPEAKING:
        if (check_group_owner(cJSON_GetObjectItem(msg->node, "id")->valueint,
                              cJSON_GetObjectItem(msg->node, "gid")->valueint))
            group_cancle_no_speaking(msg->buf, m);
        break;
    case GET_FRI_LIST:
          get_friend_list(cJSON_GetObjectItem(msg->node,"id")->valueint,m);
          break;
    case GET_GROUP_LIST:
         get_group_list(m,cJSON_GetObjectItem(msg->node,"gid")->valueint);
         break;
    case GROUP_USR_LST:
         group_usr_lsit(m,cJSON_GetObjectItem(msg->node,"id")->valueint);
         break;
   /* case QUIT_GROUP:
         quit*/
    case QUIT_GROUP:
        group_delate_member(cJSON_GetObjectItem(msg->node,"uid")->valueint,
        cJSON_GetObjectItem(msg->node,"gid")->valueint,m);
        break;
    case QUIT_GROUP_T:
        group_delate(cJSON_GetObjectItem(msg->node,"uid")->valueint,
        cJSON_GetObjectItem(msg->node,"gid")->valueint,m);
        break;
    case ADD_GROUP:
        add_group(msg->buf,m);
        break;
    case SEND_FILE:
       send_file_re(msg->buf);
       break;
    default:
        printf("敬请期待\n");
        break;
    }
    printf("ggg\n");
    //pthread_mutex_unlock(&mutex);
    free(msg);
    return NULL;
}
int main(int argc, char *argv[])
{
    int sockfd;
    char buf[80];
    pthread_mutex_init(&mutex, NULL);
    struct sockaddr_in host_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("创建套接字失败\n");
        my_error("socket", __LINE__);
    }
    bzero(&host_addr, sizeof(struct sockaddr_in));
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr  = INADDR_ANY; //inet_addr("127.0.0.1");
    struct epoll_event ev, events[100];
    int ret;
    if ((ret = bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))) < 0)
    {
        printf("绑定套接字失败\n");
        my_error("bind", __LINE__);
    }
    ret = listen(sockfd, BACKLOG);
    printf("等待客户端连接.......\n");
    int epollfd = epoll_create(100);
    //  int temp = 1;
    // setsockopt(ret,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
    int j = 0;
    while (1)
    {
        struct sockaddr_in client_addr;
        int ret;
        int length = sizeof(client_addr);
        int num_fd = epoll_wait(epollfd, events, 50, 1000);
        int number = 0;
        for (int i = 0; i < num_fd; i++)
        {
            if (events[i].data.fd == sockfd)
            {
                ret = accept(sockfd, (struct sockaddr *)&client_addr, &length);
                //  usr[usr_number].sockfd = events[i].data.fd;
                usr[usr_number].sockfd = ret;
                printf("sss%d\n", events[i].data.fd);
                printf("成功连接到一个客户端%s\n", inet_ntoa(client_addr.sin_addr));
                ev.data.fd = ret;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, ret, &ev);
            }
            else
            {
                if (recv(events[i].data.fd, &number, 16, 0) == 0)
                {
                    printf("用户已下线\n");
                    printf("%d\n", usr_number);
                    modify_status(events[i].data.fd,OUT_LINE); //更改用户为离线状态
                    usr_number--;
                    ev.data.fd = events[i].data.fd;
                    ev.events = EPOLL_CTL_DEL;
                    close(events[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                    continue;
                }
                else
                {
                    char *temp = (char *)malloc(sizeof(char) * number+1);
                     temp[number] = 0;
                    recv(events[i].data.fd, temp, number, 0);
                    printf("ｌｌｌｌｌｌｌｌｌｌｌｌｌ%s\n", temp);
                    cJSON *node = cJSON_Parse(temp);
                    MSG *msg = (MSG *)malloc(sizeof(MSG));
                    msg->node = node;
                    strcpy(msg->buf, temp);
                    msg->fd = events[i].data.fd;
                    printf("%s\n", temp);
                    pthread_t thid;
                    pthread_create(&thid, NULL, handle, (void *)msg);
                    printf("ddd\n");
                    // free(msg);         不能释放
                }
            }
        }
    }
    return 0;
}
