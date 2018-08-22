/*************************************************************************
	> File Name: clientfun.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月20日 星期一 15时31分22秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "cJSON.h"
#include <pthread.h>

#include <queue> //模拟队列

#include "clientfun.h"
#include "public.h"

char f_friend[200][3][200]; //将好友数组声明为全局变量
int fxb;					//唯一好友下标

char f_group[200][3][200];  //将群数组声明为全局变量
int gxb;					//唯一群下标

char m_group[200][4][200];  //将群数组声明为全局变量
int mxb;                    //唯一群下标


int af_flag;

//创建收包队列
queue<char *> fuck; //背下,初始化队列
queue<char *> z_fuck;
//struct mi A;

//启用互斥锁
pthread_mutex_t fuck_mutex;

//发送宏
#define ZH_CE 0   //注册
#define DE_LU 1   //登录
#define FR_SX 2   //请求好友信息表
#define CH_PN 3   //私聊
#define CH_TC 4   //自行退出
#define AD_FR 5   //添加好友
#define AD_FR_R 6 //发送好友回复信息
#define DE_FR 7   //删除好友

#define GR_SX 8	  //请求好友信息表
#define GR_CH_PN 9 //群聊
#define GR_CH_TC 10 //群聊自行退出
#define AD_GR    11 //加群
#define AD_GR_R  12 //发送加群回复信息
#define TU_GR    13  //退群
#define DE_GR    14  //解散群
#define GR_M_SX  15  //请求群成员信息表
   

//接收宏       R代表接收
#define R_ZH_CE_W 0		//注册失败
#define R_ZH_CE_R 1		//注册成功
#define R_DE_LU_W 2		//登陆失败
#define R_DE_LU_R 3		//登陆成功
#define R_FR_SX_F 4		//首次接受好友包
#define R_FR_SX_C 5		//正常接受好友包
#define R_CH_PN_Z 6		//收到好友在线回复
#define R_CH_PN_T 7		//自己退出聊天
#define R_ADD_FR 8		//给对象发送加好友信息
#define D_R_ADD_FR_R 9  //对方答复后服务器答复给我 成功
#define D_R_ADD_FR_W 10 //失败

#define R_GR_SX_F 11    //首次接收群包 
#define R_GR_SX_C 12	//正常接收群包
#define R_CH_GR_Z 13    //用户收到好友在线回复
#define R_CH_GR_T 14    //用户自己退出聊天
#define R_ADD_GR  15    //向群主发送申请消息
#define D_R_ADD_GR_R 16   //群主答复后服务器答复给我 成功
#define D_R_ADD_GR_W 17   //失败
#define R_GR_M_SC_F 18  //首次接收群成员数量包
#define R_GR_M_SC_C 19  //正常接收群包

//环境变量
#define DE_PATH 0	//登录环境变量
#define FR_PATH 1	//选择好友登录界面环境变量
#define CS_P 2		 //打印私聊消息环境变量
#define AF_TS_PATH 3 //打印加好友信息环境变量
#define GR_PATH 4  //打印群信息环境变量
#define CS_G 5     //群聊环境变量
#define GR_TS_PATH 6 //打印加群信息环境变量
#define GR_CHA_PATH 7 //显示群成员函数
//#define GR_B_CHA_PATH 8  //显示群成员函数入口

int now_path; //可变环境变量

int user_ID; //用户唯一主键

//客户端分析函数
int analyse(char *message, int fd, int now_path)
{
	//解析JSON
	rmg ff;
	cJSON *user = cJSON_Parse(message);

	ff.type = cJSON_GetObjectItem(user, "type")->valueint;
	ff.path = cJSON_GetObjectItem(user, "path")->valueint;

	if (now_path != ff.path)
		return -1; //环境变量不匹配时返回-1

	switch (ff.type)
	{
	case R_ZH_CE_R:
		printf("\n注册成功\n");
		break;
	case R_ZH_CE_W:
		printf("\n注册失败\n");
		break;
	case R_DE_LU_R:
		user_ID = cJSON_GetObjectItem(user, "user_ID")->valueint;    //客户多获得当前登陆者id
		printf("\n登录成功\n");
		printf("即将进入系统\n");
		//进入主界面
		//sleep(2);
		menue(fd); //进入主界面
		break;//请求好友信息表

	case R_DE_LU_W:
		sleep(2);
		printf("\n登录失败\n");
		break;

	case R_FR_SX_F:
		sleep(2);
		printf("\n即将进入好友聊天界面\n");
		//好友聊天界面函数
		//friend_menue(&ff, user, fd);
		break;
	case R_FR_SX_C:
		//friend_print(&ff, user, fd);
		break;
	case R_CH_PN_Z:
		printf("%s\n", cJSON_GetObjectItem(user, "message")->valuestring);
		//if (strcmp(cJSON_GetObjectItem(user, "message")->valuestring, "对方已退出") == 0)
		//flag = 1;
		break;
	case R_CH_PN_T:
		printf("自己已退出\n");
		return 1;
		break;
	case R_ADD_FR:
		//add_fr_re(&ff, user, fd);
		break;
	case R_GR_SX_F:
		group_menue(&ff, user, fd);
		break;
	case R_GR_SX_C:
		group_print(&ff, user, fd);
		break;
	case R_CH_GR_Z:
		printf("%s\n", cJSON_GetObjectItem(user, "message")->valuestring);
		//if (strcmp(cJSON_GetObjectItem(user, "message")->valuestring, "对方已退出") == 0)
		//flag = 1;
		break;
	case R_CH_GR_T:
		printf("自己已退出\n");
		return 1;
		break;	
	case R_ADD_GR:
		add_gr_re(&ff, user, fd);
		break;
	case R_GR_M_SC_F:
		cha_menu(&ff, user, fd);
		break;
	case R_GR_M_SC_C:
		cha_print(&ff, user, fd);
	}	
	return 0;
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

		scanf("%c", &ch);
		while ('\n' != (getchar()));
		/*
		if (ch == 'A' || ch == 'a')
		{
			friend_f5(fd);
		}
		*/
		//else
		if(ch=='B'||ch=='b')
		{
			group_begin(fd);
		}

	} while (ch != 'Q' && ch != 'q');
}

