/*************************************************************************
	> File Name: C.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月01日 星期三 15时14分50秒
 ************************************************************************/

#include<iostream>
using namespace std;

#include<stdio.h>
#include<string.h>
#include<queue>

struct mi{
    int x;
}ca;

void BFS(int a,int b,struct mi *zhen)
{
    int zx;
    //struct mi zhen[100000];
    int visited[100005];
    //printf("%d\n",-a);
    int xx[3]={0,1,-1};
    memset(visited,0,sizeof(visited));
    queue<mi>q;
    struct mi A;
    A.x=a;
    visited[a]=1;
    zhen[a].x=a;

    q.push(A);
    while(!q.empty())
    {
        struct mi te=q.front();
        q.pop();

        if(te.x==b)
            return;

        for(int i=0;i<3;i++)  //让它走四种情况
        {
            if(i==0)
                zx=te.x+te.x;
            else
                zx=te.x+xx[i];
            //printf("%d\n",zx);

            if(zx<0||zx>100005||visited[zx])
                continue;

            visited[zx]=1;

            struct mi kao;
            kao.x=zx;
            q.push(kao);
            
            //printf("%d\n",zx);
            //记录着谁走到了它
            zhen[zx].x=te.x;
        }
    }
}


int main(void)
{
    int a,b;
    while(scanf("%d%d",&a,&b)!=EOF)
    { 
        struct mi zhen[100005]; 

        BFS(a,b,zhen);

        int x=b;
        int i=0;
        while(x!=a)
        {
            x=zhen[x].x;
            i++;
        }

        printf("%d\n",i);
    
        //return 0;
    }
    return 0;
}
