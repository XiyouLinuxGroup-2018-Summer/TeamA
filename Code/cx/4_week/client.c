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
#include <fcntl.h>
#define PORT 8888
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
#define NO_SPEAKING 14 //禁言　　　
#define CHAT_GROUP 15
#define SEND_FILE 16     //上传文件
#define SPEAKING 17      //解除禁言
#define ADD_USR 18       //添加好友
#define DELETE_FRIEND 19 //删除好友
#define GET_FRI_LIST 20  //获得好友列表
#define GROUP_USR_LST 21 //获得群列表
#define QUIT_GROUP 22    //删除群成员
#define QUIT_GROUP_T 23  //　退群
#define ADD_GROUP   24    //加群
#define SEND_BACK   25   //文件回馈
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
char friend[30];
char pmsg[100][100];   //临时储存私聊记录
char gmsg[100][100];    //临时储存群聊记录
int uid;
int count;
int i = 0;
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
/*发包函数，美滋滋*/
void add_file_size(int fd, char *pass)
{
    int m = strlen(pass) + 16;
    //char *temp_pack =(char*)malloc(sizeof(char)*m);
    char temp_pack[m + 1]; //大小也是个坑
    temp_pack[m] = 0;
    strcpy(temp_pack + 16, pass);
    *(int *)temp_pack = strlen(pass);
    printf("lll%s\n", pass);
    send(fd, &temp_pack, m, 0);
    return;
}
/*编码函数*/
const char *base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char padding_char = '=';
int base64_encode(const unsigned char *sourcedata, char *base64, int datalength)
{
	int i = 0, j = 0;
	unsigned char trans_index = 0;
	for (; i < datalength; i += 3)
	{
		trans_index = ((sourcedata[i] >> 2) & 0x3f);
		base64[j++] = base64char[(int)trans_index];
		trans_index = ((sourcedata[i] << 4) & 0x30);
		if (i + 1 < datalength)
		{
			trans_index |= ((sourcedata[i + 1] >> 4) & 0x0f);
			base64[j++] = base64char[(int)trans_index];
		}
		else
		{
			base64[j++] = base64char[(int)trans_index];

			base64[j++] = padding_char;

			base64[j++] = padding_char;

			break; 
		}
		trans_index = ((sourcedata[i + 1] << 2) & 0x3c);
		if (i + 2 < datalength)
		{ 
			trans_index |= ((sourcedata[i + 2] >> 6) & 0x03);
			base64[j++] = base64char[(int)trans_index];

			trans_index = sourcedata[i + 2] & 0x3f;
			base64[j++] = base64char[(int)trans_index];
		}
		else
		{
			base64[j++] = base64char[(int)trans_index];

			base64[j++] = padding_char;

			break;
		}
	}
	base64[j] = '\0';
	return 0;
}
/*解码*/
int num_strchr(const char *str, char c)
{
	const char *pindex = strchr(str, c);
	if (NULL == pindex)
	{
		return -1;
	}
	return pindex - str;
}
/* 解码*/
int base64_decode(const char *base64, unsigned char *dedata)
{
	int i = 0, j = 0;
	int trans[4] = {0, 0, 0, 0};
	for (; base64[i] != '\0'; i += 4)
	{
		trans[0] = num_strchr(base64char, base64[i]);
		trans[1] = num_strchr(base64char, base64[i + 1]);
		dedata[j++] = ((trans[0] << 2) & 0xfc) | ((trans[1] >> 4) & 0x03);

		if (base64[i + 2] == '=')
		{
			continue;
		}
		else
		{
			trans[2] = num_strchr(base64char, base64[i + 2]);
		}
		dedata[j++] = ((trans[1] << 4) & 0xf0) | ((trans[2] >> 2) & 0x0f);

		if (base64[i + 3] == '=')
		{
			continue;
		}
		else
		{
			trans[3] = num_strchr(base64char, base64[i + 3]);
		}
		dedata[j++] = ((trans[2] << 6) & 0xc0) | (trans[3] & 0x3f);
	}
	dedata[j] = '\0';
	return 0;
}
/*获取文件大小*/
int get_file_size(char *filename)
{
	struct stat statbuf;
	stat(filename, &statbuf);
	int size = statbuf.st_size;
	return size;
}
/*上传文件*/
void send_file(int sockfd,int uid)
{
	int fid;
	printf("请输入好友的id:");
	scanf("%d", &fid);
	while ('\n' != (getchar()));
	char filename[80];
	printf("请输入传输的文件名称\n");
	scanf("%s",filename);
    printf("%s",filename);
	int len = strlen(filename);
	//filename[len - 1] = '\0';
	int size = get_file_size(filename);
    printf("%d",size);
    printf("hhhh\n");
	int fp;
	if ((fp = open(filename,O_RDWR)) < 0)
	{
		printf("文件打开失败\n");
	}
    printf("no\n");
	char *buf = (char *)malloc(sizeof(char)*size);
	int ret = 0;
	int num = size;
	while (num != 0)
	{ 
		if((ret = read(fp, buf + ret,num)) < 1);
        printf("%d",ret);
		num -= ret;
	}
    printf("go\n");
	buf[size] = '\0';
	close(fp);
    printf("god\n");
	char *str = (char *)malloc(sizeof(char)*size* 2);  //保存编码后的文件内容
	//base64编码
	base64_encode((const unsigned char *)buf,str,size);
	int _len = strlen(str);
	cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "signal", SEND_FILE);
	cJSON_AddNumberToObject(json, "from_id", uid);
    cJSON_AddNumberToObject(json, "to_id", fid);
	cJSON_AddNumberToObject(json, "f_size", size);
	cJSON_AddNumberToObject(json, "c_size", _len);
	cJSON_AddStringToObject(json, "filename", filename);
    cJSON_AddStringToObject(json, "content", str);
	char *pass = cJSON_PrintUnformatted(json);
	printf("base64::%s\n",pass);
	add_file_size(sockfd,pass);
	free(str);
    free(buf);
    cJSON_Delete(json);
}
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
    int number;
    recv(sockfd, &number, 16, 0);
    printf("fff\n");
    printf("%d\n", number);
    char *temp = (char *)malloc(sizeof(char) * number + 1);
    temp[number] = 0;
    recv(sockfd, temp, number, 0);
    printf("%s\n", temp);
    cJSON *node = cJSON_Parse(temp);
    printf("%s\n", cJSON_GetObjectItem(node, "content")->valuestring);
    cJSON_Delete(json);
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
/*删除好友*/
void delete_usr(int sockfd)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", DELETE_FRIEND);
    int id;
    printf("please enter the id that you want to delete\n");
    scanf("%d", &id);
    cJSON_AddNumberToObject(json, "id", id);
    char *pass = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
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
    printf("%d请求添加你为好友(0 or 1)\n", cJSON_GetObjectItem(node, "uid")->valueint);
    int ch;
    scanf("%d", &ch);
    // cJSON_Delete(node);
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", FRIEND_REPLY);
    cJSON_AddNumberToObject(json, "uid", cJSON_GetObjectItem(node, "uid")->valueint);
    if (ch == 1)
        cJSON_AddNumberToObject(json, "tip", AGREE);
    else
        cJSON_AddNumberToObject(json, "tip", DISAGREE);
    cJSON_AddNumberToObject(json, "pid", cJSON_GetObjectItem(node, "pid")->valueint);
    char *pass = cJSON_PrintUnformatted(json);
    cJSON_Delete(node);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*获得好友列表*/
