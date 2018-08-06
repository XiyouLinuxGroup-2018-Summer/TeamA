/*************************************************************************
	> File Name: zD.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月03日 星期五 14时37分01秒
 ************************************************************************/

#include<iostream>
using namespace std;

#include<stdio.h>
#include<string.h>
#include<queue>

struct mi{
    unsigned long long x;
}ca;

void BFS(unsigned long long la)
{
    unsigned long long zx;
    //struct mi zhen[100000];
    //int visited[100005];
    //printf("%d\n",-a);
    //int xx[3]={0,1,-1};
    //memset(visited,0,sizeof(visited));
    queue<mi>q;
    struct mi A;
    A.x=1;
    //visited[0]=1;
    //zhen[a].x=a;

    q.push(A);
    while(!q.empty())
    {
        struct mi te=q.front();
        q.pop();

		//printf("%ld\n",te.x);
        if((te.x%la)==0)
        {
            printf("%llu\n",te.x);
            return;
        }
        for(int i=0;i<2;i++)  
        {
			if(i==0)
            	zx=te.x*10;
            //printf("%d\n",zx);
			if(i==1)
				zx=te.x*10+1;

            //if(zx>999999999999999999)
            //    continue;

            //visited[zx]=1;

            struct mi kao;
            kao.x=zx;
            q.push(kao);
            
            //printf("%d\n",zx);
            //记录着谁走到了它
            //zhen[zx].x=te.x;
        }
    }
}


int main(void)
{
    int a,b;
    unsigned long long la;
    unsigned long long daan;
    while(1)
    { 
		scanf("%llu",&la);
        //struct mi zhen[100005]; 
        if(la==0)
           break;

        //daan=
        //for(long h=1;h<=200;h++)

        BFS(la);

        
        //printf("%lld\n",daan);
    
    }
    return 0;
}


