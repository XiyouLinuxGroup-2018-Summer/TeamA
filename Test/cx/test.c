#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<error.h>
typedef struct file{
	int fid;
	 char filename[128];
	int mode;
	 int size;
	 char data[256];
} myfile；
typedef struct list {
	myfile date;
	struct list * pNext;
}NODE,*PNODE;
PNODE creat_list(char *path)
{
	PNODE pHead = (PNODE)malloc(sizeof(NODE));
	PNODE pTail = pHead;
	pTail->pNext = NULL;
	myfile temp;
	struct stat buf;
	struct dirent ptr;
	DIR * dir;
	int count = 0,k;
	char st[256];
	char name[256];
	if(stat(path,&buf) < 0)
	{
		perror("stat");
		return -1;
	}
	if(S_ISDIR(buf,st_mode) <0)
	{
		return -1;
	}
	if((dir = open(path)) == NULL)
	{
		perror("dir");
		return -1;
	}
	while((ptr = readdir(dir)) != NULL)
	{
		++count;
		temp->fid = count;
		strcpy(name,path)
		int m = strlen(name);
		name[m+1] = '\0';
		for(i = 0;i<strlen(name);i++)
		{
			if(name[i] == '/')
			{
				k = 0;
				continue;
			}
			st[k++] = name[i]; 
		}
		st[k] = '\0';
		strcpy(temp->filename,st);
		temp->mode = buf.st_mode;
		temp->size = buf.st_size;
		printf("%d",p->date->fid);
		printf("%s",p->date->filename);
		printf("%d",p->date->mode);
		printf("%d",p->date->size);
	//	printf("%s",p->date-data);
		PNODE pNew = (PNODE)malloc(sizeof(NODE));
		pNew->data = temp;
		pTail->pNext = pNew;
		pNew->pNext = NULL;
		pTail = pNew;
		if(lstat(name,&buf) < 0)
		{
			perror("lstat");
			return NULL;
		}
		if(S_ISDIR(buf.st_mode))
			continue;
		else
			if(creat_list(name) == pHead)
				return pHead;
	}
	return pHead;
}
void save (PNODE pH,char * path)
{
	PNODE p = pH->pNext;
	int fd;
	if((fd = open(path,O_CREAT|O_RDWR|O_EXEC,S_IRWXU)) == -1)
	{
	 		printf("wrong\n");
			exit(1);
	}
	while( p!= NULL)
	{
		if(write(fd,pNew->data,sizeof(NODE)) != -1);
		p = p->pNext;
	}
}
int main (int argv,char **argv)
{
	if(argc != 2)
	{
		printf("wrong\n");
		exit(1);
	}
	creat_list(*(argv+1));
	return 0;
}
