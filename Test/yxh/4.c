#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>

typedef struct file{
	int fid;
	char filename[128];
	int mode;
	int size;
	char data[256];	
	struct file* next;
}myfile;

int main()
{
	DIR *dir;
	int i=0,len;
	struct dirent *ptr;
	struct stat buf;
	char path[100] = "/lib/";
	char filenames[1000][100];
	char name[100];
	int flag=0,fd,fd2;

	myfile *head = (myfile*)malloc(sizeof(myfile));
	myfile *cu;
	myfile *pre;
	head->next = NULL;

	dir = opendir(path);
	while((ptr = readdir(dir))!=NULL)
	{
		len = strlen(path);
		strncpy(filenames[i],path,len);
		filenames[i][len] = '\0';
		strcat(filenames[i],ptr->d_name);
		filenames[i][strlen(ptr->d_name)+len] = '\0';
		lstat(filenames[i],&buf);
		
		if(S_ISDIR(buf.st_mode))
			continue;
		flag++;

		cu = (myfile*)malloc(sizeof(myfile));
		if(head->next==NULL)
			head->next = cu;
		else
			pre->next = cu;
		cu->fid = flag;
		strcpy(cu->filename,ptr->d_name);
		cu->mode = buf.st_mode;
		cu->size = buf.st_size;
		fd = open(filenames[i],O_RDONLY);
		read(fd,cu->data,sizeof(char)*256);
		
		fd2 = creat("./allfiles.dat",S_IWRITE);
		write(fd2,cu,sizeof(myfile));

		pre = cu;
		pre->next = NULL;
		i++;
	}
	close(dir);
}