void cha_print(rmg *ff, cJSON *user, int fd)
{
	strcpy(m_group[mxb][0], cJSON_GetObjectItem(user, "user_id")->valuestring);
	strcpy(m_group[mxb][1], cJSON_GetObjectItem(user, "user_name")->valuestring);
	strcpy(m_group[mxb][2], cJSON_GetObjectItem(user, "user_state")->valuestring);
	strcpy(m_group[mxb][3], cJSON_GetObjectItem(user, "user_zhiwu")->valuestring);	

	//printf("id= %s     name= %s    state= %s\n",friend[fxb][0],friend[fxb][1],friend[fxb][2]);
	mxb++;
	//sleep(1);
}

//查询群成员函数
void cha_gr(int g_id,int fd)
{
	now_path = GR_CHA_PATH;
	char *name;
	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type",GR_M_SX);
	cJSON_AddNumberToObject(smessage, "user_ID", user_ID);
	cJSON_AddNumberToObject(smessage, "g_ID", g_id);
	//发包
	pk_s(smessage, fd);
	B_line(fd, now_path);
}

//查询群成员主界面
void cha_menu(rmg *ff, cJSON *user, int fd)
{
	now_path = GR_CHA_PATH; //环境变量

	system("clear");
	//rmg ff;
	//cJSON *user = cJSON_Parse(message);
	int sum = cJSON_GetObjectItem(user, "g_num")->valueint;
	printf("群成员的数量为%d\n",sum);

	//打印群部分
	for (int i = 0; i < sum; i++)
	{
		printf("进入循环\n");
		B_line(fd, now_path);
	}
	char ch;
	do
	{
		system("clear");
		printf("用户id        用户名        职务       在线状态\n");
		for (int i = 0; i < mxb; i++)
		{
			printf("id= %s     name= %s    职务= %s     在线状态=%s\n", m_group[i][0], m_group[i][1], m_group[i][3],m_group[i][2]);
		}

		scanf("%c", &ch);
		while ('\n' != (getchar()));		

	}while(ch!='q'||ch!='Q');
}

