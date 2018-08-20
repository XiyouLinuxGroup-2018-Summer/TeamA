/*************************************************************************
	> File Name: clientfun.h
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 19时44分53秒
 ************************************************************************/

#ifndef _CLIENTFUN_H
#define _CLIENTFUN_H
#endif

//#include"cJSON.h"

typedef struct {
	int  id;//用户id
	
	char ac_number[30];//账号
	char password[30];//用户密码
	char username[30];//用户名
} account_t;

struct smessage
{
	int  type;  

	int  user_ID;
	char user_name[30];
	char user_password[30];
	char message[200];          //聊天信息

	int  cfr_ID;

	int  group_ID;
	char  fileblock;
	int   blocknum;
};

typedef struct rmessage
{
	int type;
	int user_ID;
	char f_sum[200];
	char message[50];
	char f_id[200];
	char f_name[200];
	char f_state[200];
	int f_num;
}rmg;

typedef struct friend
{
	char f_id[200];
	char f_name[200];
	char f_state[200];
	struct friend *next,*prev;
}friend_node_t,*friend_list_t;



int GetPassword(char password[]);
void zhuce_cl(int fd);
//int getch();
void analyse(char *message,int fd);

void denglu_cl(int fd);

void menue(int fd);

void friend_f5(int fd);

void friend_print(rmg *ff,cJSON *user,int fd);

void friend_menue(rmg *ff,cJSON *user,int fd);

//void r_zhuce_cl(rmg *ff,cJSON *user);

void chat_rp(void *fd);

void chat_pn(int fr_id,int fd);