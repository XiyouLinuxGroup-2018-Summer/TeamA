/*************************************************************************
	> File Name: serverfun.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 14时30分14秒
 ************************************************************************/

#include <stdio.h>
#include <math.h>
#include "./cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <mysql/mysql.h>

#include "serverfun.h"
#include "public.h"


//接收宏
#define ZH_CE 0 //注册
#define DE_LU 1 //登录
#define FR_SX 2
#define CH_PN 3 //私聊
#define CH_TC 4 //自己已退出
#define AD_FR 5 //添加好友
#define AD_FR_R 6 //收到好友回复信息

//发送宏
#define R_ZH_CE_W 0 //注册失败
#define R_ZH_CE_R 1 //注册成功
#define R_DE_LU_W 2 //登陆失败
#define R_DE_LU_R 3 //登陆成功
#define R_FR_SX_F 4 //首次发送好友包
#define R_FR_SX_C 5 //正常发送好友包
#define R_CH_PN_Z 6 //用户收到好友在线回复
#define R_CH_PN_T 7 //自己退出聊天
#define R_ADD_FR  8 //给对象发送加好友信息
#define D_R_ADD_FR 9  //对方答复后服务器答复给我

//环境变量
#define DE_PATH 0 //登录环境变量
#define FR_PATH 1 //选择好友登录界面环境变量
#define CS_P 2	//打印私聊消息环境变量
#define AF_TS_PATH 3    //打印加好友信息环境变量

//char *message,int connfd
//解析函数
void analyse(void *wokao)
{
	//解析JSONON
	smg ff;
	//cJSON *user = cJSON_Parse(message);
	//ff.type=cJSON_GetObjectItem(user,"type")->valueint;

	//多线程用法启用
	cJSON *user = cJSON_Parse(((dcan *)wokao)->message);
	ff.type = cJSON_GetObjectItem(user, "type")->valueint;
	switch (ff.type)
	{
	case ZH_CE:
		zhuce_sr(&ff, user, ((dcan *)wokao)->connfd);
		break;
	case DE_LU:
		denglu_sr(&ff, user, ((dcan *)wokao)->connfd);
		break;
	case FR_SX:
		friend_f5_sr(&ff, user, ((dcan *)wokao)->connfd);
		break;
	case CH_PN:
		chat_pn_sr(&ff, user, ((dcan *)wokao)->connfd, 0);
		break;
	case CH_TC:
		chat_pn_sr(&ff, user, ((dcan *)wokao)->connfd, 1);
		break;
	case AD_FR:
		add_friend(&ff, user, ((dcan *)wokao)->connfd);
		break;
	case AD_FR_R:
		add_friend_r(&ff, user, ((dcan *)wokao)->connfd);
	}
}

//回复加好友成功与否函数
void add_friend_r(smg *ff, cJSON *user, int connfd)
{
	int jia_fd;
	int key=cJSON_GetObjectItem(user, "add_f_r")->valueint;
	int jia_ID=cJSON_GetObjectItem(user, "jia_ID")->valueint;
	char *jia_name;

	//根据jia_ID找到它的在线端口

	char ha[] = "SELECT * FROM user_list;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit)))
	{
		if (atoi(row[0])==jia_ID)
		{
			jia_fd=atoi(row[5]);
			//strcpy(jia_name,row[3]);
		}	
	}

	cJSON *rmessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(rmessage, "type", D_R_ADD_FR);
	cJSON_AddNumberToObject(rmessage, "path", AF_TS_PATH);
	//cJSON_AddStringToObject(rmessage, "jia_name", jia_name);
	pk_s(rmessage, jia_fd);
	printf("已执行完毕\n");
	sleep(2);
}

//加好友函数
void add_friend(smg *ff, cJSON *user, int connfd)
{
	printf("我的链接端口是%d\n",connfd);
	int zhen_ID;
	//strcpy((*ff).message, cJSON_GetObjectItem(user, "message")->valuestring);
	strcpy((*ff).ac_number,cJSON_GetObjectItem(user, "ac_number")->valuestring);
	//printf("已执行\n");
	int get_connfd;
	//根据接受id向对象用户发送聊天数据
	char ha[] = "SELECT * FROM user_list;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	
	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit)))
	{
		if ((strcmp(row[1],(*ff).ac_number)==0))
		{
			get_connfd = atoi(row[5]);
			sprintf((*ff).user_name,"%s",row[3]);
		}

		// if(connfd==atoi(row[5]))
		// {
		// 	zhen_ID=atoi(row[0]);
		// }
	}

	//char ha[] = "SELECT * FROM user_list;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	MYSQL_RES *kao = mysql_store_result(mysql);
	MYSQL_ROW tow;
	while ((tow = mysql_fetch_row(kao)))
	{
		printf("tow[5]=%d\n",atoi(tow[5]));
		if(connfd==atoi(tow[5]))
		{
			zhen_ID=atoi(tow[0]);
		}
	}


	printf("已执行\n");
	cJSON *rmessage = cJSON_CreateObject();
	cJSON_AddNumberToObject(rmessage, "type", R_ADD_FR);
	cJSON_AddNumberToObject(rmessage, "path", AF_TS_PATH);
	cJSON_AddStringToObject(rmessage, "user_name", (*ff).user_name);
	cJSON_AddNumberToObject(rmessage, "jia_ID", zhen_ID);   //我的id 我是想加好友的那个人
	pk_s(rmessage, get_connfd);
}