//解散群
void san_gr(int g_id,int fd)
{
	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", DE_GR);
	cJSON_AddNumberToObject(smessage, "dl_g_id", g_id);
	cJSON_AddNumberToObject(smessage,"wo_id",user_ID);
	pk_s(smessage, fd);	
}


//退群
void tui_group(int g_id, int fd)
{
	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", TU_GR);
	cJSON_AddNumberToObject(smessage, "tu_g_id", g_id);
	pk_s(smessage, fd);
}

//添加好友信息回馈函数
void add_gr_re(rmg *ff, cJSON *user, int fd)
{
	char g_name[200];
	strcpy((*ff).f_name, cJSON_GetObjectItem(user, "user_name")->valuestring);
	strcpy(g_name,cJSON_GetObjectItem(user, "g_name")->valuestring);

	int jia_ID;
	jia_ID = cJSON_GetObjectItem(user, "jia_ID")->valueint;

	int g_id;
	g_id = cJSON_GetObjectItem(user, "g_id")->valueint;


	printf("\n用户名为%s的用户请求加入 %s 群，您是否同意?\n",(*ff).f_name,g_name);
	printf("\n\n\nY[同意]   |   N[拒绝]\n");

	char ch;

	scanf("%c", &ch);
	while ('\n' != (getchar()))
		;

	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", AD_GR_R);
	cJSON_AddStringToObject(smessage, "user_name", (*ff).f_name);
	cJSON_AddNumberToObject(smessage, "jia_ID", jia_ID);
	cJSON_AddNumberToObject(smessage, "g_id", g_id);

	if (ch == 'y' || ch == 'Y')
	{
		cJSON_AddNumberToObject(smessage, "add_g_r", 1);
	}
	else if (ch == 'n' || ch == 'N')
	{
		cJSON_AddNumberToObject(smessage, "add_g_r", 0);
	}
	pk_s(smessage, fd);
}



//添加群函数
void add_group(int g_id, int fd) 
{
	char g_number[30];

	printf("请输入你想要添加的群号码\n:");

	fgets(g_number, 29, stdin);

	int l = strlen(g_number);
	g_number[l - 1] = '\0';

	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", AD_GR);
	cJSON_AddStringToObject(smessage, "g_number", g_number);
	pk_s(smessage, fd);
	printf("群请求已经发送，请耐心等待\n");
}


//群聊函数
void chat_gr(int g_id, int fd) 
{
	system("clear");
	printf("\n\n\n\n聊天界面");

	
	pthread_t pid;
	pthread_create(&pid, 0, chat_rp, (void *)&fd); //执行打印工作
	//printf("已执行\n");
	

	char msg[100];
	while (1)
	{
		scanf("%s", msg);
		while ('\n' != (getchar()));
		if (strcmp(msg, "exit") == 0)
		{
			cJSON *smessage = cJSON_CreateObject();
			cJSON_AddNumberToObject(smessage, "type", GR_CH_TC);
			cJSON_AddNumberToObject(smessage, "g_ID", g_id);
			//cJSON_AddNumberToObject(smessage, "flag", flag);

			char tsg[100];
			sprintf(tsg, "jiange已退出");
			cJSON_AddStringToObject(smessage, "message", tsg);
			pk_s(smessage, fd);
			return;
		}
		//发数据
		//char *chatdata;
		

		cJSON *smessage = cJSON_CreateObject();
		cJSON_AddNumberToObject(smessage, "type", GR_CH_PN);
		cJSON_AddNumberToObject(smessage, "g_ID", g_id);
		cJSON_AddStringToObject(smessage, "message", msg);

		pk_s(smessage, fd);

		memset(msg, 0, strlen(msg)); //清楚缓冲区
	}
	return;
}

