/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月28日 星期六 16时26分28秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

typedef struct file{
    int fid; // 文件编号
    char filename[128];
    int mode;
    // 直接使用整型存储的文件权限(例如 755 存储为 493 也就是 0755)
    int size; // 大小
    char data[256]; // 文件内容
} myfile;

struct node{
    myfile te;
    struct node *next;

};


void show(struct node *a)
{
    while(a!=NULL)
    {
        printf("%s\n",a->te.filename);
        a=a->next;
    }  
}


int main(int argv,char *argc[])
{
    DIR *fd;
    struct dirent *pe;
    struct stat uu;
    
    

    struct node *hh,*pre=NULL,*head=NULL;
    int i=1,j,k;
	
    if((fd=opendir("/lib"))==NULL)
	    printf("打开错误\n");
 
    while((pe=readdir(fd))!=NULL)
    {
        //printf("已进入\n");
	    hh=malloc(sizeof(struct node));
        hh->next=NULL;
       
        //printf("已成功\n");
        hh->te.fid=i;
        int pp=strlen(pe->d_name);
        strncpy(hh->te.filename,pe->d_name,pp);
        hh->te.filename[pp]='\0';
        
        //printf("已成功\n");
        //printf("%s\n",hh->te.filename);
        if(lstat(hh->te.filename,&uu)==-1)
            printf("无法获得文件信息\n");
        
        hh->te.size=uu.st_size;
           
        i++;

	    if(head==NULL)
	        head=hh;
	    else
	        pre->next=hh;
	    pre=hh;     
    }

    show(head);
     
    return 0;
}






