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
        flag=1;
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


    if(flag==0)
        printf("  1");
    else
    {
        printf("  %ld",hen.st_nlink);
    }
    
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
    printf("%s\n",name);
}

void displayml(char hei[])
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
        printf("%s\n",sum[u]);
    }

    closedir(dir);

    return;
}

int sb(char b[])
{
    if(b[0]=='-')
    {
        if(b[1]=='a')
        {
            return 1;
        }

        if(b[1]=='l')
        {
            return 2;
        }
    }
    else
        return 0;
}


void displayl(char hei[],int uu)
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



int main(int argc,char *argv[])
{
    int ch,flag1,flag2,flag3;
    int cflag;
    struct stat hen;

    
    if(argc==2)
    {
        ch=sb(argv[1]);
        
        //当单参且不是命令时
        if(ch==0)
        {
            if(stat(argv[1],&hen)==-1)
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
                printf("%s",argv[1]);
            else if(flag1==1)
                displayml(argv[1]);
            }

        if(ch==1)
        {
            char buf[100];
            getcwd(buf,100);
            displayml(buf);
        }

        if(ch==2)
        {
            char buf[100];
            getcwd(buf,100);

            displayl(buf,0);
            
        }
            
    }

    else if(argc==3)
    {
        ch=sb(argv[1]);
        
	    if(ch==1)
	    {
	        if(stat(argv[2],&hen)==-1)
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
                    printf("%s",argv[2]);
                else if(flag1==1)
                    displayml(argv[2]);
	    }
	    else if(ch==2)
	    {
	        if(stat(argv[2],&hen)==-1)
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
                displayof(argv[2]);
            else if(flag1==1)
            {
                //切换工作目录
                //chdir(argv[2]);
                //printf("但这一步了\n");
		        displayl(argv[2],1);
	        }    
        }           
    }

    
    return 0;
}