//开线程执行接受任务
void *chat_rp(void *fd)
{
	pthread_mutex_lock(&fuck_mutex);
	printf("开始执行打印函数\n");

	now_path = CS_G;

	while (1)
	{
		//char buf[100];
		char *name;

		int key;
		int wai = 0;
		while (wai == 0)
		{
			while (!fuck.empty())
			{	
				now_path = CS_G;
				printf("已进入副线程\n");
				char *tmp;
				tmp = fuck.front();
				fuck.pop();
				printf("已获取队首元素 为%s\n",tmp);
	
				key = analyse(tmp, *((int *)fd), now_path);

				printf("now_path=%d\n",now_path);
				printf("key=%d\n",key);
				sleep(1);
				if (key != -1)
				{
					wai = 1;
					break;
				}
				fuck.push(tmp);
			}
		}
		
		if (key == 1)
		{
			printf("打印聊天线程已关闭\n");
			sleep(2);
			pthread_mutex_unlock(&fuck_mutex);
			break;
		}
		//printf("%s\n",buf);
	}
	pthread_mutex_unlock(&fuck_mutex);
}




//存储群数据
void group_print(rmg *ff, cJSON *user, int fd)
{
	printf("已进入群打印函数\n");
	strcpy(f_group[gxb][0], cJSON_GetObjectItem(user, "g_id")->valuestring);
	strcpy(f_group[gxb][1], cJSON_GetObjectItem(user, "g_name")->valuestring);
	strcpy(f_group[gxb][2], cJSON_GetObjectItem(user, "g_state")->valuestring);
	printf("拷贝完成\n");
	//printf("id= %s     name= %s    state= %s\n",friend[fxb][0],friend[fxb][1],friend[fxb][2]);
	gxb++;
	//sleep(1);	
}

//群的主界面
void group_menue(rmg *ff, cJSON *user, int fd)
{
	now_path = GR_PATH; //环境变量

	printf("\t\t\t已经入群聊天界面\n");
	system("clear");
	char *name;
	int sum = cJSON_GetObjectItem(user, "g_num")->valueint;
	printf("群的数量为%d\n",sum);

	//打印群部分
	for (int i = 0; i < sum; i++)
	{
		printf("进入循环\n");
		B_line(fd, now_path);
	}
	char ch;

	//选项接口部分
	do
	{
		//now_path = AF_TS_PATH;
		system("clear");

		printf("g_id        g_name        state\n");
		for (int i = 0; i < gxb; i++)
		{
			printf("id= %s     name= %s    state= %s\n", f_group[i][0], f_group[i][1], f_group[i][2]);
		}

		printf("请选择\n");
		printf("\n\n\n\n");
		printf("[A]加群 |[B]退群 |[C]群聊 |[N]收消息界面 |[D]解散群 |[F]查看群成员 |[Q] 退出\n");

		/*

		//比较low的接收方法
		int key;
		while (!fuck.empty())
		{
			//printf("已进入\n");
			char *tmp;
			tmp = fuck.front();
			fuck.pop();

			key = analyse(tmp, fd, now_path);
			if (key != -1)
			{
				break;
			}
			fuck.push(tmp);
		}
		*/
		printf("请输入你的选择:");
		scanf("%c", &ch);
		while ('\n' != (getchar()));

		
		if (ch == 'A' || ch == 'a')
		{
			int g_id;
			// printf("\n请输入你想要添加的好友的id:");
			// scanf("%d", &fr_id);
			// while ('\n' != (getchar()));
			add_group(g_id, fd);
		}

		
		if (ch == 'B' || ch == 'b')
		{
			int g_id;
			printf("\n请输入你想要退出的群的id:");
			scanf("%d", &g_id);
			while ('\n' != (getchar()));
			tui_group(g_id, fd);
		}
		

		else if (ch == 'C' || ch == 'c')
		{
			int g_id;
			printf("\n请输入你想要群聊的群的id:");

			scanf("%d", &g_id);
			while ('\n' != (getchar()));
			chat_gr(g_id, fd);
		}

		else if (ch == 'D' || ch == 'd')
		{
			int g_id;
			printf("\n请输入你想要解散的群的id:");

			scanf("%d", &g_id);
			while ('\n' != (getchar()));
			san_gr(g_id, fd);
		}

		else if (ch=='F'||ch=='f')
		{
			int g_id;
			printf("\n请输入你想要查询的群的id:");

			scanf("%d", &g_id);
			while ('\n' != (getchar()));
			cha_gr(g_id, fd);			
		}

		else if(ch=='N'||ch=='n')
		{
			int key;
			key=new_report(fd);
		}		
	}while (ch != 'Q' && ch != 'q');
	gxb = 0;
}

