/*************************************************************************
	> File Name: serverfun.h
	> Author: 
	> Mail: 
	> Created Time: 2018年08月17日 星期五 08时32分50秒
 ************************************************************************/

#ifndef _SERVERFUN_H
#define _SERVERFUN_H
#endif
extern MYSQL *mysql;


typedef struct smessage
{
	int  type;  

	int  user_ID;

	char ac_number[30];
	char user_name[30];
	char user_password[30];
	char message[50];          //聊天信息

	int  si_ID;

	int  group_ID;
	char  fileblock;
	int   blocknum;
	int g_id;
	int wo_id;
}smg;

struct rmessage
{
	int type;
	int user_ID;
	char f_sum[200];
	char message[50];
	char f_id[200];
	char f_name[200];
	char f_state[200];
	int f_num;
	int g_id;
};

//多线程用法

typedef struct date
{
	char *message;
	int connfd;
}dcan;


//ZH_CE系函数
int database(char *ac,char *pa,char *usr);   //将创建的用户信息写入数据库函数

//void analyse(char *message,int connfd);
void *analyse(void *wokao);         //多线程用法

void zhuce_sr(smg *ff,cJSON *user,int connfd);

void denglu_sr(smg *ff,cJSON *user,int connfd);

int bidatabase(char *ac,char *pa,int *user_ID,int connfd,char *user_name);

void friend_f5_sr(smg *ff,cJSON *user,int connfd);

int fetch_state_byID(int *num,int *xb,int connfd);

int fetch_friend_byID(int user_ID,int *num,int *xb);

void chat_pn_sr(smg *ff,cJSON *user,int connfd,int mask); //向好友发送聊天信息

void add_friend_r(smg *ff, cJSON *user, int connfd);

void add_friend(smg *ff, cJSON *user, int connfd);

void del_friend_sr(smg *ff, cJSON *user, int connfd);

void group_f5_sr(smg *ff, cJSON *user, int connfd);

//int fetch_group_byID(int user_ID, int *num, int *xb,char **state);

void fetch_groupstate_byID(int id,int *num, int *xb,int connfd);

void chat_gr_sr(smg *ff, cJSON *user, int connfd, int mask);

void add_group(smg *ff, cJSON *user, int connfd);

void add_group_r(smg *ff, cJSON *user, int connfd);

void tu_group_sr(smg *ff, cJSON *user, int connfd);

void del_group_sr(smg *ff, cJSON *user, int connfd);

void get_grmember_sr(smg *ff, cJSON *user, int connfd);

void fetch_groupme_byID(int g_id,int *num, int *xb,int connfd);

void get_h_pchat(smg *ff, cJSON *user, int connfd);

void get_h_gchat(smg *ff, cJSON *user, int connfd);

void send_file_sr(smg *ff, cJSON *user, int connfd);

void zhuxiao(int connfd);