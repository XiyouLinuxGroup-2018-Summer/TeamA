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
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>

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
int ay(char *te,int num,char ch[500][256])
{
    char *nb;
    int i=0;
    int j,k=0,flag=0;
    int m=0;

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
    return m;
}


void shiyan(char ch[500][256],int m)
{
    char *tmp[256];
    char *ssp[256];
    int status,status2,r,u=0;
    pid_t pid,pid2;
    int flag=0,fd,hd,sflag=0;
    char *name;

    if(strcmp(ch[m-1],"&")==0)
    {
        sflag=1;
        m--;
    }

    for(r=0;r<m;r++)
    {
        tmp[r]=(char*)ch[r];
        //printf("%s\n",tmp[r]);         
        if(strcmp(tmp[r],">")==0)
	    {
	        flag=2;
            name=(char*)ch[r+1];
            tmp[r]=NULL;
            break;
	    }
        
        else if(strcmp(tmp[r],"<")==0)
	    {
	        flag=3;
            name=(char*)ch[r+1];
            tmp[r]=NULL;
            break;
	    }
        
        else if(strcmp(tmp[r],"|")==0)
	    {
	        flag=4;
            tmp[r]=NULL;
            for(int la=r+1;la<m;la++)
            {
                ssp[u]=(char*)ch[la];
                u++;
            }
            ssp[u]=NULL;
            break;
	    }
    }

    if(flag==0)
        tmp[r]=NULL;
    

    if((pid = fork())<0)
    {
        printf("fork error\n");
        return;
    }
    //pid为0说明是子进程
    if(pid==0)
    {
        if(flag==2)
        {
            fd=open(name,O_RDWR|O_CREAT|O_TRUNC,0644);
            dup2(fd,1);                //改标准输出 为 fd对应的文件
        }        
        else if(flag==3)
        {
            fd=open(name,O_RDONLY);
            dup2(fd,0);                //改标准输入 为 fd对应的文件
        }

        else if(flag==4)
        {
            if((pid2=fork())<0)
            {
                printf("fork error\n");
                return;    
            }
            
            //再打开一个进程
            if(pid2==0)
            {
                hd=open("tmp",O_WRONLY|O_CREAT|O_TRUNC,0644);
                dup2(hd,1);
                execvp(tmp[0],tmp);
                exit(0);
            }
            
            //必须使用等待
	        if(waitpid(pid2,&status2,0)==-1)
                printf("wait for child process error\n");    
            
            //printf("到这了\n");
            hd=open("tmp",O_RDONLY);
            dup2(hd,0);
            execvp(ssp[0],ssp);
            //printf("到这了\n");
            exit(0);
        }
        
        if(flag!=4)
        {
            execvp(tmp[0],tmp);   
            exit(0);
        }   
    }

    if(sflag==1)
        return;

    if(waitpid(pid,&status,0)==-1)
        printf("wait for child process error\n");    
}

int main(void)
{  
    while(1)
    {
        char te[256];
        int num,ca;
        char ch[500][256];
        printf("jianshell$$ ");
        num=shu(te);
        if(strcmp(te,"exit")==0)
            break;
        ca=ay(te,num,ch);
        shiyan(ch,ca);
    }

    exit(0);
}


