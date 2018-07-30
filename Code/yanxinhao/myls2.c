#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>
//#include<Linux/limits.h>

#define MAXROWLEN 150

int g_leave_len = MAXROWLEN;
int g_maxlen;

void myerr(char *errstr,int line);
void display_att(struct stat buf,char *name)//
{
	//char buf_time[50];
	char * buf_time = (char *)malloc(sizeof(char)*1000);
	struct passwd *psd;
	struct group * grp;
	if(S_ISLNK(buf.st_mode))
		printf("l");
	else if(S_ISREG(buf.st_mode))
		printf("-");
	else if(S_ISDIR(buf.st_mode))
		printf("d");
	else if(S_ISCHR(buf.st_mode))
		printf("c");
	else if(S_ISBLK(buf.st_mode))
		printf("b");
	else if(S_ISFIFO(buf.st_mode))
		printf("f");
	else if(S_ISSOCK(buf.st_mode))
		printf("s");
	
	if(buf.st_mode&S_IRUSR)
		printf("r");
	else
		printf("-");
	if(buf.st_mode&S_IWUSR)
		printf("w");
	else
		printf("-");
	if(buf.st_mode&S_IXUSR)
		printf("x");
	else
		printf("-");

	if(buf.st_mode&S_IRGRP)
		printf("r");
	else
		printf("-");
	if(buf.st_mode&S_IWGRP)
		printf("w");
	else
		printf("-");
	if(buf.st_mode&S_IXGRP)
		printf("x");
	else
		printf("-");

	if(buf.st_mode&S_IROTH)
		printf("r");
	else
		printf("-");
	if(buf.st_mode&S_IWOTH)
		printf("w");
	else
		printf("-");
	if(buf.st_mode&S_IXOTH)
		printf("x");
	else
		printf("-");

	printf("	");//

	psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);

	printf("%4ld",buf.st_nlink);
	printf("	");
	printf("%-8s",psd->pw_name);
	printf("%-8s",grp->gr_name);
	printf("%6ld",buf.st_size);

	strcpy(buf_time,ctime(&buf.st_mtime));
	buf_time[strlen(buf_time)-1] = '\0';
	printf("	%s",buf_time);

}

void display_sin(char *name)//打印单个文件名
{
	int i,len;
	if(g_leave_len < g_maxlen)//全局变量
	{
		printf("\n");
		g_leave_len = MAXROWLEN;
	}
	len = strlen(name);
	len = g_maxlen - len;
	printf("%-s",name);

	for(i=0;i<len;i++)
		printf(" ");
	printf(" ");
	g_leave_len -= (g_maxlen+2);
}

