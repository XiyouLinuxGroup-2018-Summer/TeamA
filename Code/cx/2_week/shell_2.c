#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<wait.h>
#include<dirent.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<sys/types.h>
#include<signal.h>
#define normal 0
#define out  1
#define in 2
#define pipe 4
#define app 3
int m = 0;
void do_cmd(int argcount,char arglist[100][256]);
void explain_input(char *buf,int *argcount,char st[100][256]);
int main (int argc,char * argv[])
{
    int i,argcount = 0;
    char st[100][256];
    FILE * fp;
    char * buf = (char*)malloc(sizeof(char)*256);
    memset(buf,0,256);
    char temp[80];
    while(1)
    {
        system("hostname");
	getcwd(temp,512);
       	printf("[cxinsect:%s]",temp);
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGSTOP, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        if((fp = fopen("/tmp/testfile","a+")) == NULL)
        {
            printf("文件打开失败\n");
            exit(1);
        }
	memset(buf,0,sizeof(buf));
	strcpy(buf,readline(" "));
        fprintf(fp,"%s",buf);
        fputc('\n',fp); 
        if(strcmp(buf,"exit")== 0)
            break;
        argcount = 0;
        for(int i;i<strlen(buf);i++)
            if(buf[i] == '\n')
               continue;
        add_history(buf);
        explain_input(buf,&argcount,st);
        fclose(fp);
        do_cmd(argcount,st);
    }
    if(buf != NULL){
        free(buf);
        buf = NULL;
    }
    return 0;
}
void explain_input(char *buf,int *argcount,char st[100][256])
{
    char *p = buf;
    char * q = buf;
    int number = 0;
    while(1)
    {
        if(p[0] == '\n')
        break;
        if(p[0] == ' ')
        p++;
        else
        {
            number = 0;
            q = p;
            while((q[0] != '\n') && (q[0] != ' '))
            {
                number++;
                q++;
            }
            strncpy(st[*argcount],p,number+1);
            st[*argcount][number] = '\0';
            *argcount += 1;
            p = q;
        }
    }
    return;
}
void do_cmd(int argcount,char arglist[100][256])
{
    FILE *fp;
    int flag = 0;
    int type = 0;
    int back = 0;
    int status,i,fd;
    char * arg[argcount+1];
    char * next[argcount+1];
    char * file;
    pid_t pid;
    char buf[512];
    for(i = 0;i < argcount;i++)
        arg[i] = arglist[i];
        arg[argcount] = NULL;
        if(strcmp(arg[0],"cd") == 0)
        {
            if(chdir(arg[1]) < 0)
            printf("文件切换错误\n");
            else
		         return;
        }
        arg[0][strlen(arg[0]) +1] = '\0';
        if(strcmp(arg[0],"history") == 0)
        {
            char string[512];
            if((fp = fopen("/tmp/testfile","r")) == NULL)
            {
                printf("文件读取失败\n");
                exit(1);
            }
            while(fscanf(fp,"%s",string)!= EOF)
                 puts(string);
            return;
        }
    for(i = 0;i<argcount;i++)
    {
        if(strncmp(arg[i],"&",1) == 0)
        {
            if(i == argcount - 1)
            {
                back = 1;
                arg[argcount - 1] = NULL;
                break;
            }
            else
            printf("wrong command\n");
            return;
        }
    }
    for(i = 0;arg[i] != NULL;i++)
    {
        if(strcmp(arg[i],">>") == 0)
                type = app;
        if(strcmp(arg[i],">") == 0)
                type = out;   
        if(strcmp(arg[i],"<") == 0)
            type = in;
        if(strcmp(arg[i],"|") == 0)
            type = pipe;
    }
    if(type == out)
    {
        for(i = 0;arg[i] != NULL;i++)
        {
            if(strcmp(arg[i],">") == 0)
            {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }
    if(type == app)
    {
        for(i = 0;arg[i] != NULL;i++)
        {
            if(strcmp(arg[i],">>") == 0)
            {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }
    if(type == in)
    {
        for(i = 0;arg[i] != NULL;i++)
        {
            if(strcmp(arg[i],"<") == 0)
            {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }
    if(type == pipe)
    {
        for(i = 0;arg[i] != NULL;i++)
        {
            if(strcmp(arg[i],"|") == 0)
            {
                arg[i] = NULL;
                int j;
                for(j = i+1;arg[j] != NULL;j++)
                    next[j-i-1] = arg[j];
                    next[j-i-1] = arg[j];
                    break;
            }
        }
    }
    if( (pid = fork()) < 0)
    {
        printf("error\n");
        exit(0);
    }
    switch(type)
    {
        case 0:
                if(pid == 0)
                {
                   if(execvp(arg[0],arg) <0 && strcmp(arg[0],"cd") != 0 && strcmp(arg[0],"cd") != 0)
                    {
                        printf("%s :not found\n",arg[0]);
                        exit(0);
                    }
                    exit(0);
                }
                break;
        case 1:
                if(pid == 0)
                {
                    fd = open(file,O_CREAT|O_RDWR|O_TRUNC,0644);
                    dup2(fd,1);
                    execvp(arg[0],arg);
                    exit(0);
                }
                break;
        case 2:
                if(pid == 0)
                {
                    fd = open(file,O_RDONLY);
                    dup2(fd,0);
                    execvp(arg[0],arg);
                    exit(0);
                }
                break;
        case 3:
                 if(pid == 0)
                {
                    fd = open(file,O_RDWR|O_APPEND,0644);
                    dup2(fd,1);
                    execvp(arg[0],arg);
                    exit(0);
                }
                    break;
            case 4:
                    if(pid == 0)
                 {
                    pid_t pid2;
                    int status2;
                    int fd2;
                    
                    if((pid2 = fork()) <0)
                    {
                        printf("fork.error\n");
                        exit(1);
                    }
                    else if(pid2 == 0)
                    {
                        fd2 = open("/home/cxinsect/shpipetmp",O_CREAT|O_TRUNC|O_WRONLY,0644);
                        dup2(fd2,1);
                        execvp(arg[0],arg);
                        exit(0);
                    }
                    if(waitpid(pid2,&status2,0) == -1)
                        printf("child process exit failed\n");
                    fd2 = open("/home/cxinsect/shpipetmp",O_RDONLY);
                    dup2(fd2,0);
                    execvp(next[0],next);
                    exit(0);
                }
                break;
            default:
                break;
        }
        if(back == 1)
        {
            printf("process %d\n",pid);
            return;
        }
        if(waitpid(pid,&status,0) == -1)
           printf("shibai\n");
	return;
}
