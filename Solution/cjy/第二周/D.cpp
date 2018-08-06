/*************************************************************************
	> File Name: D.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月01日 星期三 19时12分47秒
 ************************************************************************/


#include<iostream>
using namespace std;
#include<stdio.h>
#include<string.h>
#include<queue>

int lnum;
//int hang;

struct mi{
    int x;
    int y;
}ca;

//struct mi zhen[5][5];

//初始障碍
char map[20][20];

int sum=0;
int lie[200];


void DFS(int hh,int k,int hang)
{
    if(k==0)
    {     
        sum++;
        //printf("%d",sum);
        return;
    }
    for(int i=hh;i<hang;i++)
    {
        for(int j=0;j<hang;j++)
        {
            if(map[i][j]=='.'||(lie[j]==1))
                continue;
            lie[j]=1;
            DFS(i+1,k-1,hang);
            lie[j]=0;     
        }
    }
}

int main(void)
{

    memset(lie,0,sizeof(lie));

    int a,k,hang,m,key,daan;
    while(1)
    { 
        sum=0;
        daan=0;
        scanf("%d%d",&hang,&k);   //a是矩阵阶数
        getchar();
         
        //printf("%d %d\n",hang,k);
        if(a==-1&k==-1)
            break;
        

        memset(map,'\0',sizeof(map));

        //char shit;
        for(int m=0;m<hang;m++)
        {
            for(int n=0;n<hang;n++)
            {
                cin>>map[m][n];
            }
                //getchar();   
        }

    
        DFS(0,k,hang);
        printf("%d\n",sum);
        

    }
    return 0;
}
