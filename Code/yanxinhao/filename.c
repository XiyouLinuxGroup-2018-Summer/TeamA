#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
int main()
{
	struct dirent *cu = (struct dirent*)malloc(sizeof(struct dirent));
	DIR *dir;
	char buf[100];
	getcwd(buf,100);
	dir = opendir(buf);
	while((cu = readdir(dir))!=NULL)
	{
		printf("%s\n",cu->d_name);
	}
	closedir(dir);

}
