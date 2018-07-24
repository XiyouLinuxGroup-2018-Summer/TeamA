#include<stdio.h>
#include<unistd.h>
#include<limits.h>
void myerr(const char * err_str,int line)
{
	fprintf(stderr,"line:%d",line);
	perror(err_str);
	exit(1);
}
int main(int argc,char **argv)
{
	char buf[PATH_MAX+1];
	if(argc!=2)
	{
		printf("mycd <target path>\n");
		exit(1);
	}
	if(chdir(argv[1])<0)
	{
		myerr("chdir",__LINE__);
	}
	if(getcwd(buf,512)<0)
	{
		myerr("getcwd",__LINE__);
	}
	printf("%s\n",buf);


}
