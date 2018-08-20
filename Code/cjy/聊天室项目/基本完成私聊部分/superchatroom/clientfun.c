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


char friend[200][3][200];   //将好友数组声明为全局变量
int fxb;               //唯一好友下标    


//发送宏
#define ZH_CE 0  //注册
#define DE_LU 1  //登录
#define FR_SX 2  //请求好友信息表
#define CH_PN 3 //私聊

//接收宏       R代表接收
#define  R_ZH_CE_W 0   //注册失败
#define  R_ZH_CE_R 1   //注册成功
#define  R_DE_LU_W 2   //登陆失败
#define  R_DE_LU_R 3   //登陆成功
#define  R_FR_SX_F 4   //首次接受好友包
#define  R_FR_SX_C 5   //正常接受好友包  
#define  R_CH_PN_Z 6   //收到好友在线回复


int user_ID; //用户唯一主键

//客户端分析函数
void analyse(char *message,int fd)
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
		case R_DE_LU_R:
			user_ID=cJSON_GetObjectItem(user,"user_ID")->valueint;
			printf("\n登录成功\n");
			printf("即将进入系统\n");
			//进入主界面
			sleep(2);
			menue(fd);       //进入主界面
			break;

		case R_DE_LU_W:
			sleep(2);
			printf("\n登录失败\n");
			break;

		case R_FR_SX_F:
			sleep(2);
			printf("\n即将进入好友聊天界面\n");
			//好友聊天界面函数
			friend_menue(&ff,user,fd);
			break;
		case R_FR_SX_C:
			friend_print(&ff,user,fd);
			break;
		case R_CH_PN_Z:
			printf("%s\n",cJSON_GetObjectItem(user,"message")->valuestring);
			break;
	}	
}

void friend_print(rmg *ff,cJSON *user,int fd)
{
	char *name;
	strcpy(friend[fxb][0],cJSON_GetObjectItem(user,"f_id")->valuestring);
	strcpy(friend[fxb][1],cJSON_GetObjectItem(user,"f_name")->valuestring);
	strcpy(friend[fxb][2],cJSON_GetObjectItem(user,"f_state")->valuestring);

	//printf("id= %s     name= %s    state= %s\n",friend[fxb][0],friend[fxb][1],friend[fxb][2]);
	fxb++;
	//sleep(1);
}

void friend_menue(rmg *ff,cJSON *user,int fd)
{
	printf("\t\t\t已经入好友聊天界面\n");
	system("clear");
	char *name;
	int sum=cJSON_GetObjectItem(user,"f_num")->valueint;
	//printf("好友的数量为%d\n",sum);

	//打印好友部分
	for(int i=0;i<sum;i++)
	{
		pk_r(&name,fd);
		analyse(name,fd);
	}

	char ch;
	//选项接口部分
	do
	{

		system("clear");

		printf("id        name        state\n");
		for(int i=0;i<fxb;i++)
		{
			printf("id= %s     name= %s    state= %s\n",friend[i][0],friend[i][1],friend[i][2]);
		}


		printf("请选择\n");
		printf("\n\n\n\n");
		printf("[A]加好友 | [D]删好友|[C]私聊 |[Q] 退出登录\n");
		printf("请输入你的选择:");

		scanf("%c",&ch);
		getchar();
		if(ch=='A'||ch=='a') {
			
		}
		if(ch=='C'||ch=='c')
		{
			int fr_id;
			printf("\n请输入你想要私聊的好友的id:");
			scanf("%d",&fr_id);
			getchar();
			chat_pn(fr_id,fd);
		}

	}while(ch!='Q'&&ch!='q');
}

void chat_pn(int fr_id,int fd)
{
	system("clear");
	printf("\n\n\n\n聊天界面");

	pthread_t pid;
	pthread_create(&pid,0,(void *)chat_rp,(void *)&fd);     //执行打印工作
	char msg[100];
	do
	{
		scanf("%s",msg);
		if(strcmp(msg,"exit")==0)
		{
			break;
		}
		//发数据
		//char *chatdata;
		cJSON * smessage = cJSON_CreateObject();
		cJSON_AddNumberToObject(smessage,"type",CH_PN);
		cJSON_AddNumberToObject(smessage,"cfr_ID",fr_id);
		cJSON_AddStringToObject(smessage,"message",msg);		

		pk_s(smessage,fd);
		/*必须把缓冲区清空*/
		memset(msg,0,strlen(msg));
	}while(1);

	return;
}

