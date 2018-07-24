#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<string.h>
#include<error.h>
#include<dirent.h>
#include<time.h>
#define NONE 0
#define L 1
#define A 2
#define AL 3
void traverse_full(char * path);
void traverse_s(char *path);
void display(int flag,char * path);
void traverse_dir(char * path);
void inode (char *path);
int main (int argc, char * argv[])
{
	if(argc != 3)
	{
		printf("wrong\n");
		exit(1);
	}
	struct stat buf;
	traverse_full(*(argv+1));
	traverse_s(*(argv+1));
//	display(1,*(argv+2));
//	inode(*(argv+2));
	traverse_dir(*(argv+2));
		return 0;
}
void traverse_full(char * path)
{
		DIR *dir;
		struct stat buf;
		struct group *grp;
		struct passwd *psd;
		char st[32];
		if(stat(path,&buf) < 0)
		{
			perror("stat");
			exit(1);
		}
		if(S_ISLNK(buf.st_mode))
			printf("l");
		if(S_ISREG(buf.st_mode))
			printf("-");
		if(S_ISDIR(buf.st_mode))
			printf("d");
		if(S_ISCHR(buf.st_mode))
			printf("c");
		if(S_ISBLK(buf.st_mode))
			printf("b");
		if(S_ISFIFO(buf.st_mode))
			printf("f");
		if(S_ISSOCK(buf.st_mode))
			printf("s");
		if(buf.st_mode & S_IRUSR)
			printf("r");
		else
			printf("-");
		if(buf.st_mode & S_IWUSR)
			printf("w");
		else
			printf("-");
		if(buf.st_mode & S_IXUSR)
			printf("x");
		else
			printf("-");
		if(buf.st_mode & S_IRGRP)
			printf("r");
		else
			printf("-");
		if(buf.st_mode & S_IWGRP)
			printf("w");
		else
			printf("-");
		if(buf.st_mode & S_IXGRP)
 			printf("x");
		else
			printf("-");
		if(buf.st_mode & S_IROTH)
			printf("r");
		else
			printf("-");
		if(buf.st_mode & S_IWOTH)
			printf("w");
		else
			printf("-");
		if(buf.st_mode & S_IXOTH)
			printf("x");
		else
			printf("-");
		psd = getpwuid(buf.st_uid);
		grp = getgrgid(buf.st_gid);
		printf(" ");
		printf("%4d",buf.st_nlink);
		printf("%8s",psd->pw_name);
		printf("%8s",grp->gr_name);
		printf("%6d\t",buf.st_size);
		strcpy(st,ctime(&buf.st_mtime));
		st[strlen(st) - 1] = '\0';
		printf("%s\t",st);
		return;

}
void traverse_s(char *path)
{
		char st[32];
		strcpy(st,path);
		int m = strlen(st);
		st[m+1] = '\0';
		puts(st);
		return;
}
void display(int flag,char * path)
{
	      char name[32];
	      char st[32];
              int i,j;
		struct stat buf;
	       if(chdir(path) < 0)
	       {
		       perror("chdir");
		       exit(1);
	       }
	      if( getcwd(st,512) < 0)
	      {
		      perror("getcwd");
		      exit(1);
	      }
	      printf("%s\n",st);
	      int m = strlen(path);
	       for(i = 0,j = 0;i<m;i++)
	       {
		       if(st[i] == '/')
		       {
			       j = 0;
			       continue;
		       }
		       name[j++] = st[i];
	       }
		name[j] = '\0';
		switch(flag)
		{
			case 1:
				traverse_full(path);
				break;
			case 2:
				if(name[0] != '.')
				traverse_s(path);
				break;
		
		}
		return;

}
void traverse_dir(char * path)
{
         	DIR *dir;
		char filename[256][80],temp[80];
		struct dirent * ptr;
		int count = 0,i;
		if((dir = opendir(path)) == NULL)
		{
			perror("opendir");
			exit(1);
		}
		while((ptr = readdir(dir)) != NULL)
		{
			count++;
		}
		printf("%d",count);
		closedir(dir);
		if(count > 256)
		printf("too many files\n");
	        int m = strlen(path);
		dir = opendir(path);
		for(i = 0;i<count;i++)
		{
			ptr = readdir(dir);
			if(ptr == NULL)
			{
				perror("readdir");
				exit(1);
			}
			strcpy(filename[i],ptr->d_name);
		/*	filename[i][m] = '\0';
			strcat(filename[i],ptr->d_name);
			filename[i][m+strlen(ptr->d_name)] = '\0';*/
			
		}
		for(i = 0;i<count;i++)
		{
			traverse_s(filename[i]);
		}
		closedir(dir);
		return;
}
void inode (char *path)
{
	struct stat buf;
	char name[256];
	if(stat(path,&buf) < 0)
	{
		perror("stat");
		exit(1);
	}
	DIR *dir;
        struct dirent * ptr;
	if(opendir(path) == NULL)
	{
		perror("opendir");
		exit(1);
	}
	int i = 0;
	while((ptr = readdir(dir)) != NULL)
	{
		printf("%d",buf.st_ino);
	}
	return ;
}
