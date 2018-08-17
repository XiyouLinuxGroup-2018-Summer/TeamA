/*************************************************************************
	> File Name: clientfun.h
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 19时44分53秒
 ************************************************************************/

#ifndef _CLIENTFUN_H
#define _CLIENTFUN_H
#endif

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
	char message[50];          //聊天信息

	int  si_ID;

	int  group_ID;
	char  fileblock;
	int   blocknum;
};

int GetPassword(char password[]);
void zhuce_cl(int fd);
int getch();