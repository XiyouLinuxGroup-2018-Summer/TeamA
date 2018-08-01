#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<wait.h>
#include<dirent.h>
#include<readline/readline.h>
#include<readline/history.h>
#define normal 0
#define out  1
#define in 2
#define pipe 4
#define app 3
void get_input(char * buf);
void do_cmd(int argcount,char arglist[100][256]);
int  find_cmd(char *command);
void explain_input(char *buf,int *argcount,char st[100][256]);
int main (int argc,char * argv[])
{
    int i,argcount = 0;
    char st[100][256];
    char **arg = NULL;
    char * buf = (char*)malloc(sizeof(char)*256);
    while(1)
    {
        memset(buf,0,256);
        printf("Myshell>>");
        get_input(buf);
        if(strcmp(buf,"exit\n")== 0 || strcmp(buf,"logout\n") == 0)
            break;
        argcount = 0;
        explain_input(buf,&argcount,st);
        do_cmd(argcount,st);
    }
    if(buf != NULL){
        free(buf);
        buf = NULL;
    }
    return 0;
}
void get_input(char * buf)
{
    int i = 0,m = 0;
    char ch;
    while((ch = getchar()) != '\n' && i< 256)
    {
        buf[i] = ch;
        i++;
    }
    if(i>256)
    printf("命令过长\n");
    buf[i] = '\n';
    i++;
    buf[i+1] = '\0';
    //char *p = readline(buf);
}
void explain_input(char *buf,int *argcount,char st[100][256])
{
    char *p = buf;
    char *q = buf;
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
        arg[i] = (char*)arglist[i];
        arg[argcount] = NULL;
         printf("jjjjjjjjjjjj");
      // readline(arg[0]);
    //  char *p= readline(arg[0]);
        add_history(arg[0]);
        //p = readline(arg[0]);
        if(strcmp(arg[0],"cd") == 0)
        {
            if(chdir(arg[1]) < 0)
            printf("文件切换错误\n");
            else
            {
                 getcwd(buf,512);
                puts(buf);
            }
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
                {
                    next[j-i-1] = arg[j];}
                    next[j-i-1] = arg[j];
                    break;
            }
        }
    }
    if((pid = fork()) < 0)
    {
        printf("error\n");
        exit(1);
    }
    switch(type)
    {
        case 0:
                if(pid == 0)
                {
                   if(execvp(arg[0],arg) <0 && strcmp(arg[0],"cd") != 0)
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
                   /* if(execvp(arg[0],arg) <0)
                    {
                        printf("%s :not found\n",arg[0]);
                        exit(0);
                    }*/
                    fd = open(file,O_CREAT|O_RDWR|O_TRUNC,0644);
                    dup2(fd,1);
                    execvp(arg[0],arg);
                    exit(0);
                }
                break;
        case 2:
                if(pid == 0)
                {
                    if(execvp(arg[0],arg) <0)
                    {
                        printf("%s :not found\n",arg[0]);
                        exit(0);
                    }
                    fd = open(file,O_RDONLY);
                    dup2(fd,0);
                    exit(0);
                }
                break;
        case 3:
                 if(pid == 0)
                {
                   /* if(execvp(arg[0],arg) < 0)
                    {
                        printf("%s %d :not found\n",arg[0],__LINE__);
                        exit(1);
                    }*/
                    fd = open(file,O_RDWR|O_CREAT|O_APPEND,0644);
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
                    if((pid = fork()) <0)
                    {
                        printf("fork.error\n");
                        exit(1);
                    }
                    else if(pid2 == 0)
                    {
                        if(execvp(arg[0],arg) <0)
                         {
                            printf("%s :not found\n",arg[0]);
                            exit(0);
                        }
                        fd2 = open("/tmp/test",O_CREAT|O_TRUNC|O_WRONLY,0644);
                        dup2(fd2,1);
                        exit(0);
                    }
                    if(waitpid(pid2,&status2,0) != pid2)
                    {
                        printf("child process exit failed\n");
                        exit(1);
                    }
                   if(execvp(next[0],next) <0)
                     {
                        printf("%s :not found\n",arg[0]);
                        exit(0);
                    }
                    fd2 = open("/tmp/test1",O_RDONLY);
                    dup2(fd2,0);
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
        {
           printf("shibai\n");
           exit(1);
        }
}