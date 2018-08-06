/*************************************************************************
	> File Name: B.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月01日 星期三 11时07分09秒
 ************************************************************************/

#include<iostream>
using namespace std;
#include<stdio.h>
#include<string.h>
#include<queue>

struct mi{
    int x;
    int y;
}ca;

struct mi zhen[5][5];

//初始障碍
int map[5][5];
            /*
             ={
                {0,1,0,0,0},
                {0,1,0,1,0},
                {0,0,0,0,0},
                {0,1,1,1,0},
                {0,0,0,1,0},
              };
            */

int visited[5][5];
int xx[4]={1,-1,0,0};
int yy[4]={0,0,-1,1};

void BFS()
{
    memset(visited,0,sizeof(visited));
    queue<mi>q;
    struct mi A;
    A.x=0;
    A.y=0;
    visited[0][0]=1;
    zhen[0][0].x=0;
    zhen[0][0].y=0;

    q.push(A);
    while(!q.empty())
    {
        struct mi te=q.front();
        q.pop();

        if(te.x==4&&te.y==4)
            return;

        for(int i=0;i<4;i++)  //让它走四种情况
        {
            int zx=te.x+xx[i];
            int zy=te.y+yy[i];

            if(zx<0||zx>4||zy<0||zy>4||map[zx][zy]||visited[zx][zy])
                continue;

            visited[zx][zy]=1;

            struct mi kao;
            kao.x=zx;
            kao.y=zy;
            q.push(kao);
            
            //printf("%d %d\n",zx,zy);
            //记录着谁走到了它
            zhen[zx][zy].x=te.x;
            zhen[zx][zy].y=te.y;
        }
    }          
}

int main(void)
{

    for(int m=0;m<5;m++)
    {
        for(int n=0;n<5;n++)
        {
            scanf("%d",&map[m][n]);
        }
        //printf("\n");
    }

    BFS();
    int num[30][2];

    //printf("%d %d\n",zhen[3][4].x,zhen[3][4].y);
    //printf("????\n");

    int x=4,y=4;
    int i=0;
    while(1)
    {
        //printf("%d,%d\n",x,y);
        int k;
        k=x;
        x=zhen[x][y].x;
        y=zhen[k][y].y;
       // printf("%d,%d\n",zhen[3][4].x,zhen[3][4].y);
        //printf("%d,%d\n",x,y);
        //printf("已执行\n");
        
        num[i][0]=x;
        num[i][1]=y;  
        i++;
        if(x==0&&y==0)
            break;
    }

    //num[i][0]=0;
    //num[i][1]=0;
    
    for(int j=i-1;j>=0;j--)
    {
        printf("(%d,%d)\n",num[j][0],num[j][1]);
    }
    printf("(4,4)\n");
    
      
    return 0;
}
