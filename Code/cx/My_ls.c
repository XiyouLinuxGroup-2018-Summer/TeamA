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
#include<sys/ioctl.h>
#include<termios.h>
void traverse_full(struct stat buf,char * path);
void traverse_s(char *path);
int pos (char (*)[80],int low,int high);
void q_sort (char (*)[80],int low,int high);
void traverse_dir(int select,char * path);
int Recursive(char *pathi,char c);
void inode (char *path);
int main (int argc, char * argv[])
{
	int i,j = 0,k,choice;
	char st[30];
	char name[30];
	int m;
	char ch;
	if(argc > 3 || argc < 0)
	{
		printf("wrong\n");
		exit(1);
	}
	char *p;
	p = (char*)malloc(sizeof(char)*512);
	strcpy(p,*(argv+1));
	struct stat buf;
	if(argc == 2)
	{
	if(stat(argv[1],&buf) <0)
	{
		perror("stat");
		exit(1);
	}
		if(S_ISDIR(buf.st_mode))
		{
			traverse_dir(2,*(argv+1));
		}
		else
		traverse_s(*(argv+1));
	}
	else
	{
		if((m = getopt(argc,argv+1,*(argv+2))) != -1)
		{
			if(stat(*(argv+1),&buf) < 0)
			{
				perror("stat");
				exit(1);
			}
		if(strcmp(argv[2],"-lR") == 0)
		{
			Recursive(*(argv+1),'l');
			return 0;
		}	
		if(strcmp(argv[2],"-al") == 0)
		{
			if(S_ISDIR(buf.st_mode))
				traverse_dir(1,*(argv+1));	
			else
				traverse_full(buf,*(argv+1));
			return 0;	
		}
			switch(m)
			{
				case 'a':
					if(S_ISDIR(buf.st_mode))
						traverse_dir(4,*(argv+1));
						else
						traverse_s(*(argv+1));
						break;
				case 'l':
						if(S_ISDIR(buf.st_mode))
						traverse_dir(3,*(argv+1));
						else
						traverse_full(buf,*(argv+1));
						break;
				case 'R':
							Recursive(*(argv+1),'c');
						break;	
				
			}
		}
	}
			return 0;
}
void traverse_full(struct stat buf,char * path)
{
		DIR *dir;
		struct group *grp;
		struct passwd *psd;
		char st[32];
		char name[32];
		if(stat(path,&buf) < 0)
		{
			printf("%d",__LINE__);
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
			traverse_s(path);
			printf("\n");
		return;

}
void  traverse_s(char *path)
{
		char name[50];
		struct stat buf;
		int i,k;
		char st[50];
		if(stat(path,&buf) < 0)
		{
			printf("%d",__LINE__);
			perror("stat");
			exit(1);
		}
			strcpy(name,path);
			int m = strlen(name);
			name[m+1] = '\0';
		if(S_ISLNK(buf.st_mode))
			printf("\033[40;30m%s\t\033[0m",name);
		if(S_ISREG(buf.st_mode))
			printf("\033[40;31m%s\t\033[0m",name);
		if(S_ISDIR(buf.st_mode))
			printf("\033[40;32m%s\t\033[0m",name);
		if(S_ISCHR(buf.st_mode))
			printf("\033[40;33m%s\t\033[0m",name);
		if(S_ISBLK(buf.st_mode))
			printf("\033[40;34m%s\t\033[0m",name);
		if(S_ISFIFO(buf.st_mode))
			printf("\033[40;35m%s\t\033[0m",name);
		if(S_ISSOCK(buf.st_mode))
			printf("\033[40;36m%s\t\033[0m",name);
		return;
}
void traverse_dir(int select,char * path)
{
         	DIR *dir;
		char filename[256][80];
		struct dirent * ptr;
		struct stat buf;
		struct winsize size;
		int SIZE  = 8,col,row;
		row = size.ws_row;
		col = size.ws_col;
		if(SIZE > col)
		 SIZE  = col;
		int count = 0,i;
		int j = 0;
		char name[50];
		char st[50];
		int times = 0;
		char  (*p)(char * path);
			if((dir = opendir(path)) == NULL)
			{
				perror("opendir");
				exit(1);
			} 
			while((ptr = readdir(dir)) != NULL)
			{
				count++;
			}
				printf("文件总数:%d\n",count);
				if(count > 256)
				printf("too many files\n");
			        int m = strlen(path);
				closedir(dir);
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
				}
				q_sort(filename,0,count - 1);
					if(getcwd(st,512) <0)
					{
						perror("getcwd");
						exit(1);
					}
					if(chdir(path) < 0)
					{
						perror("chdir");
							exit(1);
					}
				for(i = 0;i<count;i++)
				{
					switch(select)
					{
					case 1:
						traverse_full(buf,filename[i]);
						break;
					case 2:
						    if(filename[i][0]!= '.'){
								traverse_s(filename[i]);
						    }
							break;
					case 3:if(filename[i][0] != '.')
						       traverse_full(buf,filename[i]);
					       break;
					case 4:
					       traverse_s(filename[i]);
					       break;
					}
			}
				closedir(dir);
				printf("\n");
			return;
}
int pos (char (*a)[80],int low,int high)
{
	char filename[80];
	int m = high;
	strcpy(filename,a[high]);
	filename[strlen(a[high])] = '\0';
	while(low < high)
	{
		while(low<high && strcmp(filename,a[low]) > 0)
			low++;
		strcpy(a[high],a[low]);
		a[high][strlen(a[low])] = '\0';
		while(low<high && strcmp(filename,a[high]) < 0)
			--high;
		strcpy(a[low],a[high]);
		a[low][strlen(a[high])] = '\0';
		
	}
	strcpy(a[high],filename);
	a[high][strlen(filename)] = '\0';
	return low;
}
void q_sort(char (*a)[80],int low,int high)
{
		int p = pos(a,low,high);
	if(low >= high)
		return;
	else
	
	{
		q_sort(a,low,p-1);
		q_sort(a,p+1,high);
	}
	return;
}
int Recursive(char *path,char c)
{
	DIR * dir;
	struct stat buf;
	struct dirent *ptr;
	int i = 0,count = 0,k = 0,m;
	char st[20000];
	char name[20000];	//当你想统计'/'目录下的文件的时候，大数组完全就是种享受（滑稽）
	char *p  = path + strlen(path);
	if(lstat(path,&buf) < 0)		//获取目录属性
	{
		perror("stat");
		return -1;
	}
	if(S_ISDIR(buf.st_mode) == 0)		//判断是否是目录文件
	{
		return -1;
	}
	if(*(p-1)!= '/')			//将目录文件路径补充完整
	{
		*p++ = '/';
		*p = 0;
	}
	printf("\n");
	puts(path);				//输出该级目录路径
	if((dir = opendir(path)) == NULL)	//打开指定目录
	{
		perror("opendir");
		return -1;
	}
	while((ptr = readdir(dir)) != NULL)
	{
		int a = 0;
		if((strcmp(ptr->d_name,".")==0) || (strcmp(ptr->d_name,"..")==0))
	          continue;			//剔除隐藏文件
		strcpy(p,ptr->d_name);
		strcpy(name,path);		//保存文件所在路径名
		for(i = 0;i<strlen(name);i++)	//单纯的获得文件名称
		{
			if(name[i] == '/')
			{
				k = 0;
				continue;
			}
			st[k] = name[i];
			k++;
		}
		st[k] = '\0';
		if(lstat(name,&buf) <0)
		{
			perror("stat");
			return -1;
		}
		if(c == 'l')
			traverse_full(buf,name);
		else
		{
			c = 'c';
		if(S_ISLNK(buf.st_mode))
			printf("\033[40;30m%s\t\033[0m",st);
		if(S_ISREG(buf.st_mode))
			printf("\033[40;31m%s\t\033[0m",st);
		if(S_ISDIR(buf.st_mode))
			printf("\033[40;32m%s\t\033[0m",st);
		if(S_ISCHR(buf.st_mode))
			printf("\033[40;33m%s\t\033[0m",st);
		if(S_ISBLK(buf.st_mode))
			printf("\033[40;34m%s\t\033[0m",st);
		if(S_ISFIFO(buf.st_mode))
			printf("\033[40;35m%s\t\033[0m",st);
		if(S_ISSOCK(buf.st_mode))
			printf("\033[40;36m%s\t\033[0m",st);
		}
		if(lstat(name,&buf) < 0)	//对该层目录下的文件进行判断与删选
		{
			perror("lstat");
		}
		if(S_ISDIR(buf.st_mode) < 0)   //如果不是目录文件继续遍历该层其余文件
			continue;
		if(Recursive(name,c) == 0)	//继续沿着目录文件向下访问
			printf("\n");
	}
	p[-1] = 0;
	if((m =closedir(dir)) == -1)
	{
		return -1;
	}
	printf("\n");
		return 0;

}