//开线程执行接受任务
void chat_rp(void *fd)
{
	while(1)
	{
		//char buf[100];
		char *name;
		pk_r(&name,*((int *)fd));
		analyse(name,*((int *)fd));
		//printf("%s\n",buf);
	}
}

//好友与群选择界面
void menue(int fd)
{
	char ch;
	do
	{
		//friend_f5(fd);
		system("clear");
		printf("请选择\n");

		printf("\n\n\n\n");
		printf("[A]好友 | [B]群\n");
		

		printf("请输入你的选择:");

		scanf("%c",&ch);
		getchar();
		if(ch=='A'||ch=='a') {
			friend_f5(fd);
		}
		//else if(ch=='B'||ch=='b')

	}while(ch!='Q'&&ch!='q');	
}


//伪好友选择界面入口
void friend_f5(int fd)
{
	char *name;
	cJSON * smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage,"type",FR_SX);
	cJSON_AddNumberToObject(smessage,"user_ID",user_ID);

	//发包
	pk_s(smessage,fd);
	//收包
	pk_r(&name,fd);
	//解析收包
	analyse(name,fd);
}


int GetPassword(char password[])
{
	char ch;
	int i = 0;
	while ((ch = getch()) != '\n')                        // sleep(1);

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
		//cJSON * tmp=cJSON_CreateObject();

		cJSON_AddNumberToObject(smessage,"type",ZH_CE);
		cJSON_AddNumberToObject(smessage,"user_ID",1);
		cJSON_AddStringToObject(smessage,"ac_number",usr.ac_number);
		cJSON_AddStringToObject(smessage,"user_password",usr.password);
		cJSON_AddStringToObject(smessage,"user_name",usr.username);

		//发包
		pk_s(smessage,fd);
		//收包
		pk_r(&name,fd);
		//printf("完成收包\n");
		//解析收包
		analyse(name,fd);

		printf("\n\t    [A]继续添加   [R]返回");
		choice = getchar();
		getchar();
	} while ('a' == choice || 'A' == choice);
}

void denglu_cl(int fd)
{
	char *name;
	char ac_number[20];
	char password[20];
	int i = 0, j;

	system("clear");
	printf("       ------->>            请登录           <<------\n");
	printf("==============================================================\n");
	//for (i = 1; i <= 3; i++) 
	//{
	printf("\t\t\t\t******** 请输入你的账号: ");
	fgets(ac_number, 19, stdin);
	int l = strlen(ac_number);
	ac_number[l - 1] = '\0';
	printf("\t\t\t\t******** 请输入你的密码: ");
	GetPassword(password);
	printf("\n\t\t\t\t==========================================================\n");
	
	cJSON * smessage = cJSON_CreateObject();
	cJSON * tmp=cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage,"type",DE_LU);
	cJSON_AddStringToObject(smessage,"ac_number",ac_number);
	cJSON_AddStringToObject(smessage,"user_password",password);

	//发包
	pk_s(smessage,fd);

	//收包
	pk_r(&name,fd);

	//解析收包
	analyse(name,fd);
	//}
	//验证登录账号是否已存在，存在
	/*
	if ()
	{
		printf("\t\t\t\t      ");
		printf("正在登陆，请稍等");
		for (j = 0; j < 23; j++)
		{
			Sleep(100);
			printf(".");
		}
		printf("\n");
		return 1;
	}
	else if (i ==3)
	{
		return 0;
	}
	else
	{
		printf("\t\t\t\t\t    ");
		for (j = 0; j < 11; j++)
		{
			Sleep(100);
			printf(".");
		}
		printf("\n");
		for (int o = i * 10 - 5; o >= 0; o--)
		{
			goto_xy(0, 13);
			printf("\t\t\t\t身份验证错误!!!   还有%d次机会   请%d秒后重试----还剩%d秒\n", 4 - i, i * 10 - 5, o);
			Sleep(1000);
		}
	*/
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







		
