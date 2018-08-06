/*************************************************************************
	> File Name: F.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月01日 星期三 19时39分37秒
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

//struct mi zhen[5][5];

//初始障碍
char **map;

//int visited[5][5];
int xx[8]={1,-1,0,0,1,1,-1,-1};
int yy[8]={0,0,-1,1,1,-1,1,-1};


int BFS(int a,int b,int ca,int ka)
{
    //int flag=0;
    if(map[a][b]=='*')
        return 0;
    //memset(visited,0,sizeof(visited));
    queue<mi>q;
    struct mi A;
    A.x=a;
    A.y=b;
    //visited[0][0]=1;
    //zhen[0][0].x=0;
    //zhen[0][0].y=0;

    q.push(A);
    while(!q.empty())
    {
        struct mi te=q.front();
        q.pop();

        //if(te.x==4&&te.y==4)
        //    return;
        //int flag=0;
        //if(map[a][b]=='*')
        //    return 0;

        for(int i=0;i<8;i++)  
        {
            int zx=te.x+xx[i];
            int zy=te.y+yy[i];

            if(zx<0||zx>=ca||zy<0||zy>=ka||map[zx][zy]=='*')
                continue;

            //flag=1;
            map[zx][zy]='*';
        
            struct mi  kao;
            kao.x=zx;
            kao.y=zy;
            q.push(kao);

        //记录着谁走到了它
        //zhen[zx][zy].x=te.x;
        //zhen[zx][zy].y=te.y;    
        }

    }
    return 1;
}

int main(void)
{
    //char **map;
    //BFS();
    //int num[20][2];
    

    /*
    int x=4,y=4;
    int i=0;
    while(x!=0&&y!=0)
    {
        x=zhen[x][y].x;
        y=zhen[x][y].y;

        num[i][0]=x;
        num[i][1]=y;
        i++;
    }

    num[i+1][0]=0;
    num[i+1][1]=0;
    for(int j=i+1;j>=0;j--)
    {
        printf("(%d,%d)\n",num[j][0],num[j][1]);
    }
    printf("(4,4)\n");
    */

    int a,b,k,m,key,daan;
    while(1)
    { 
        daan=0;
        scanf("%d%d",&a,&b);
        getchar();
        
        
        if(a==0&b==0)
            break;
                
        map=(char**)malloc(a*sizeof(char*));
        for(m=0;m<a;m++)
	        map[m]=(char *)malloc(b*sizeof(char));

        //memset(map,'\0',sizeof(map));

        char shit;
        for(int m=0;m<a;m++)
        {
            for(int n=0;n<b;n++)
            {
                map[m][n]=getchar();
            }
            getchar();
                //getchar();   
        }

        //printf("%s",map[0]);
        /*
        for(int g=0;g<a;g++)
        {
            for(int fuck=0;fuck<b;fuck++)
            {
                printf("%c",map[g][fuck]);
            }
            printf("\n");
        } 
        */
        
        for(int i=0;i<a;i++)
        {
            for(int j=0;j<b;j++)
            {
                //printf("已执行\n");
                key=BFS(i,j,a,b);
                if(key==1)
                {
                    daan++;
                }
            }
        }
        
        printf("%d\n",daan);
        


        for(int g=0;g<a;g++)
        {            
               free(map[g]);
        }
        free(map);

    }
    return 0;
}
