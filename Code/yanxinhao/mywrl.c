#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

void myerr(char *errstr,int line)
{
	fprintf(stderr,"line:%d",line);
	perror("errstr");
	exit(1);
}

int myread(int fd)
{
	int len;
	int ret;
	int i;
	char read_buf[100];
	if(lseek(fd,0,SEEK_END)==-1)
		myerr("lseek",__LINE__);
	if(len = lseek(fd,0,SEEK_CUR)==-1)
		myerr("lseek",__LINE__);
	if(lseek(fd,0,SEEK_SET)==-1)
		myerr("sleek",__LINE__);
	printf("len:%d\n",len);

	if((ret=read(fd,read_buf,len))<0)
	{
		myerr("lseek",__LINE__);
	}
	for(i=0;i<len;i++)
		printf("%c",read_buf[i]);
	printf("\n");

}

int main()
{
	int fd;
	char write_buf[100] = "hello world";
	if((fd=open("lizi.c",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU))==-1)
	{
		myerr("open",__LINE__);
	}
	else
		printf("creat ok\n");
	if(write(fd,write_buf,strlen(write_buf))!=strlen(write_buf))
	{
		myread(fd);
	}

	printf("------------------f g x-------------------\n");
	if(lseek(fd,10,SEEK_END)==-1)
	{
			myerr("lseek",__LINE__);
	}
	if(write(fd,write_buf,strlen(write_buf))!=strlen(write_buf))
	{
		myerr("write",__LINE__);
	}
	myread(fd);
	close(fd);
}
