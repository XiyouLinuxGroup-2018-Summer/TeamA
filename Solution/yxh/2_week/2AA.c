#include<stdio.h>


int main(void)
{
struct note{
	int x;
	int y;
	int f;
	int s;
};
	struct note que[125];
	int num[5][5] = {0};
	int book[5][5] = {0};
	int next[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};

	int head = 1;
	int tail = 1;

	int tx,ty;
	int flag = 0;

	int tmp[25]={0};
	int len;

	que[head].x = 0;
	que[head].y = 0;
	que[head].s = 0;
	que[head].f = 0;
	book[0][0] = 1;
	tail++;


	for(int i=0;i<5;i++)
		for(int j=0;j<5;j++)
			scanf("%d",&num[i][j]);

	while(head<tail)
	{
		for(int i=0;i<4;i++)
		{
			tx = que[head].x + next[i][0];
			ty = que[head].y + next[i][1];

			if(tx>4||ty>4||tx<0||ty<0)
				continue;
			
			if(num[tx][ty]==0&&book[tx][ty]==0)
			{
				book[tx][ty] = 1;
				que[tail].x = tx;
				que[tail].y = ty;
				que[tail].f = head;
				que[tail].s = que[head].s + 1;
				tail++;
			}

			if(tx==4 && ty==4)
			{
				flag = 1;
				break;
			}
		}
		if(flag==1)
			break;
		head++;
	}
	
	tail -=1;
	len = que[tail].s;
	for(int i=0;que[tail].f!=0;i++)
	{
		tmp[i] = tail;
		tail = que[tail].f;

	}
	printf("(0,0)\n");
	for( ;len>0;len--)
	{
		printf("(%d,%d)",que[tmp[len-1]].x,que[tmp[len-1]].y);
		if(len!=0)
			printf("\n");
	}










}
