#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

void myerr(char *errstr,int line)
{
	fprintf(stderr,"line:%d",line);
	perror("errstr");
	exit(1);
}

int lock_test(int fd,struct flock *lock)
{
	if(fcntl(fd,F_GETLK,lock)==0)
	{
		if(lock->l_type==F_UNLCK)
		{
			printf("lock can be set in fd\n");
			return 0;
		}
		else//锁已存在，判断锁类型并打印pid
		{
			if(lock->l_type==F_RDLCK)
				printf("读锁已经通过%d设置\n",lock->l_pid);
			if(lock->l_type==F_WRLCK)
				printf("写锁已经通过%d设置\n",lock->l_pid);
			return -2;
		}
	}
	else
	{
		printf("获取锁信息失败\n");
		return -1;
	}
}

int lock_set(int fd,struct flock*lock)
{
	if(fcntl(fd,F_SETLK,lock)==0)
	{
		if(lock->l_type==F_RDLCK)
			printf("读锁已设置,pid:%d\n",getpid());
		if(lock->l_type==F_WRLCK)
			printf("写锁已设置，pid:%d\n",getpid());
		if(lock->l_type==F_UNLCK)
			printf("锁已释放，pid:%d\n",getpid());
	}
	else
	{
		perror("lock operation fail\n");
		return -1;
	}
}

int main(void)
{
	int fd;
	int ret;
	struct flock  lock;
	char read_buf[32];

	if((fd=open("lizi",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1)
	{
		myerr("open",__LINE__);
	}
	if((write(fd,"ceshidelizi",12))!=12)
	{
		myerr("write",__LINE__);
	}

	//初始化lock结构
	memset(&lock,0,sizeof(struct flock));
	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;
	
	//设置读锁
	lock.l_type = F_RDLCK;
	if(lock_test(fd,&lock)==0)
	{
		lock.l_type = F_RDLCK;
		lock_set(fd,&lock);
	}

	//读数据
	lseek(fd,0,SEEK_SET);
	if((ret=read(fd,read_buf,12))<0)
	{
		myerr("read",__LINE__);
	}
	read_buf[ret] = '\0';
	printf("%s\n",read_buf);

	//暂停画面
	getchar();

	//设置写锁
	lock.l_type = F_WRLCK;
	if((lock_test(fd,&lock))==0)
	{
		lock.l_type = F_WRLCK;
		lock_set(fd,&lock);
	}
	getchar();

	//释放锁了解下
	lock.l_type = F_UNLCK;
	lock_set(fd,&lock);
	close(fd);
	return 0;
}
