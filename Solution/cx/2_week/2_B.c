#include<stdio.h>
#include<string.h>
typedef struct node
{
    int station;
    int step;
}Node;
#define MAX 100010
Node  queue[MAX];
int step;
 int memory[MAX] = {0};
int n,k,rear,front;
void init()
{
    rear = 0;
    front = 0;
}
int bfs(int n,int k)
{
    init();
    Node temp;
    memory[n] = 1;
    queue[rear].station = n;
    queue[rear].step = 0;
    rear++;
    while(front < rear)
    {
        temp = queue[front++];
        if(temp.station+1 <= k && memory[temp.station+1] == 0)
        {
            queue[rear].step = temp.step+1;
            queue[rear++].station = temp.station+1;
            memory[temp.station+1] = 1;
            if(queue[rear-1].station == k)
            return queue[rear-1].step;
        }
        if(temp.station-1 >= 0 && memory[temp.station-1] == 0)
        {
            queue[rear].step = temp.step+1;
            queue[rear++].station = temp.station-1;
             memory[temp.station+1] = 1;
             if(queue[rear-1].station == k)
           return queue[rear-1].step;
        }
        if(temp.station*2 <=k && memory[temp.station*2] == 0)
        {
            queue[rear].step = temp.step+1;
            queue[rear++].station = temp.station*2;
            memory[temp.station*2] = 1;
             if(queue[rear-1].station == k)
           return queue[rear-1].step;
        }
    }
return 0;
}
int main (void)
{
    int n,k,m;
    while(~scanf("%d %d",&n,&k) && n >= 0 && k >= 0)
    {
        memset(memory,0,sizeof(memory));
        m = bfs(n,k);
        printf("%d\n",m);
    }
    return 0;
}