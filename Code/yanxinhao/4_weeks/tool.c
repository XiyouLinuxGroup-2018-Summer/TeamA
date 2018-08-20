#include"heads.h"

//错误处理函数
int my_err(char * err,int line)
{
	printf("第%d行出错:",line);
	perror(err);
	exit(1);
}
//由于内核中给套接字的缓冲区可能会达到极限,读写函数输入或输出的字节数可能会比请求的少,
//所以需要通过函数返回值和循环确定实际输入输出字节数
int my_read(int fd,void *vptr,int n)
{
	int nread,nleft=n;
	PACK * ptr = vptr;
	while(nleft>0)
	{
			printf("nleft:%d\nn:%d\n",nleft,n);
		if(nread = recv(fd,ptr,nleft,0)<0)
		{
				printf("readd:%d\n",nread);
			//如果因为信号中断导致未读到数据则重新循环
			if(errno==EINTR)
				nread = 0;
			else
				return -1;
		}
		else if(nread==0)
			break;
		
				printf("readd:%d\n",nread);
		nleft-=nread;
		ptr+=nread;
	}
	printf("nleft:%d\nn:%d\n",nleft,n);
	return n-nleft;
}
int my_write(int fd, void* vptr,int n)
{
	int nleft = n;
	int nwrite;
	 PACK * ptr = vptr;

	while(nleft>0)
	{
		if((nwrite = send(fd,ptr,nleft,0))<=0)
		{
			if(nwrite<0&&errno==EINTR)
					nwrite = 0;
			else
					return -1;
		}
		nleft-=nwrite;
		ptr+=nwrite;
	}
	return n;
}


//包处理函数,根据包类型调用相应函数
void* pack_deal(void*recv_package_t)
{
	    PACK *recv_pack = (PACK *)recv_package_t;
		switch(recv_pack->type)
		{
				case LOGIN:
						login_deal(recv_pack);
						break;
				case REGISTER:

				case EXIT:
						break;
			
		}

}
void login_deal(PACK*recv_package_t)
{
	PACK re_package;
	memset(&re_package,0,sizeof(PACK));
	int flag=0;
	printf("查询数据库...\n");
	char * sql = "SELECT username,password FROM userlogin";
	if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
			printf("数据库询问:%s\n",mysql_error(&mysql));
	MYSQL_RES* res = mysql_store_result(&mysql);
	MYSQL_ROW row;
	while((row = mysql_fetch_row(res)))
	{
		if(strcpy(row[0],recv_package_t->data.send_name)==0)
		{
			if(strcpy(row[1],recv_package_t->data.mes)==0)
			{
				strcpy(re_package.data.send_name,"serve");
				strcpy(re_package.data.recv_name,recv_package_t->data.send_name);
				strcpy(re_package.data.mes,"Y");
				re_package.data.recv_fd = recv_package_t->data.send_fd;
				//printf("%d\n",re_package.data.recv_fd);
				if(write(re_package.data.recv_fd,&re_package,sizeof(PACK))<0)
				{
					printf("写函数错误\n");
					exit(-1);
				}
				//free(recv_package_t);
				return;
			}
		}
	}
		strcpy(re_package.data.send_name,"serve");
        strcpy(re_package.data.recv_name,recv_package_t->data.send_name);
        strcpy(re_package.data.mes,"N");
        re_package.data.recv_fd = recv_package_t->data.send_fd;
        if(write(re_package.data.recv_fd,&re_package,sizeof(PACK))<0)
        {
			printf("写函数错误\n");
			exit(-1);
        }
        //free(recv_package_t);
        return;	
}
