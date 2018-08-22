/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 16时56分32秒
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
//#include<cJSON.h>
#include<pthread.h>

int t;

typedef struct {
	int  id;           //用户id
	char username[30]; //用户名
	char password[20]; //用户密码
} account_t;

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


void denglu() {
	account_t usr;
	int newUsrCount = 0;
	char choice;
	//char *jian;
	char passwd[20];
	int l;
	do
	{
		system("clear");
		//kuangjia();
		//goto_xy(0, 5);
		//printf("\n\t\t\t\t    =======================================================\n");
		//printf("\t\t\t\t       ****************  添加一个新用户  ****************\n");
		//printf("\t\t\t\t    -------------------------------------------------------\n");
		printf("请输入新的用户名:");
		fgets(usr.username, 29, stdin);
		int l = strlen(usr.username);
		usr.username[l - 1] = '\0';
		//getchar();
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
		
		printf("\t    [A]继续添加   [R]返回");
		choice = getchar();
		//getchar();
	} while ('a' == choice || 'A' == choice);
}

int main(void)
{
    Account_UI_Add();
    return 0;
}