void display(int flag,char * filename)
{
		int i,j;
	//char name[50];
	char *name = (char *)malloc(sizeof(char)*1000);
	struct stat buf;	

	//从路径中截取文件名
	for(i=0,j=0;i<strlen(filename);i++)
	{
		if(filename[i]=='/')
		{
			j=0;
			continue;
		}
		name[j++] = filename[i];
	}
	name[j] = '\0';
	
	if(lstat(filename,&buf)==-1)
			perror("lstat");
	//1:l 2:a 3:al or la 0:only

	if(flag==0)
	{	if(name[0]!='.')
		{	
			if(S_ISDIR(buf.st_mode))
				printf("\033[31m\033[1m");
			display_sin(name);
			printf("\033[0m");
		}
	}
	else if(flag==1)
	{	if(name[0]!='.')
			{
				display_att(buf,name);
				if(S_ISDIR(buf.st_mode))
					printf("\033[31m\033[1m");
				printf("	%-s\n",name);
				printf("\033[0m");

			}
	}
	else if(flag==2)
	{	if(S_ISDIR(buf.st_mode))
			printf("\033[31m\033[1m");
		display_sin(name);
		printf("\033[0m");
	}

	else if(flag==3)		
	{
		display_att(buf,name);
		if(S_ISDIR(buf.st_mode))
			printf("\033[31m\033[1m");
		printf("	%-s\n",name);
		printf("\033[0m");
	}


}
void display_dir_R(int flag,char *path)
{
	DIR *dir;
	struct stat buf;
	struct dirent * ptr;
	int count = 0;
	int len;
	char *tmp = (char *)malloc(sizeof(char)*100);
	//char filenames[100000][1000];
	char **filenames;
	filenames = (char**)malloc(sizeof(char*)*100000);
	for(int i=0;i<10000;i++)
			filenames[i] = (char *)malloc(sizeof(char)*10000);
	dir = opendir(path);
	if(dir==NULL)
		myerr("opendir",__LINE__);
	while((ptr = readdir(dir))!=NULL)
	{
		if(flag<2&&ptr->d_name[0]=='.')
			continue;
		if(g_maxlen<strlen(ptr->d_name))
			g_maxlen = strlen(ptr->d_name);
		if(path[strlen(path)-1]!='/')
		{
			path[strlen(path)] = '/';
			path[strlen(path)+1] = '\0';
		}
		len = strlen(path);
		strncpy(filenames[count],path,len);
		filenames[count][len] = '\0';
		strcat(filenames[count],ptr->d_name);
		filenames[count][len + strlen(ptr->d_name)] = '\0';
		count++;
	}
	for(int i=0;i<count;i++)
	{
			
		display(flag,filenames[i]);
	}

	for(int i=0;i<count;i++)
	{
		lstat(filenames[i],&buf);
		if(S_ISDIR(buf.st_mode))
		{
			printf("%s\n",filenames[i]);
			if('.'==filenames[i][strlen(filenames[i])-1])
			{
					continue;
			}
			else
				display_dir_R(flag,filenames[i]);
		}
		else
			continue;
	}
}
void display_dir(int flag_param,char *path)
{
	DIR *dir;
	int count=0;
	char filenames[256][50];
	char tmp[100];
	struct dirent *ptr;

	//获取指定目录下文件总数和最长文件名长度
	dir = opendir(path);
	if(dir==NULL)
		myerr("opendir",__LINE__);
	while((ptr=readdir(dir))!=NULL)
	{
		if(g_maxlen<strlen(ptr->d_name))
			g_maxlen = strlen(ptr->d_name);
		count++;
	}
	closedir(dir);
	if(count>256)
		myerr("files is too many",__LINE__);
	
	int len = strlen(path);
	dir = opendir(path);
	for(int i=0;i<count;i++)
	{
		ptr = readdir(dir);
		if(ptr==NULL)
			myerr("readdir",__LINE__);
		strncpy(filenames[i],path,len);
		filenames[i][len] = '\0';
		strcat(filenames[i],ptr->d_name);
		filenames[i][len+strlen(ptr->d_name)] = '\0';
	}
	
	//冒泡对文件名排序
	for(int i=0;i<count;i++)
		for(int j=0;j<count-1;j++)
		{
			if(strcmp(filenames[j],filenames[j+1])>0)
			{
				strcpy(tmp,filenames[j]);
				tmp[strlen(filenames[j])] = '\0';
				strcpy(filenames[j],filenames[j+1]);
				filenames[j][strlen(filenames[j+1])] = '\0';
				strcpy(filenames[j+1],tmp);
				filenames[j+1][strlen(tmp)] = '\0';
			}
		}

	for(int i=0;i<count;i++)
		display(flag_param,filenames[i]);

	closedir(dir);
}

void myerr(char *errstr,int line)
{
	fprintf(stderr,"line:%d",line);
	perror(errstr);
	exit(1);
}

//终于开始的主函数。。。
int main(int argc,char** argv)
{
	char path[100] = "./";
	char param[20];
	struct stat buf;
	int flag=0,flag2=0,flag3=0;

	for(int i=1;i<argc;i++)
	{
		if(argv[i][0]=='-')
		{
			for(int j=1;j<1+strlen(argv[i]);j++)
			{
				if(argv[i][j]=='l')
				{
						if(flag==2)
							flag = 3;
						else if(flag==0)
							flag = 1;
				}
				if(argv[i][j]=='a')
				{
						if(flag==1)
							flag = 3;
						else if(flag==0)
							flag = 2;
				}
				if(argv[i][j]=='R')
						flag3 = 1;
			}
		}
		else
		{
			strcpy(path,argv[i]);
			path[strlen(argv[i])] = '\0';
			flag2 = 1;
		}
	}

	if(flag2==0)//直接打印当前目录
		display_dir(flag,path);
	
	if(stat(path,&buf)==-1)//无效路径
		myerr("stat",__LINE__);
	
	//接下来判断是目录文件还是其他文件
	if(S_ISDIR(buf.st_mode))
	{
		if(path[strlen(path)-1]!='/')
		{
			path[strlen(path)] = '/';
			path[strlen(path)+1] = '\0';
		}
		else
			path[strlen(path)] = '\0';
		if(flag3==1)
			display_dir_R(flag,path);
		else
			display_dir(flag,path);
	}
	else 
	{
		display(flag,path);
	}
}


































