#include<stdio.h>
struct rode{
	int x;
	int s;
};
int main()
{
	struct rode roade[100000];
	int start,end;
	int book[100000] = {0};
	int head = 0;
	int tail = 0;
	int tx;
	int flag2 = 0;
	int next[2] = {1,-1};

	while(~scanf("%d%d",&start,&end)){

			if(start==end){
				printf("0\n");
				continue;
			}
			for(int i=0;i<100000;i++)
			{
				book[i] = 0;
				roade[i].s = 0;
			}
			flag2 = 0;
			tail = 0;
			head = 0;

	roade[head].x = start;
	roade[head].s = 0;
	tail++;	
	book[start] = 1;

	while(head<tail)
	{
		for(int i=0;i<2;i++)
		{
			tx = roade[head].x + next[i];

			if(tx<0||tx>100000)
				continue;
			if(book[tx]!=1)
			{
				roade[tail].x = tx;
				roade[tail].s = roade[head].s+1;
				book[tx] = 1;
				tail++;
			}
			if(roade[tail-1].x == end)
				flag2 = 1;
		}
		if(flag2==1)
			break;

		tx = roade[head].x *2;

		if(tx<=100000)
		{
			roade[tail].x = tx;
			roade[tail].s = roade[head].s+1;
			book[tx] = 1;
			tail++;
		}

		if(roade[tail-1].x == end)
				break;
		head++;
	}
	printf("%d\n",roade[tail-1].s);

}
}