int new_report(int fd)           //消息窗口
{
	system("clear");
	now_path=GR_TS_PATH;

	B_line(fd,now_path);

}

//群界面入口
void group_begin(int fd)
{
	now_path = GR_PATH;
	char *name;
	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", GR_SX);
	cJSON_AddNumberToObject(smessage, "user_ID", user_ID);

	//发包
	pk_s(smessage, fd);

	printf("已进入且发包成功\n");
	//收包
	printf("测试三秒\n");

	B_line(fd, now_path);	
}


int GetPassword(char password[])
{
	char ch;
	int i = 0;
	while ((ch = getch()) != '\n') // sleep(1);

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
		int p = strlen(usr.ac_number);
		usr.ac_number[p - 1] = '\0';
		//getchar();
		printf("%s\n", usr.ac_number);

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

		cJSON *smessage = cJSON_CreateObject();
		//cJSON * tmp=cJSON_CreateObject();

		cJSON_AddNumberToObject(smessage, "type", ZH_CE);
		cJSON_AddNumberToObject(smessage, "user_ID", 1);
		cJSON_AddStringToObject(smessage, "ac_number", usr.ac_number);
		cJSON_AddStringToObject(smessage, "user_password", usr.password);
		cJSON_AddStringToObject(smessage, "user_name", usr.username);

		//发包
		pk_s(smessage, fd);
		//收包
		pk_r(&name, fd);
		//printf("完成收包\n");
		//解析收包
		analyse(name, fd, 100);

		printf("\n\t    [A]继续添加   [R]返回");
		choice = getchar();
		getchar();
	} while ('a' == choice || 'A' == choice);
}

void denglu_cl(int fd)
{
	now_path = DE_PATH;
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

	cJSON *smessage = cJSON_CreateObject();
	cJSON *tmp = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", DE_LU);
	cJSON_AddStringToObject(smessage, "ac_number", ac_number);
	cJSON_AddStringToObject(smessage, "user_password", password);

	//发包
	pk_s(smessage, fd);


	B_line(fd, now_path);
}

//开线程执行接受任务
void *ac_bao(void *fd)
{
	while (1)
	{
		int c;
		char *name;
		pk_r(&name, *((int *)fd));
		fuck.push(name);
		printf("已压入\n");
		z_fuck.push(name);
	}
}

void view(int fd)
{
	pthread_t pid;
	pthread_create(&pid, 0, ac_bao, (void *)&fd); //执行接受包
	do
	{
		system("clear");
		printf("请选择\n");

		printf("\n\n\n\n");
		printf("[A]登录 | [B]注册\n");
		char ch;

		printf("请输入你的选择:");

		scanf("%c", &ch);
		getchar();
		if (ch == 'B' || ch == 'b')
			zhuce_cl(fd);
		else if (ch == 'A' || ch == 'a')
		{
			denglu_cl(fd);
			printf("never go to here\n");
		}

	} while (1);
}

void B_line(int fd, int path)
{
	int key;
	int wai = 0;
	while (wai == 0)
	{
		while (!fuck.empty())
		{
			//printf("已进入\n");
			char *tmp;
			tmp = fuck.front();
			fuck.pop();

			key = analyse(tmp, fd, now_path);
			if (key != -1)
			{
				wai = 1;
				break;
			}
			fuck.push(tmp);
		}
	}
}

