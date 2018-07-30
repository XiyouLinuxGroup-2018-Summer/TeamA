/*************************************************************************
	> File Name: sy.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月25日 星期三 09时52分20秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>


#include<grp.h>
#include<pwd.h>
#include<dirent.h>
#include<string.h>

#define MAXROWLEN 40

#define PRINT_RED(s)    printf("\033[0;31m%s\033[0;39m\n",s);
#define PRINT_GREEN(s)    printf("\033[0;32m%s\033[0;39m\n",s);
#define PRINT_YELLOW(s)    printf("\033[0;33m%s\033[0;39m\n",s);
#define PRINT_BULE(s)    printf("\033[0;34m%s\033[0;39m\n",s);
#define PRINT_PURPLE(s)    printf("\033[0;35m%s\033[0;39m\n",s);



int g_leave_len = MAXROWLEN;
int g_maxlen;

void displayof(char name[])
{
    struct stat hen;
    int flag=0;
    int k;
    char te[100];

    struct passwd *ag;
    struct group *we;


    if(stat(name,&hen)==-1)
    {
        perror("stat:");
        exit(1);
    }

    //printf("到这一步了4\n");

    if(S_ISLNK(hen.st_mode))
        printf("I");
    else if(S_ISREG(hen.st_mode))
    {
        printf("-");
        //flag=1;
    }
    else if(S_ISDIR(hen.st_mode))
        printf("d");
    else if(S_ISCHR(hen.st_mode))
        printf("c");
    else if(S_ISBLK(hen.st_mode))
        printf("b");
    else if(S_ISFIFO(hen.st_mode))
        printf("p");
    else if(S_ISSOCK(hen.st_mode))
        printf("s");

    
    //所有者权限
    if(hen.st_mode&S_IRUSR)
        printf("r");
    else
        printf("-");

    if(hen.st_mode&S_IWUSR)
        printf("w");
    else
        printf("-");

    if(hen.st_mode&S_IXUSR)
        printf("x");
    else
        printf("-");

    //组权限strcmp
    if(hen.st_mode&S_IRGRP)
        printf("r");
    else
        printf("-");

    if(hen.st_mode&S_IWGRP)
        printf("w");
    else
        printf("-");

    if(hen.st_mode&S_IXGRP)
        printf("x");
    else
        printf("-");

    //其他人权限
    if(hen.st_mode&S_IROTH)
        printf("r");
    else
        printf("-");

    if(hen.st_mode&S_IWOTH)
        printf("w");
    else
        printf("-");

    if(hen.st_mode&S_IXOTH)
        printf("x");
    else
        printf("-");


    //if(flag==0)
    //    printf("  1");
    //else
    //{
    printf("%4ld",hen.st_nlink);
    //}
    
    ag=getpwuid(hen.st_uid);
    we=getgrgid(hen.st_gid);


    printf(" ");
    printf("%s",ag->pw_name);
    printf(" ");
    printf("%s",we->gr_name);
    printf(" ");

    printf("%6ld",hen.st_size);

    printf("  ");
    
    k=strlen(ctime(&hen.st_mtime));

    strncpy(te,ctime(&hen.st_mtime),k);
    te[k-1]='\0';


    printf("%s",te);
    printf("  ");
    


    if(S_ISDIR(hen.st_mode))
    {
        PRINT_BULE(name)
    }
    else
        printf("%s\n",name);
}


void pr(char *name)
{
    int i,len;

    if(g_leave_len<g_maxlen)
    {
        printf("\n");
        g_leave_len=MAXROWLEN;
    }

    len=strlen(name);
    len=g_maxlen-len;
    printf("%-s",name);

    for(i=0;i<len;i++)
    {
        printf(" ");
    }

    printf("  ");

    g_leave_len-=(g_maxlen+2);
}

void displayml(char hei[],int ch)
{
    DIR * dir;
    struct dirent *ptr;
    char sum[100][100];
    int i=0,k,p,c;
    char te[100];

    if((dir=opendir(hei))==NULL)
    {
        perror("opendir");
        return;
    }

    while((ptr=readdir(dir))!=NULL)
    {
        k=strlen(ptr->d_name);
        strncpy(sum[i],ptr->d_name,k);
        //sum[i][k]='\0';
        //printf("file name:%s\n",sum[i]);
        i++;
    }

    for(int y=0;y<i;y++)
    {
        for(int j=y+1;j<i;j++)
        {
            if(sum[j][0]<sum[y][0])
            {
                p=strlen(sum[y]);
                strncpy(te,sum[y],p);
                te[p]='\0';

                c=strlen(sum[j]);
                strncpy(sum[y],sum[j],c);
                sum[y][c]='\0';

                strncpy(sum[j],te,p);
                
                sum[j][p]='\0';
            }
        }
    }

    for(int u;u<i;u++)
    {
	    if(ch==0)
            if((strcmp(sum[u],".")==0)||(strcmp(sum[u],"..")==0))
                continue;
        pr(sum[u]);
    }

    closedir(dir);

    return;
}


int sb(char *b[],int num,char *wen,int *flag)
{
    int i,j,k=0,na;
    char te[50];
    flag[0]=0;   
    //printf("%s\n",b[1]);
    //printf("%c\n",b[1][0]);
    //printf("%c\n",b[1][1]);

    for(i=1;i<num;i++)
    {
        if(b[i][0]=='-')
        {
            if(b[i][1]=='a')
            {
                te[k]='a';
                k++;

                if(b[i][2]=='l')
                {
                    te[k]='l';
                    k++;
                    if(b[i][3]=='R')
                    {
                        te[k]='R';
                        k++;
                    }   
                }
                if(b[i][2]=='R')
                {
                    te[k]=='R';
                    k++;
                    if(b[i][3]=='l')
                    {
                        te[k]='l';
                        k++;
                    }
                }
            }



            if(b[i][1]=='l')
            {
                te[k]='l';
                k++;


                if(b[i][2]=='a')
                {
                    te[k]='a';
                    k++;
		            if(b[i][3]=='R')
                    {
                        te[k]='R';
                        k++;
                    }   
                }		
		        if(b[i][2]=='R')
                {
                    te[k]=='R';
                    k++;
                    if(b[i][3]=='a')
                    {
                        te[k]='a';
                        k++;
                    }
                }


            }

            
            if(b[i][1]=='R')
            {
		        te[k]='R';
                k++;


                if(b[i][2]=='a')
                {
                    te[k]='a';
                    k++;
		            if(b[i][3]=='l')
                    {
                        te[k]='l';
                        k++;
                    }   
                }		
		        if(b[i][2]=='l')
                {
                    te[k]=='l';
                    k++;
                    if(b[i][3]=='a')
                    {
                        te[k]='a';
                        k++;
                    }
                }
            }
        }


        else
        {
            na=strlen(b[i]);
            strncpy(wen,b[i],na);
            wen[na]='\0';
            flag[0]=1;
            //printf("%s",wen);
        }   
        
    }
    int f1=0,f2=0,f3=0;

    for(int y=0;y<k;y++)
    {
        if(te[y]=='a')
        {
            f1=1;
        }
        if(te[y]=='l')
        {
            f2=1;
        }
        if(te[y]=='R')
        {
            f3=1;
        }
    }  
    

    
    if(f1==1&&f2==1&&f3==1)
        return 7;
    else if(f1==1&&f3==1)
        return 5;
    else if(f2==1&&f3==1)
        return 6;


    else if(f1==1&&f2==1)
        return 3;
        
    else if(f3==1)
        return 4;    
    else if(f1==1)
        return 1;
    else if(f2==1)
        return 2;
    else 
        return 0;


}


void displayl(char hei[],int uu,int ff)
{
    DIR * dir;
    struct dirent *ptr;
    char sum[100][100];
    int i=0,k,p,c;
    char te[100];

    if((dir=opendir(hei))==NULL)
    {
        perror("opendir");
        return;
    }
    
    //printf("到这一怒了2\n");

    
     while((ptr=readdir(dir))!=NULL)
    {
        k=strlen(ptr->d_name);
        strncpy(sum[i],ptr->d_name,k);
        //sum[i][k]='\0';
        //printf("file name:%s\n",sum[i]);
        i++;
    }
    
    //printf("但这一步了3\n");

    for(int y=0;y<i;y++)
    {
        for(int j=y+1;j<i;j++)
        {
            if(sum[j][0]<sum[y][0])
            {
                p=strlen(sum[y]);
                strncpy(te,sum[y],p);
                te[p]='\0';

                c=strlen(sum[j]);
                strncpy(sum[y],sum[j],c);
                sum[y][c]='\0';

                strncpy(sum[j],te,p);
                
                sum[j][p]='\0';
            }
        }
    }

    for(int u;u<i;u++)
    {
        if(ff==0||ff==2)
            if((strcmp(sum[u],".")==0)||(strcmp(sum[u],"..")==0))
                continue;
        if(uu==1)
            chdir(hei);
        displayof(sum[u]);
    }
    if(uu==1)
        chdir("../");
    closedir(dir);

    return;

}

void Rstar(char *hei)
{
   
    int flag=0;
    
    struct stat hen;

    DIR * dir;
    struct dirent *ptr;
    char sum[100][100];
    int i=0,k,p,c;
    char te[100];

    if((dir=opendir(hei))==NULL)
    {
        perror("opendir");
        return;
    }
        
    while((ptr=readdir(dir))!=NULL)
    {
        k=strlen(ptr->d_name);
        strncpy(sum[i],ptr->d_name,k);
        //sum[i][k]='\0';
        //printf("file name:%s\n",sum[i]);
        i++;
    }
    
    printf("%s\n",sum[i]);
    //chdir(hei);
    
    for(int y=0;y<i;y++)
    {
        for(int j=y+1;j<i;j++)
        {
            if(sum[j][0]<sum[y][0])
            {
                p=strlen(sum[y]);
                strncpy(te,sum[y],p);
                te[p]='\0';

                c=strlen(sum[j]);
                strncpy(sum[y],sum[j],c);
                sum[y][c]='\0';

                strncpy(sum[j],te,p);
                
                sum[j][p]='\0';
            }
        }
    }
    
    
    //printf("%s\n",sum[i]);

    char buf[100];
    getcwd(buf,100);

    printf("%s\n",buf);
    for(int u=0;u<i;u++)
    {
        pr(sum[u]);
        //printf("%s  ",sum[u]);
    }


    printf("\n\n\n");



    for(int u=0;u<i;u++)
    {
        //if((chdir(hei))==0)
           //printf("成功\n");
            
        //chdir(hei);
        char cuf[100];
        getcwd(cuf,100);

        printf("%s\n",cuf);
        printf("%s\n",sum[u]);
        if(stat(sum[u],&hen)==-1)
        {
            printf("%s\n",sum[u]);
            perror("stat:");
            exit(1);
        }
        //chdir("..");
        if(S_ISDIR(hen.st_mode))
        {     
            if((strcmp(sum[u],".")==0)||(strcmp(sum[u],"..")==0))
                continue;
            chdir(sum[u]);
	        Rstar(".");
            //目录文件
            chdir("..");
            flag=1;
	    }
	/*
        if(ff==0||ff==2)
            
        if(uu==1)
            chdir(hei);
        displayof(sum[u]);
	*/       
    }
    //if(flag=0)
        //chdir("..");    
}





