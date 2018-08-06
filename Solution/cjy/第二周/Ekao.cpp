/*************************************************************************
	> File Name: Ekao.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月02日 星期四 10时45分44秒
 ************************************************************************/

#include<iostream>
using namespace std;
#include<stdio.h>
#include<string.h>
#include<queue>

struct mi{
    int x;
}ca;

void BFS(int a,int b,int m,struct mi *zhen)
{
    int zx;
    //struct mi zhen[100000];
    int visited[100005];
    //printf("%d\n",-a);
    int xx[2]={b,m};
    memset(visited,0,sizeof(visited));
    queue<mi>q;
    struct mi A;
    A.x=0;
    visited[0]=1;
    zhen[0].x=0;

    q.push(A);
    while(!q.empty())
    {
        struct mi te=q.front();
        q.pop();

        if(te.x==(a))
            return;

        for(int i=0;i<3;i++)  //让它走四种情况
        {
            zx=te.x+xx[i];
            //printf("%d\n",zx);

            if(visited[zx])
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
    int a,b,m;
    while(scanf("%d%d%d",&a,&b,&m)!=EOF)
    {
        struct mi zhen[100005];

        BFS(a,b,m,zhen);

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
