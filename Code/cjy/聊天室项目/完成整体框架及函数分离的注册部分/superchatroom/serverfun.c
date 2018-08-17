/*************************************************************************
	> File Name: serverfun.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月15日 星期三 14时30分14秒
 ************************************************************************/

#include<stdio.h>
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
#include<fcntl.h>
#include <errno.h>
#include<mysql/mysql.h>

#include"serverfun.h"
#include"public.h"

//发送宏
#define  ZH_CE 0

//接收宏
#define  R_ZH_CE_W 0
#define  R_ZH_CE_R 1



//解析函数
void analyse(char *message,int connfd)
{
	//解析JSON
	smg ff;
	cJSON *user = cJSON_Parse(message);
	ff.type=cJSON_GetObjectItem(user,"type")->valueint;
	switch(ff.type)
	{
		case ZH_CE:
			zhuce_sr(&ff,user,connfd);
			break;
	}	
}

//注册函数入口整体
void zhuce_sr(smg *ff,cJSON *user,int connfd)
{
	int key;     //判断database注册新用户是否成功
	(*ff).user_ID = cJSON_GetObjectItem(user,"user_ID")->valueint;
	strcpy((*ff).ac_number,cJSON_GetObjectItem(user,"ac_number")->valuestring);
	strcpy((*ff).user_password,cJSON_GetObjectItem(user,"user_password")->valuestring);
	strcpy((*ff).user_name,cJSON_GetObjectItem(user,"user_name")->valuestring);
	cJSON_Delete(user);
	key=database((*ff).ac_number,(*ff).user_password,(*ff).user_name);
	
	cJSON * rmessage = cJSON_CreateObject();
	cJSON * tmp=cJSON_CreateObject();

	if(key==1)
		cJSON_AddNumberToObject(rmessage,"type",R_ZH_CE_R);
	else if(key==0)
		cJSON_AddNumberToObject(rmessage,"type",R_ZH_CE_W);


	pk_s(rmessage,connfd);
	
}

//注册函数细节
int database(char *ac,char *pa,char *usr)
{
	MYSQL *mysql = mysql_init(NULL);
	//初始化MYSQL句柄
	if(NULL == mysql) {//mysql_init(&mysql)){
		printf("init: %s\n",mysql_error(mysql));
	}

	//建立MYSQL连接
	if(NULL == mysql_real_connect(mysql,"localhost","debian-sys-maint","wRSyIQoZEAJdafna","chatroom",0,NULL,0))
	{
		printf("connect: %s\n",mysql_error(mysql));
	}

	char sql[512];
	sprintf(sql, "INSERT INTO user_list(ac_number,password,user_name) VALUES(\'%s\',\'%s\',\'%s\');",ac,pa,usr);


	printf("这是拼接完成字符串%s\n",sql);
	//printf("其长度为:%d\n",strlen(tmp));
	//getchar();
	//printf("strlen(sql) = %d\n", strlen(sql));
	if(0 != mysql_real_query(mysql,sql,strlen(sql)))
	{
		printf("query: %s\n",mysql_error(mysql));
	}


	char ha[]= "SELECT * FROM user_list;";

	if(0 != mysql_real_query(mysql,ha,strlen(ha))){
		printf("query: %s\n",mysql_error(mysql));
	}
      
	MYSQL_RES *shit = mysql_store_result(mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(shit))) {
		printf("%s %s %s %s\n",row[0],row[1],row[2],row[3]);
	}

	mysql_free_result(shit);
	mysql_close(mysql);
      
	return 1;
}