/*************************************************************************
	> File Name: ss.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 20时20分18秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>

int m=0;

//用户输入
int shu(char *te)
{
    char c;
    int i=0;
    while((c=getchar())!='\n')
    {
        te[i]=c;
        i++;
    }
    
    te[i]='\0';
    return i;
}

//num为用户输入长度
void ay(char *te,int num,char ch[500][256])
{
    //printf("ok\n");
    char *nb;
    int i=0;
    //extern m=0;
    int j,k=0,flag=0;

    while(i<num)
    {
        flag=0;
        j=0;
        while(te[i]!=' '&&te[i]!='\0')
        {
            flag=1;
        
            ch[m][j]=te[i];
            
            j++;
            i++;

            if(te[i]=='\0')
                break;
        }
        
        if(flag==1)
        {
            ch[m][j]='\0';
            m++;            
        }
        i++;
    }   
}


void shiyan(char ch[500][256])
{

    char *tmp[256];
    int status,r;
    pid_t pid;
    int flag=0;
    char name[10];
    
    for(r=0;r<m;r++)
    {
        tmp[r]=(char*)ch[r];
        //printf("%s\n",tmp[r]);
        
        if(strcmp(tmp[r],"&")==0)
        {
            flag=1;
        }
        
        
    }

    tmp[r]=NULL;
    

    if((pid = fork())<0)
    {
        printf("fork error\n");
        return;
    }
    //pid为0说明是子进程
    if(pid==0)
    {
        //printf("已运行\n");
        execvp(tmp[0],tmp);
        //execl("/ls","ls","-l","/home/chujiangke");
        exit(0);
    }
    
    if(waitpid(pid,&status,0)==-1)
        printf("wait for child process error\n");    
}

int main(void)
{
    char te[256];
    int num;
    char ch[500][256];
    //har *ch;
    
    num=shu(te);
    ay(te,num,ch);
    shiyan(ch);    

    
    /*
    for(int u=0;u<m;u++)
    {
        printf("%s\n",ch[u]);
    }
    */
    return 0;
}


