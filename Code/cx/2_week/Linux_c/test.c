#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<wait.h>
#include<sys/resource.h>
int main (int argc,char * argv[])
{
	pid_t pid;
	int old,new;
	int stat_val = 0;
	pid = fork();
	switch(pid)
	{
		case 0:
			printf("child process%d %d\n",pid,getppid());
			old = getpriority(PRIO_PROCESS,0);
			printf("%d\n",old);
			new = nice(2);
			printf("%d",new);
			break;
		case -1:
			printf("creat fail\n");
			break;
		default:
			sleep(1);
			printf("parent process %d\n",getpid());
			break;
	}
	wait(&stat_val);
	return 0;
	
}
