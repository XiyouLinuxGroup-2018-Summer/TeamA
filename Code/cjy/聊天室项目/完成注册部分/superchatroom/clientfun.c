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

#define ZH_CE 0

int getch()//getch实现
{
    int c=0;
    struct termios org_opts, new_opts;
    int res=0;
    //-----  store old settings -----------
     res=tcgetattr(STDIN_FILENO, &org_opts);
     assert(res==0);
    //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c=getchar();
    //------  restore old settings ---------
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res==0);
    return c;
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

		char *tmp_out= cJSON_Print(smessage);
		tmp_out= cJSON_PrintUnformatted(smessage);// 生成适合传输的JSON字符串

		int number = strlen(tmp_out); 

		char package[number+4];
		strcpy(package+4,tmp_out);
		*(int*)package=number;
		/*
		printf("number=%d\n",number);
		printf("package4=%d\n",*(int *)package);
		printf("json=%s\n",package+4);	
		*/
		send(fd,package,number + 4,0);

		printf("\n\t    [A]继续添加   [R]返回");
		choice = getchar();
		getchar();
	} while ('a' == choice || 'A' == choice);
}

		
