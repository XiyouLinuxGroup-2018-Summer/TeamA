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
	int path;            //环境变量
}rmg;


int GetPassword(char password[]);
void zhuce_cl(int fd);
//int getch();
int analyse(char *message,int fd,int now_path);

void denglu_cl(int fd);

void menue(int fd);

void friend_f5(int fd);

void friend_print(rmg *ff,cJSON *user,int fd);

void friend_menue(rmg *ff,cJSON *user,int fd);

//void r_zhuce_cl(rmg *ff,cJSON *user);

void *chat_rp(void *fd);

void *chat_rg(void *fd);

void chat_pn(int fr_id,int fd);

void *ac_bao(void *fd);  //总收包函数

void view(int fd);  //主菜单

void B_line(int fd,int path); //遍历队列函数

void add_fr_re(rmg *ff, cJSON *user, int fd);

void add_friend(int fr_id, int fd);

void del_friend(int fr_id, int fd);

void group_begin(int fd);

void group_print(rmg *ff, cJSON *user, int fd);

void group_menue(rmg *ff, cJSON *user, int fd);

void chat_gr(int g_id, int fd);

int new_report(int fd);

void add_group(int g_id, int fd);

void add_gr_re(rmg *ff, cJSON *user, int fd);

void tui_group(int g_id, int fd);

void san_gr(int g_id,int fd);

void cha_print(rmg *ff, cJSON *user, int fd);

void cha_gr(int g_id,int fd);

void cha_menu(rmg *ff, cJSON *user, int fd);

void cha_pchat(int fr_id, int fd);

void hpcha_menu(rmg *ff, cJSON *user, int fd);

void hpcha_print(rmg *ff, cJSON *user, int fd);