//私聊函数
void chat_pn_sr(smg *ff, cJSON *user, int connfd, int mask)
{
	strcpy((*ff).message, cJSON_GetObjectItem(user, "message")->valuestring);
	(*ff).si_ID = cJSON_GetObjectItem(user, "cfr_ID")->valueint;

	int get_connfd;
	//根据接受id向对象用户发送聊天数据
	char ha[] = "SELECT * FROM user_list;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit)))
	{
		if (atoi(row[0]) == ((*ff).si_ID))
		{
			get_connfd = atoi(row[5]);
		}
	}
	mysql_free_result(shit);

	//发送聊天信息

	if (mask == 0)
	{
		cJSON *rmessage = cJSON_CreateObject();

		cJSON_AddNumberToObject(rmessage, "type", R_CH_PN_Z);
		cJSON_AddNumberToObject(rmessage, "path", CS_P);
		cJSON_AddStringToObject(rmessage, "message", (*ff).message);

		pk_s(rmessage, get_connfd);
		//printf("发送对象端口号为%d\n",get_connfd);
	}
	else //自己退出
	{
		int flag = cJSON_GetObjectItem(user, "flag")->valueint;
		printf("已经准备给自己发送\n");
		printf("flag的值是%d\n", flag);

		//给自己的消息
		cJSON *rmessage1 = cJSON_CreateObject();
		cJSON *rmessage2 = cJSON_CreateObject();

		cJSON_AddNumberToObject(rmessage1, "type", R_CH_PN_T);
		cJSON_AddNumberToObject(rmessage1, "path", CS_P);
		pk_s(rmessage1, connfd);

		//给好友的消息
		cJSON_AddNumberToObject(rmessage2, "type", R_CH_PN_Z);
		cJSON_AddStringToObject(rmessage2, "message", (*ff).message);
		cJSON_AddNumberToObject(rmessage2, "path", CS_P);
		pk_s(rmessage2, get_connfd);
	}
}

//获取当前好友列表
void friend_f5_sr(smg *ff, cJSON *user, int connfd)
{
	int num[200];
	int xb = 0; //好友数量
	int key;
	(*ff).user_ID = cJSON_GetObjectItem(user, "user_ID")->valueint;
	fetch_friend_byID((*ff).user_ID, num, &xb);

	printf("好友的数量是%d\n", xb);
	printf("好友的id是%d\n", num[0]);

	fetch_state_byID(num, &xb, connfd); //问题点
}

int fetch_friend_byID(int user_ID, int *num, int *xb) //获得xb数量
{
	char ha[] = "SELECT * FROM friend;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit)))
	{
		if ((atoi(row[0])) == user_ID)
		{
			printf("已执行好友表比对成功\n");
			num[*xb] = atoi(row[1]);
			(*xb) = (*xb) + 1;
		}
	}
	mysql_free_result(shit);
}

int fetch_state_byID(int *num, int *xb, int connfd)
{

	int i = 0;

	char id[200][200];
	char user_name[200][200];
	char state[200][200];
	//注意结果均为字符串
	int m = 0; //计数

	char ha[] = "SELECT * FROM user_list;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit)))
	{
		//printf("(*xb)为%d\n",(*xb));
		for (int j = 0; j < (*xb); j++)
		{
			//printf("num[%d]=%d\n",j,num[j]);
			if (num[j] == atoi(row[0]))
			{
				printf("num[%d]=%d", j, num[j]);
				strcpy(id[m], row[0]);
				strcpy(user_name[m], row[3]);
				strcpy(state[m], row[4]);
				printf("已进入\n");
				m++;
			}
		}
	}
	mysql_free_result(shit);

	for (int g = 0; g < m + 1; g++)
	{
		cJSON *rmessage = cJSON_CreateObject();
		if (g == 0)
		{
			cJSON_AddNumberToObject(rmessage, "type", R_FR_SX_F);
			cJSON_AddNumberToObject(rmessage, "f_num", m);
			cJSON_AddNumberToObject(rmessage, "path", FR_PATH);
		}
		else
		{
			cJSON_AddNumberToObject(rmessage, "type", R_FR_SX_C);
			cJSON_AddStringToObject(rmessage, "f_id", id[g - 1]);
			cJSON_AddStringToObject(rmessage, "f_name", user_name[g - 1]);
			cJSON_AddStringToObject(rmessage, "f_state", state[g - 1]);
			cJSON_AddNumberToObject(rmessage, "path", FR_PATH);
			//cJSON_AddNumberToObject(rmessage,"f_sum",m);
		}
		pk_s(rmessage, connfd);
	}
	printf("执行完毕\n 好友数量m为%d\n", m);
	printf("\n\n");
}

