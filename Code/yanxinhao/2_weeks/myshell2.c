#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
#include<readline/readline.h>
#include<readline/history.h>

void wait_get(char *buf)
{
	int len = 0;
	char ch;

	strcpy(buf,readline(""));
	//ch = getchar();
	//while((ch!='\n')&&len<256)
	//{
	//	buf[len++] = ch;
	//	ch = getchar();
	//}
	len = strlen(buf);
	if(len==256)
	{
		printf("command is too long!\n");
		exit(-1);
	}

	buf[len] = '\n';
	len+=1;
	buf[len] = '\0';

}

void analyze_input(char *buf,int *argc,char argl[100][256])
{
	char *p = buf;
	char *q = buf;
	int num = 0;
	while(1){
		if(p[0]=='\n')
			break;
		if(p[0]==' ')
			p++;
		else{
		  q = p;
		  num = 0;
		  while(q[0]!=' '&&q[0]!='\n'){
			q++;
			num++;
		  }
		  strncpy(argl[*argc],p,num+1);
		  argl[*argc][num] = '\0';
		  *argc += 1;
		  p = q;
		}
	}
}

void analyze_cmd(int argc,char argl[100][256])
{
	int back = 0;
	int how = 0;
	int flag = 0;
	char *arg[argc+1];
	char *arg2[argc+1];
	char *file;
	int fd;
	pid_t pid;
	int status;

	for(int i=0;i<argc;i++){
		arg[i] = (char*)argl[i];
	}
	arg[argc] = NULL;

	for(int i=0;i<argc;i++){
		if(strcmp(arg[i],"&")==0){
			if(i == argc-1){
				back = 1;
				arg[argc-1] = NULL;
				break;
			}
			else{
				printf("wrong command!\n");
				exit(-1);
			}
		}
	}

	for(int i=0;arg[i]!=NULL;i++){

		if(strcmp(arg[i],">") == 0){
			flag++;
			how = 1;
			if(arg[i+1]==NULL){
				flag++;
			}
		}

		if(strcmp(arg[i],"<") == 0){
			flag++;
			how = 2;
			if(arg[i+1]==NULL){
				flag++;
			}
		}

		if(strcmp(arg[i],"|") == 0){
			flag++;
			how = 3;
			if(arg[i+1]==NULL){
				flag++;
			}
			if(i==0){
				flag++;
			}
		}

		if(strcmp(arg[i],">>") == 0){
			flag++;
			how = 4;
			if(arg[i+1]==NULL){
				flag++;
			}
			if(i==0){
				flag++;
			}
		}
	}

		
		if(flag>1){
			printf("command is wrong!\n");
			return;
		}

		if(how==1){
			for(int i=0;arg[i]!=NULL;i++){
				if(strcmp(arg[i],">") ==0 ){
					file = arg[i+1];
					arg[i] = NULL;
				}

			}
		}
		
		if(how==2){
			for(int i=0;arg[i]!=NULL;i++){
				if(strcmp(arg[i],"<") ==0 ){
					file = arg[i+1];
					arg[i] = NULL;
				}
			}
		}

		if(how==3){
			for(int i=0;arg[i]!=NULL;i++){
				if(strcmp(arg[i],"|") == 0){
					arg[i] = NULL;
					int j;
					for(j=i+1;arg[j]!=NULL;j++){
						arg2[j-i-1] = arg[j];
					}
					arg2[j-i-1] = arg[j];
					break;
				}
			}
		}

		if(how==4){
			for(int i=0;arg[i]!=NULL;i++){
				if(strcmp(arg[i],">>") == 0){
					arg[i] = NULL;
					file = arg[i+1];

				}
			}
		}

		if((pid = fork())<0){
			printf("fork error!\n");
			return;
		}

		if(how==0){
			if(pid==0){
				execvp(arg[0],arg);
				exit(0);
			}
		}
		else if(how==1){
			if(pid==0){
				fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
				dup2(fd,1);
				execvp(arg[0],arg);
				exit(0);
			}
		}
		else if(how==2){
			if(pid==0){
				fd = open(file,O_RDONLY);
				dup2(fd,0);
				execvp(arg[0],arg);
				exit(0);
			}
		}
		else if(how==3){
			if(pid==0){
				int pid2;
				int status2;
				int fd2;

				if((pid2=fork())<0){
					printf("fork2 error!\n");
					exit(-1);
				}
				else if(pid2==0){
					fd2 = open("/tmp/y_d_k_f",O_WRONLY|O_CREAT|O_TRUNC,0644);
					dup2(fd2,1);
					execvp(arg[0],arg);
					exit(0);
				}

				if(waitpid(pid2,&status,0)==-1){
					printf("wait for child child process error!\n");
				}

				fd2 = open("/tmp/y_d_k_f",O_RDONLY);
				dup2(fd2,0);
				execvp(arg2[0],arg2);

				if(remove("/tmp/y_d_k_f"))
					printf("remove error!\n");
				exit(0);
				
			}
			
		}
		else if(how==4){	
			if(pid==0){
				fd = open(file,O_RDWR|O_CREAT|O_APPEND,0644);
				dup2(fd,1);
				execvp(arg[0],arg);
				exit(0);
			}
		
		}

		if(back==1){
			printf("[process id is %d]\n",pid);
			return;
		}
		if(waitpid(pid,&status,0)==-1)
			printf("wait process error!\n");
}


void print(void)
{
	char buf[50];
	getwd(buf);
	printf("@yxh:%s$ ",buf);	
}


int main()
{
	int argc = 0;
	char argl[100][256];
	char **arg = NULL;
	char * buf = NULL;
	char *tmp;
//	char *history = (char*)malloc(sizeof(char)*256);

	buf = (char*)malloc(sizeof(char)*256);
	while(1){
		memset(buf,0,256);
		print();
		wait_get(buf);
		if(strncmp(buf,"exit",4)==0){
			break;
		}
		else if(strncmp(buf,"cd",2)==0)
		{
			int i;
			for( i=0;buf[i]!='\n';i++);
			buf[i] = '\0';
			tmp = &buf[3];
			chdir(tmp);

		}
//		strcpy(history,buf);
//			int j;
//			for( j=0;buf[j]!='\n';j++);
//			history[j] = '\0';
//		add_history(history);
		


		for(int i=0;i<100;i++)
			argl[i][0] = '\0';

		argc = 0;
		analyze_input(buf,&argc,argl);
		analyze_cmd(argc,argl);
	}
//	free(history);
		if(buf!=NULL){
			free(buf);
			buf = NULL;
		}


	return 0;
}





