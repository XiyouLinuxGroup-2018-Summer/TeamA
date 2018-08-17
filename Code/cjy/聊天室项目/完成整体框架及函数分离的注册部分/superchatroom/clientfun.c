/*************************************************************************
	> File Name: clientfun.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 14时30分37秒
 ************************************************************************/

#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"cJSON.h"
#include<pthread.h>

#include"clientfun.h"
#include"public.h"

#define ZH_CE 0

//接收宏
#define  R_ZH_CE_W 0   //注册失败
#define  R_ZH_CE_R 1   //注册成功

//客户端分析函数
void analyse(char *message)
{
	//解析JSON
	rmg ff;
	cJSON *user = cJSON_Parse(message);
	ff.type=cJSON_GetObjectItem(user,"type")->valueint;
	switch(ff.type)
	{
		case R_ZH_CE_R:
			printf("\n注册成功\n");
			break;
		case R_ZH_CE_W:
			printf("\n注册失败\n");
			break;	
	}	
}



int GetPassword(char password[])
{
	char ch;
	int i = 0;
	while ((ch = getch()) != '\n')
	{
		if (ch < 0)
		{
			getch();
			continue;
		}
		if (ch == '\t' || ch == 32)
		{
			continue;
		}
		else if (ch == 8 && i > 0)
		{
			printf("\b \b");
			i--;
			continue;
		}
		else if (ch != 8 && i < 10)
		{
			printf("*");
			password[i] = ch;
			i++;
		}
	}
	password[i] = '\0';
	return i;
}

void zhuce_cl(int fd) 
{
	char *name;
	
	account_t usr;
	int newUsrCount = 0;
	char choice;
	//char *jian;
	char passwd[30];
	int l;
	do
	{
		system("clear");
		//setbuf(stdin,NULL);
		printf("请输入新的用户名:");
		//setbuf(stdin,NULL);
		//getchar();
		fgets(usr.username, 29, stdin);
		int l = strlen(usr.username);
		usr.username[l - 1] = '\0';

		printf("请输入新的账号:");
		//setbuf(stdin,NULL);
		fgets(usr.ac_number, 29, stdin);
		int p= strlen(usr.ac_number);
		usr.ac_number[p - 1] = '\0';
		//getchar();
		printf("%s\n",usr.ac_number);

		printf("请输入新的密码:");		
		GetPassword(usr.password);
		l = strlen(usr.password);
		while (l < 6)
		{
			printf("\n密码不足6位,请重新输入!\n");
			printf("\n请输入新的密码:");
			GetPassword(usr.password);
			l = strlen(usr.password);
		}

		printf("\n请再次输入上面的密码:");
		GetPassword(passwd);

		while (strcmp(usr.password, passwd) != 0)
		{
			printf("\n两次密码不一致，请重新输入！\n");
			printf("\n请输入新的密码:");
			GetPassword(usr.password);
			l = strlen(usr.password);
			while (l < 6)
			{
				printf("\n密码不足6位,请重新输入!\n");
				printf("\n请输入新的密码:");
				GetPassword(usr.password);
				l = strlen(usr.password);
			}
			printf("\n请再次输入上面的密码:");
			GetPassword(passwd);
		}

		cJSON * smessage = cJSON_CreateObject();
		cJSON * tmp=cJSON_CreateObject();

		cJSON_AddNumberToObject(smessage,"type",ZH_CE);
		cJSON_AddNumberToObject(smessage,"user_ID",1);
		cJSON_AddStringToObject(smessage,"ac_number",usr.ac_number);
		cJSON_AddStringToObject(smessage,"user_password",usr.password);
		cJSON_AddStringToObject(smessage,"user_name",usr.username);

		//发包
		pk_s(smessage,fd);

		//收包
		pk_r(&name,fd);

		//解析收包
		analyse(name);

		printf("\n\t    [A]继续添加   [R]返回");
		choice = getchar();
		getchar();
	} while ('a' == choice || 'A' == choice);
}




/*
//分析注册回传函数
void r_zhuce_cl(rmg *ff,cJSON *user)         
{
	int key;
	//(*ff).user_ID = cJSON_GetObjectItem(user,"user_ID")->valueint;
	strcpy((*ff).message,cJSON_GetObjectItem(user,"message")->valuestring);
	printf("%s\n");
	cJSON_Delete(user);
}
*/







		
