#include<stdio.h>
#include<stdlib.h>
typedef struct node {
    int a;
    int b;
    struct node *pNext;
}Node,*pNode;
int memory[50][50] = {0};
Node way[1000];
int cnt;
Node queue[10000];
int front,tail;
void init(void)
{
    front = 0;
    tail = 0;
    cnt = 0;
}
void wall(void)
{
    int i,j;
    for(i = 0;i< 7;i++)
    {
        for(j = 0;j< 7;j++)
        {
            if(i == 0 || i == 6 || j == 0|| j == 6)
                memory[i][j] = 1;
        }
    }
}
void en_queue(int a,int b,pNode p)
{
    queue[tail].a = a;
    queue[tail].b = b;
    queue[tail].pNext = p;
    tail++;
}
pNode out_queue(void)
{
    pNode p = &queue[front++];
    //front++;
    return p;
}
void bfs(void)
{
    pNode get_elem;
    en_queue(1,1,NULL);
    while(1)
    {
        if(tail == front)
        return;
        get_elem = out_queue();
        if(get_elem->a == 5  && get_elem->b == 5)
        {
            while(get_elem)
            {
                way[cnt].a = get_elem->a;
                way[cnt].b = get_elem->b;
                cnt++;
                get_elem = get_elem->pNext;
            }
            return;
        }
        if(memory[get_elem->a][get_elem->b-1] == 0)
        {
            en_queue(get_elem->a,get_elem->b-1,get_elem);
            memory[get_elem->a][get_elem->b-1] = 1;
        }
        if(memory[get_elem->a][get_elem->b+1] == 0)
        {
            en_queue(get_elem->a,get_elem->b+1,get_elem);
            memory[get_elem->a][get_elem->b+1] = 1;
        }
        if(memory[get_elem->a-1][get_elem->b] == 0)
        {
            en_queue(get_elem->a-1,get_elem->b,get_elem);
            memory[get_elem->a-1][get_elem->b] = 1;
        }
        if(memory[get_elem->a+1][get_elem->b] == 0)
        {
            en_queue(get_elem->a+1,get_elem->b,get_elem);
            memory[get_elem->a+1][get_elem->b] = 1;
        }
    }
}
void traverse(void)
{
    int i;
    for(i = cnt - 1;i>=0;i--)
        printf("(%d, %d)\n",way[i].a-1,way[i].b-1);
}
int main (void)
{
    int i,j;
    while(1)
    {
        init();
        for(i = 1;i<=5;i++)
        {
            for(j = 1;j<=5;j++)
            {
                if(scanf("%d",&memory[i][j]) == EOF)
                {
                printf("bu治所云\n");
                return 0;
                }
            }
        }
        wall();
        bfs();
        traverse();
    }
    return 0;
}