void get_friend_list(int sockfd, int id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", GET_FRI_LIST);
    cJSON_AddNumberToObject(json, "id", id);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*获得群好友列表*/
int get_group_list(int sockfd, int id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", GET_GROUP_LIST);
    printf("please enter the group id\n");
    int gid;
    scanf("%d", &gid);
    cJSON_AddNumberToObject(json, "gid", gid);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*获得群列表*/
void group_usr_list(int sockfd, int id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", GROUP_USR_LST);
    cJSON_AddNumberToObject(json, "id", id);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*原始的分包函数*/
cJSON *analysis_package(int sockfd)
{
    int number;
    int m = recv(sockfd, &number, 16, 0);
    printf("%d\n", number);
    char *temp = (char *)malloc(sizeof(char) * number + 1);
    temp[number] = 0;
    recv(sockfd, temp, number, 0);
    printf("%s\n", temp);
    cJSON *node = cJSON_Parse(temp);
    free(temp);
    return node;
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
    printf("建群%s\n", pass);
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
/*删除群成员*/
void quit_group(int sockfd, int uid)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", QUIT_GROUP);
    cJSON_AddNumberToObject(json, "uid", uid);
    int gid;
    printf("please enter the id of your group\n");
    scanf("%d", &gid);
    cJSON_AddNumberToObject(json, "gid", gid);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*退群*/
void quit_group_t(int sockfd, int uid)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", QUIT_GROUP_T);
    cJSON_AddNumberToObject(json, "uid", uid);
    int gid;
    printf("please enter the id of your group\n");
    scanf("%d", &gid);
    cJSON_AddNumberToObject(json, "gid", gid);
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
        printf("bbb%s\n", pass);
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
void group_set_no_speaking(int sockfd, int uid)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", NO_SPEAKING);
    printf("请输入要禁言人的id\n");
    int id;
    scanf("%d", &id);
    cJSON_AddNumberToObject(json, "uid", id);
    printf("请输入组账号\n");
    int gid;
    scanf("%d", &gid);
    cJSON_AddNumberToObject(json, "gid", gid);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*解除禁言*/
void group_cancle_no_speaking(int sockfd, int uid)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "signal", SPEAKING);
    printf("请输入对方id\n");
    int id;
    scanf("%d", &id);
    cJSON_AddNumberToObject(json, "uid", id);
    printf("please enter the group id\n");
    int gid;
    scanf("%d", &gid);
    cJSON_AddNumberToObject(json, "gid", gid);
    char *pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd, pass);
    cJSON_Delete(json);
}
/*加群*/
void add_group(int sockfd,int uid)
{
    cJSON * json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"signal",ADD_GROUP);
    cJSON_AddNumberToObject(json,"uid",uid);
    int gid;
    printf("please enter the gid\n");
    scanf("%d",&gid);
    cJSON_AddNumberToObject(json,"gid",gid);
    char * pass = cJSON_PrintUnformatted(json);
    add_file_size(sockfd,pass);
    cJSON_Delete(json);
}
//收文件函数
void rec_file(int sockfd,cJSON * node)
{
	int fp = open("tmp", O_CREAT | O_RDWR | O_APPEND, 0777);
	if (-1 == fp)
		printf("创建文件失败\n");
	char filename[50];
	int fid;
	int f_size;
	strcpy(filename, cJSON_GetObjectItem(node, "filename")->valuestring);
	f_size = cJSON_GetObjectItem(node, "f_size")->valueint;
	int len = cJSON_GetObjectItem(node, "c_size")->valueint;
	char *buf = (char *)malloc(sizeof(char)*len);
	strcpy(buf, cJSON_GetObjectItem(node, "content")->valuestring);
	char *str = (char *)malloc(f_size);
	//解码
	base64_decode(buf,(unsigned char *)str);

	int ret = 0;
	int num = f_size;
	while (num >= 0)
	{ 
		ret = write(fp, buf + ret,num);
		num -= ret;
	}
	buf[f_size] = '\0';
	close(fp);
	if (rename("tmp", filename) < 0)
	{
		printf("重命名失败");
	}
	free(buf);
	free(str);
}
/*二十一世纪得分包函数*/
void *analysis_pack(void *arg)
{
    int flag = 1;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        int sockfd = *(int *)arg;
        int number;
        int m = recv(sockfd, &number, 16, 0);
        if (m == 0)
            break;
       // printf("梳到爆了\n");
       printf("gggggggggggggggggg\n");
        char *temp = (char *)malloc(sizeof(char) * number + 1);
        int sf, c1 = 0, c2 = 0, c3 = 0;
        temp[number] = 0;
        if ((sf = recv(sockfd, temp, number, 0)) > 0)
        {
            // printf("%s,%d\n", temp, sf);
            cJSON *node = cJSON_Parse(temp);
            int item = cJSON_GetObjectItem(node, "signal")->valueint;
            int choice;
          //  printf("新消息到了，是否要查看(0 or 1)\n");
          //  scanf("%d",&choice);
            switch(item)
            {
            case CHAT_PRI:
              //  strcpy(pmsg[i++],cJSON_GetObjectItem(node,"content")->valuestring);
               // if(choice != 0)
                printf("%s\n",cJSON_GetObjectItem(node,"content")->valuestring);
               // count++;
                break;
            case DELETE_FRIEND:
                flag = 0;
                printf("成功删除\n");
                break;
            case ADD_USR:
                flag = 0;
                printf("成功添加\n");
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
                //printf("%d发了一条群消息\n", cJSON_GetObjectItem(node, "pid")->valueint);
                printf("%s\n", cJSON_GetObjectItem(node, "content")->valuestring);
                break;
            case SPEAKING:
                flag = 0;
                printf("%d已被解除禁言\n", cJSON_GetObjectItem(node, "id")->valueint);
                break;
            case NO_SPEAKING:
                flag = 0;
                printf("%d已被成功禁言\n", cJSON_GetObjectItem(node, "id")->valueint);
                break;
            case CREAT_GROUP:
                flag = 0;
                printf("建群成功\n");
                break;
            case GET_FRI_LIST:
                if (c1 == 0)
                    printf("pid\tstatus\n");
                printf("%d\t%d\n", cJSON_GetObjectItem(node, "pid")->valueint, cJSON_GetObjectItem(node, "status")->valueint);
                c1++;
                break;
            case GET_GROUP_LIST:
                if (c2 == 0)
                    printf("uid\n");
                printf("%d\n", cJSON_GetObjectItem(node, "id")->valueint);
                c2++;
                break;
            case GROUP_USR_LST:
                if (c3 == 0)
                    printf("%s\t%d\n", cJSON_GetObjectItem(node, "name")->valuestring,
                           cJSON_GetObjectItem(node, "gid")->valueint);
                c3++;
                break;
            case QUIT_GROUP:
                printf("删除成功\n");
                break;
            case QUIT_GROUP_T:
                printf("退群成功\n");
                break;
            case ADD_GROUP:
                printf("加群成功\n");
                break;
            case SEND_BACK:
                rec_file(sockfd,node);
                break;
            default:
                flag = 0;
                printf("好戏即将开始\n");
                break;
            }
            if (flag == 0)
                break;
            cJSON_Delete(node);
        }
        pthread_mutex_unlock(&mutex);
        free(temp);
    }
}
/*菜单选择函数*/
void menu_select(int sockfd, int id)
{
    pthread_t thid1, thid2;
    pthread_create(&thid1, NULL, analysis_pack, (void *)&sockfd);
    printf("\t\t\t*********1.删除好友********\t\t\t\n");
    printf("\t\t\t*********2.添加好友*******\t\t\t\n");
    printf("\t\t\t*********3.回复好友*******\t\t\t\n");
    printf("\t\t\t*********4.获取好友列表***\t\t\t\n");
    printf("\t\t\t*********5.私聊*******\t\t\t\n");
    printf("\t\t\t*********6.创群*******\t\t\t\n");
    printf("\t\t\t*********7.群聊**********\t\t\t\n");
    printf("\t\t\t*********8.禁言*******\t\t\t\n");
    printf("\t\t\t*********9.解除禁言*******\t\t\t\n");
    printf("\t\t\t*********10.获得群好友列表*******\t\t\t\n");
    printf("\t\t\t*********11.获得用户所在群列表*******\t\t\t\n");
    printf("\t\t\t*********12.删除群成员**********\t\t\t\n");
    printf("\t\t\t*********13.退群 **********\t\t\t\n");
     printf("\t\t\t*********14.加群**********\t\t\t\n");
      printf("\t\t\t*********15.上传文件**********\t\t\t\n");
    printf("\t\t\t*********1６.退出*******\t\t\t\n");
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
        get_friend_list(sockfd, id);
        break;
    case 5:
        chat_private(sockfd, id);
        break;
    case 6:
        create_group(sockfd, id);
        break;
    case 7:
        chat_group(sockfd, id);
        break;
    case 8:
        group_set_no_speaking(sockfd, id);
        break;
    case 9:
        group_cancle_no_speaking(sockfd, id);
        break;
        /*  case 10:
           // add_usr(sockfd);
            break;*/
    case 10:
        get_group_list(sockfd, id);
        break;
    case 11:
        group_usr_list(sockfd, id);
        break;
    case 12:
        quit_group(sockfd, id);
        break;
    case 13:
        quit_group_t(sockfd, id);
        break;
    case 14:
        add_group(sockfd,id);
        break;
    case 15:
         send_file(sockfd,id);
         break;
    default:
        printf("输入错误，请重新输入\n");
        break;
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
    uid = id;
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
    int number;
    int m = recv(sockfd, &number, 16, 0);
    printf("fff\n");
    printf("%d\n", number);
    char *temp = (char *)malloc(sizeof(char) * number + 1);
    temp[number] = 0;
    recv(sockfd, temp, number, 0);
    printf("%s\n", temp);
    cJSON *node = cJSON_Parse(temp);
    if (strcmp(cJSON_GetObjectItem(node, "flag")->valuestring, "login successfully!!") == 0)
        while (1)
            menu_select(sockfd, id);
    cJSON_Delete(node);
    cJSON_Delete(json);
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
    char *p = "127.0.0.1";
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
    inet_pton(AF_INET, p, &sever_addr.sin_addr);
    int ret = connect(sockfd, (struct sockaddr *)&sever_addr, sizeof(sever_addr));
    if (ret == -1)
    {
        printf("连接失败\n");
        exit(1);
    }
    menu_main(sockfd);
    // while(1)
    //  menu_select(sockfd,uid);
    return 0;
}