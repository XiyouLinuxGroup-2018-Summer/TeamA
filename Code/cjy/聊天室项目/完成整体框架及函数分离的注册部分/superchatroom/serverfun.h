/*************************************************************************
	> File Name: serverfun.h
	> Author: 
	> Mail: 
	> Created Time: 2018年08月17日 星期五 08时32分50秒
 ************************************************************************/

#ifndef _SERVERFUN_H
#define _SERVERFUN_H
#endif

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
}smg;

struct rmessage
{
	int type;
	int user_ID;
	char message[50];
};


//ZH_CE系函数
int database(char *ac,char *pa,char *usr);   //将创建的用户信息写入数据库函数

void analyse(char *message,int connfd);

void zhuce_sr(smg *ff,cJSON *user,int connfd);