int main(int argc,char *argv[])
{    	    
    int ch,flag1,flag2,flag3;
    int flag[1];
    int cflag;
    struct stat hen;
    char mb[50];

    char wen[30];
    
    ch=sb(argv,argc,wen,flag);


    //有文件时
    if(flag[0]==1)
    {
        ////没有命令但是有文件
        if(ch==0)
        {
            if(stat(wen,&hen)==-1)
    	    {
                perror("stat:");
                exit(1);
    	    }

            if(S_ISREG(hen.st_mode))
            {
                flag1=0;                //普通文件
            }
            else if(S_ISDIR(hen.st_mode))
                flag1=1;                //目录文件
            
            
            if(flag1==0)
                printf("%s",wen);
            else if(flag1==1)
                displayml(wen,ch);
            }

	
	    if(ch==1)
	    {
	        if(stat(wen,&hen)==-1)
    	        {
                    perror("stat:");
                    exit(1);
    	        }

                if(S_ISREG(hen.st_mode))
                {
                    flag1=0;                //普通文件
                }
                else if(S_ISDIR(hen.st_mode))
                    flag1=1;                //目录文件
            
            
                if(flag1==0)
                    printf("%s",wen);
                else if(flag1==1)
                    displayml(wen,ch);
	    }
	    else if(ch==2)
	    {
	        if(stat(wen,&hen)==-1)
    	    {
                perror("stat:");
                exit(1);
    	    }

            if(S_ISREG(hen.st_mode))
            {
                flag1=0;                //普通文件
            }
            else if(S_ISDIR(hen.st_mode))
                flag1=1;                //目录文件
            
            
            if(flag1==0)
                displayof(wen);
            else if(flag1==1)
            {
    	        //切换工作目录
                //B
                //chdir(argv[2]);
                //printf("但这一步了\n");
		        displayl(wen,1,ch);
	        }    
        }           
        
        else if(ch==3)
        {
             if(stat(wen,&hen)==-1)
    	    {
                perror("stat:");
                exit(1);
    	    }

            if(S_ISREG(hen.st_mode))
            {
                flag1=0;                //普通文件
            }
            else if(S_ISDIR(hen.st_mode))
                flag1=1;                //目录文件
            
            
            if(flag1==0)
                displayof(wen);
            else if(flag1==1)
            {
    	        //切换工作目录
                //B
                //chdir(argv[2]);
                //printf("但这一步了\n");
		        displayl(wen,1,ch);
	        }        
        }

        /*
        else if(ch==4)
        {
                 
        }
        else if(ch==5)
        {

        }
        else if(ch==6)
        {
            
        }
        */
            
    }

    else if(flag[0]==0)
    {
        
        if(ch==0)
        {
            char buf[100];
            getcwd(buf,100);
            displayml(buf,ch);
        }

	    if(ch==1)
        {
            char buf[100];
            getcwd(buf,100);
            displayml(buf,ch);
        }

        if(ch==2)
        {
            char buf[100];
            getcwd(buf,100);

            displayl(buf,0,ch);
            
        }
	    if(ch==3)
	    {
	        char buf[100];
            getcwd(buf,100);

	        if(stat(buf,&hen)==-1)
    	    {
                perror("stat:");
                exit(1);
    	    }

            if(S_ISREG(hen.st_mode))
            {
                flag1=0;                //普通文件
            }
            else if(S_ISDIR(hen.st_mode))
                flag1=1;                //目录文件
            
            
            if(flag1==0)
                displayof(buf);
            else if(flag1==1)
            {
    	        //切换工作目录
                //B
                //chdir(argv[2]);
                //printf("但这一步了\n");
		        displayl(buf,0,ch);
	        }  	
	    }

        if(ch==4)
        {
            char buf[100];
            getcwd(buf,100);
            Rstar(buf);    
        }


        /*
        if(ch==5)
        {
            
        }

        if(ch==6)
        {

        }

        if(ch==7)
        {
            
        }

        */
    }    
    return 0;

}