///////////////////////////////以下为好友部分函数
/*


//添加好友信息回馈函数
void add_fr_re(rmg *ff, cJSON *user, int fd)
{

	strcpy((*ff).f_name, cJSON_GetObjectItem(user, "user_name")->valuestring);
	int jia_ID;
	jia_ID = cJSON_GetObjectItem(user, "jia_ID")->valueint;

	printf("\n用户名为%s的用户请求与您称为好友，您是否同意?\n", (*ff).f_name);
	printf("\n\n\nY[同意]   |   N[拒绝]\n");

	char ch;

	scanf("%c", &ch);
	while ('\n' != (getchar()))
		;

	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", AD_FR_R);
	cJSON_AddStringToObject(smessage, "user_name", (*ff).f_name);
	cJSON_AddNumberToObject(smessage, "jia_ID", jia_ID);

	if (ch == 'y' || ch == 'Y')
	{
		cJSON_AddNumberToObject(smessage, "add_f_r", 1);
	}
	else if (ch == 'n' || ch == 'N')
	{
		cJSON_AddNumberToObject(smessage, "add_f_r", 0);
	}
	pk_s(smessage, fd);
}


/*

//伪好友选择界面入口
void friend_f5(int fd)
{
	now_path = FR_PATH;
	char *name;
	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", FR_SX);
	cJSON_AddNumberToObject(smessage, "user_ID", user_ID);

	//发包
	pk_s(smessage, fd);

	printf("已进入且发包成功\n");
	//收包
	printf("测试三秒\n");

	//getchar();

	//解析收包
	//printf("已进入且收包成功\n");

	//getchar();
	// pk_r(&name, fd);
	// analyse(name, fd,100);
	B_line(fd, now_path);
}




void friend_print(rmg *ff, cJSON *user, int fd)
{
	char *name;
	strcpy(f_friend[fxb][0], cJSON_GetObjectItem(user, "f_id")->valuestring);
	strcpy(f_friend[fxb][1], cJSON_GetObjectItem(user, "f_name")->valuestring);
	strcpy(f_friend[fxb][2], cJSON_GetObjectItem(user, "f_state")->valuestring);

	//printf("id= %s     name= %s    state= %s\n",friend[fxb][0],friend[fxb][1],friend[fxb][2]);
	fxb++;
	//sleep(1);
}

void friend_menue(rmg *ff, cJSON *user, int fd)
{
	now_path = FR_PATH; //环境变量

	printf("\t\t\t已经入好友聊天界面\n");
	system("clear");
	char *name;
	int sum = cJSON_GetObjectItem(user, "f_num")->valueint;
	//printf("好友的数量为%d\n",sum);

	//flag = 0;
	//打印好友部分
	for (int i = 0; i < sum; i++)
	{
		// pk_r(&name, fd);
		// analyse(name, fd,100);
		printf("进入循环\n");
		B_line(fd, now_path);
	}
	char ch;

	//选项接口部分
	do
	{
		now_path = AF_TS_PATH;
		system("clear");

		printf("id        name        state\n");
		for (int i = 0; i < fxb; i++)
		{
			printf("id= %s     name= %s    state= %s\n", f_friend[i][0], f_friend[i][1], f_friend[i][2]);
		}

		printf("请选择\n");
		printf("\n\n\n\n");
		printf("[A]加好友 | [D]删好友|[C]私聊 |[Q] 退出登录\n");

		//比较low的接收方法
		int key;
		while (!fuck.empty())
		{
			//printf("已进入\n");
			char *tmp;
			tmp = fuck.front();
			fuck.pop();

			key = analyse(tmp, fd, now_path);
			if (key != -1)
			{
				break;
			}
			fuck.push(tmp);
		}

		printf("请输入你的选择:");
		scanf("%c", &ch);
		while ('\n' != (getchar()))
			;

		if (ch == 'A' || ch == 'a')
		{
			int fr_id;
			// printf("\n请输入你想要添加的好友的id:");
			// scanf("%d", &fr_id);
			// while ('\n' != (getchar()));
			add_friend(fr_id, fd);
		}


		if (ch == 'D' || ch == 'd')
		{
			int fr_id;
			printf("\n请输入你想要删除的好友的id:");
			scanf("%d", &fr_id);
			while ('\n' != (getchar()));
			del_friend(fr_id, fd);
		}

		if (ch == 'C' || ch == 'c')
		{
			int fr_id;
			printf("\n请输入你想要私聊的好友的id:");

			scanf("%d", &fr_id);
			while ('\n' != (getchar()))
				;
			chat_pn(fr_id, fd);
		}

	} while (ch != 'Q' && ch != 'q');
	fxb = 0;
}

//删除好友函数
void del_friend(int fr_id, int fd)
{
	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", DE_FR);
	cJSON_AddNumberToObject(smessage, "de_fr_id", fr_id);
	pk_s(smessage, fd);
}

void add_friend(int fr_id, int fd) //添加好友函数
{
	char ac_number[30];

	printf("请输入你想要添加的用户账号:");

	fgets(ac_number, 29, stdin);

	int l = strlen(ac_number);
	ac_number[l - 1] = '\0';

	cJSON *smessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(smessage, "type", AD_FR);
	cJSON_AddStringToObject(smessage, "ac_number", ac_number);
	pk_s(smessage, fd);
	printf("好友请求已经发送，请耐心等待\n");
}

void chat_pn(int fr_id, int fd) //私聊函数
{
	system("clear");
	printf("\n\n\n\n聊天界面");

	pthread_t pid;
	pthread_create(&pid, 0, chat_rp, (void *)&fd); //执行打印工作

	char msg[100];
	while (1)
	{

		scanf("%s", msg);
		while ('\n' != (getchar()));
		if (strcmp(msg, "exit") == 0)
		{
			cJSON *smessage = cJSON_CreateObject();
			cJSON_AddNumberToObject(smessage, "type", CH_TC);
			cJSON_AddNumberToObject(smessage, "cfr_ID", fr_id);
			//cJSON_AddNumberToObject(smessage, "flag", flag);

			char tsg[100];
			sprintf(tsg, "jiange已退出");
			cJSON_AddStringToObject(smessage, "message", tsg);
			pk_s(smessage, fd);
			return;
		}
		//发数据
		//char *chatdata;

		cJSON *smessage = cJSON_CreateObject();
		cJSON_AddNumberToObject(smessage, "type", CH_PN);
		cJSON_AddNumberToObject(smessage, "cfr_ID", fr_id);
		cJSON_AddStringToObject(smessage, "message", msg);

		pk_s(smessage, fd);


		memset(msg, 0, strlen(msg)); //清楚缓冲区
	};
	return;
}

//开线程执行接受任务
void *chat_rp(void *fd)
{
	pthread_mutex_lock(&fuck_mutex);
	printf("开始执行打印函数\n");

	now_path = CS_P;

	while (1)
	{
		//char buf[100];
		char *name;

		int key;
		int wai = 0;
		while (wai == 0)
		{
			while (!fuck.empty())
			{	
				now_path = CS_P;
				//printf("已进入\n");
				char *tmp;
				tmp = fuck.front();
				fuck.pop();

				key = analyse(tmp, *((int *)fd), now_path);
				printf("now_path=%d\n",now_path);
				printf("key=%d\n",key);
				sleep(1);
				if (key != -1)
				{
					wai = 1;
					break;
				}
				fuck.push(tmp);
			}
		}
		
		if (key == 1)
		{
			printf("打印聊天线程已关闭\n");
			sleep(2);
			pthread_mutex_unlock(&fuck_mutex);
			break;
		}
		//printf("%s\n",buf);
	}
	pthread_mutex_unlock(&fuck_mutex);
}

*/


