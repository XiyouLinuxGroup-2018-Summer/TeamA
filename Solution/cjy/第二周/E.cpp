/*************************************************************************
	> File Name: E.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月02日 星期四 09时16分11秒
 ************************************************************************/

#include<iostream>
using namespace std;

#include<stdio.h>
#include<string.h>
#include<queue>

struct mi{
    int x;
    int sp;
}ca;

int BFS(int a,int end,struct mi *zhen)
{
    int zx;
    int vv=1;
    int visited[100005];
    int xx[1]={a};
    memset(visited,0,sizeof(visited));
    queue<mi>q;
    struct mi A;
    A.x=0;
    zhen[0].x=0;
    A.sp=0;

    q.push(A);
    while(!q.empty())
    {
        struct mi te=q.front();
        q.pop();
        
        zx=te.x+xx[0];

        struct mi kao;
        kao.x=zx;
        q.push(kao);
        kao.sp=vv;
        vv++;
        
        if(zx>(end/2))
        {
            //printf("%d\n",zx);
            return kao.sp;
        }

        if(zx==(end/2))
        {
            return kao.sp;
        }     
    }
}


int main(void)
{
    int daan;
    int a,b,k,m;
    while(1)
    { 
        scanf("%d%d%d",&a,&b,&m);
        if(a==0&b==0&m==0)
            break;
        struct mi zhen[100005]; 
        
        if(b>m)
        {
            k=m;
        }
        else
            k=b;

        if(a%2!=0)
        {
            printf("NO\n");
            continue;
        }

        if((a/2)%k!=0)
        {
            printf("NO\n");
            continue;
        }

        daan=BFS(k,a,zhen);

        int x=daan;
        printf("%d\n",x+1);
    }
    return 0;
}
    
