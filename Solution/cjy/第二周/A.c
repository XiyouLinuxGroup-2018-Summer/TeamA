B
/*************************************************************************
	> File Name: A.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月01日 星期三 10时25分51秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<queue>
using namespace std;

struct mi{
    int x,y
}ca;


ca zhen[5][5];

int map[5][5]={
                {0,1,0,0,0},
                {0,1,0,1,0},
                {0,0,0,0,0},
                {0,1,1,1,0},
                {0,0,0,1,0}
              }

int visited[5][5]; 

int xx[2]={1,-1,0,0};
int yy[2]={0,0,-1,1};

void BFS()
{
    memset(visited,0,sizeof(visited));
    queue<Maze>q;
    ca A;
    A.x=0;
    A.y=0;
    visited[0][0]=1;
    zhen[0][0].x=0;
    zhen[0][0].y=0;
    
    q.push(A);
    while(!q.empty())
    {
        ca te=q.front();
        q.pop();

        if(te.x==4&&te.y==4)
            return;

        for(int i=0;i<4;i++)  //让它走四种情况
        {
            int zx=te.x+xx[0];
            int zy=te.y+yy[0];

            if(dx<0||dx>4||dy<0||dy>4||map[zx][zy]||visited[zx][zy])
                continue;
            
            visited[zx][zy]=1;
               
            ca kao;
            kao.x=zx;
            kao.y=zy;
            q.push(kao);
            zhen[zx][zy]=te.x;
            zhen[zx][zy]=te.y;
        }
    }
}

int main(void)
{
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            printf("(%d,%d)\n",zhen[i][j]);
        }
    }

    return 0;
}
