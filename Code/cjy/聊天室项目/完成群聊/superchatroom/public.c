/*************************************************************************
	> File Name: public.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月17日 星期五 15时17分11秒
 ************************************************************************/

#include<stdio.h>
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

#include"public.h"

//打包发送函数
void pk_s(cJSON * smessage,int fd)
{
	char *tmp_out= cJSON_Print(smessage);
	tmp_out= cJSON_PrintUnformatted(smessage);// 生成适合传输的JSON字符串
	printf("发送的数据包是%s\n",tmp_out);

	int number = strlen(tmp_out); 
	char package[number+4];
	strcpy(package+4,tmp_out);
	*(int*)package=number;
	send(fd,package,number + 4,0);
}

//接收函数
int pk_r(char **name,int fd)
{
	int n;	
	int renumber,tmpnumber,tmpn;
	recv(fd,&renumber,4,0);

	tmpnumber=renumber;

	*name=(char*)malloc(renumber+1);
	n = recv(fd,*name,renumber,0);
	if(n == 0) {
		close(fd);
		return 0;
	}
	tmpn=n;
	tmpnumber=renumber-n;
	
	int neitmpn;

	while(tmpnumber!=0)
	{     //neitmpn=tmpn;                  
		tmpn=recv(fd,(*name)+tmpn,tmpnumber,0);
		tmpnumber-=tmpn;
	}

	(*name)[renumber]='\0';
	printf("收到的数据包是%s\n",*name);
}

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