//注册函数入口整体
void zhuce_sr(smg *ff, cJSON *user, int connfd)
{
	int key; //判断database注册新用户是否成功
	(*ff).user_ID = cJSON_GetObjectItem(user, "user_ID")->valueint;
	strcpy((*ff).ac_number, cJSON_GetObjectItem(user, "ac_number")->valuestring);
	strcpy((*ff).user_password, cJSON_GetObjectItem(user, "user_password")->valuestring);
	strcpy((*ff).user_name, cJSON_GetObjectItem(user, "user_name")->valuestring);
	cJSON_Delete(user);
	key = database((*ff).ac_number, (*ff).user_password, (*ff).user_name);

	cJSON *rmessage = cJSON_CreateObject();
	cJSON *tmp = cJSON_CreateObject();

	if (key == 1)
	{
		cJSON_AddNumberToObject(rmessage, "type", R_ZH_CE_R);
	}
	else if (key == 0)
		cJSON_AddNumberToObject(rmessage, "type", R_ZH_CE_W);

	pk_s(rmessage, connfd);
	//printf("已发送 发送端口为%d\n",connfd);
}

//登录函数入口
void denglu_sr(smg *ff, cJSON *user, int connfd)
{
	int key;
	int *user_ID;
	strcpy((*ff).ac_number, cJSON_GetObjectItem(user, "ac_number")->valuestring);
	strcpy((*ff).user_password, cJSON_GetObjectItem(user, "user_password")->valuestring);
	cJSON_Delete(user);

	key = bidatabase((*ff).ac_number, (*ff).user_password, user_ID, connfd);
	printf("user_ID=%d\n", *user_ID);

	cJSON *rmessage = cJSON_CreateObject();
	cJSON *tmp = cJSON_CreateObject();

	if (key == 1)
	{
		cJSON_AddNumberToObject(rmessage, "type", R_DE_LU_R);
		cJSON_AddNumberToObject(rmessage, "user_ID", *user_ID);
	}
	else if (key == 0)
		cJSON_AddNumberToObject(rmessage, "type", R_DE_LU_W);

	cJSON_AddNumberToObject(rmessage, "path", DE_PATH);
	pk_s(rmessage, connfd);
}

//登录函数细节
//在用户表中查找指定用户和密码    登陆成功后更改用户在线状态和登录端口
int bidatabase(char *ac, char *pa, int *user_ID, int connfd)
{

	char ha[] = "SELECT * FROM user_list;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit)))
	{
		if ((strcmp(row[1], ac) == 0) && (strcmp(row[2], pa) == 0))
		{
			char result[500];
			//printf("row[0]=%d\n",atoi(row[0]));
			*user_ID = atoi(row[0]); //获取用户唯一主键
			//printf("函数内user_ID=%d\n",*user_ID);
			sprintf(result, "update user_list set state=1 where ac_number=\"%s\";", row[1]);
			mysql_real_query(mysql, result, strlen(result));
			//mysql_query( mysql ,"update user_list set state=0 where ac_number=\'%s\';",row[1]);

			//获得登陆者fd
			sprintf(result, "update user_list set duankou=%d where ac_number=\"%s\";", connfd, row[1]);
			mysql_real_query(mysql, result, strlen(result));
			return 1;
		}
	}
	mysql_free_result(shit);
	return 0;
}

//注册函数细节
int database(char *ac, char *pa, char *usr)
{

	char sql[512];
	sprintf(sql, "INSERT INTO user_list(ac_number,password,user_name) VALUES(\'%s\',\'%s\',\'%s\');", ac, pa, usr);

	printf("这是拼接完成字符串%s\n", sql);

	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	char ha[] = "SELECT * FROM user_list;";
	if (0 != mysql_real_query(mysql, ha, strlen(ha)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}

	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit)))
	{
		printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
	}

	mysql_free_result(shit);
	return 1;
}