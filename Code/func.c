#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
#include<sys/stat.h>
int main (int argc,char ** argv)
{
	int fd;
	char buf[32];
	char buf1[32];
	char buf2[3];
	struct stat temp;
	if( argc != 5)
	{
		printf("wrong\n");
		exit(1);
	}
	if((fd=open(*(argv+1),O_RDWR|O_CREAT|O_EXCL,S_IRWXU))  == -1)
	{
		perror("open");
		exit(1);
	}
	gets(buf);
	int m = strlen(buf);
	if((write(fd,buf,m)) <0)
	{
		perror("write");
		exit(1);
	}
	close(fd);
	if((fd=open(*(argv+1),O_RDONLY))  == -1)
	{
		perror("open");
		exit(1);
	}
	if((read(fd,buf1,m)) <0)
	{
		perror("read");
		exit(1);
	}
	int len = lseek(fd,0,SEEK_END);
	if(len == -1){
		perror("lseek");
	        exit(1);
	}
         printf("%d\n",len);
	 lseek(fd,0,SEEK_SET);
	for(int i = 0;i<m;i++)
	printf("%c",buf1[i]);
	printf("\n");
	fchown(fd,1002,1002);
	umask(0);
	if((fd = creat(*(argv+2),S_IRWXU|S_IRWXG|S_IRWXO)) == -1)
	{
		perror("creat");
		exit(1);
	}
	umask(S_IRWXU);
        if((fd = creat(*(argv+3),S_IRWXU|S_IRWXG|S_IRWXO)) == -1)
	{
		perror("creat");
		exit(1);
	}
	if(chdir(argv[4]) <0)
	{
		perror("chdir");
		exit(1);
	}
	if(getcwd(buf1,512) < 0)
	{
		perror("getcwd");
		exit(0);
	}
	printf("%s\n",buf1);
	for(int i = 1;i<3;i++)
	{
		if(stat(argv[i],&temp)<0)
		{
			perror("stat");
			exit(1);
		}
		printf("UID:%d\n",temp.st_uid);
	}
	for(int i = 0;i<4;i++)
	{
		char st[7];
		strcpy(st,*(argv+i));
		printf("%s",st);
		if(unlink("st")<0)
		{
			perror("unlink");
			exit(0);
		}

	}
	if(lseek(fd,-1,SEEK_SET)<0)
	{
		perror("lseek");
		exit(1);
	}
	close(fd);

}
