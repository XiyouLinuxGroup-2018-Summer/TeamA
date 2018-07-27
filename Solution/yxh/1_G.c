#include<stdio.h>
int main(void)
{
	int num[101];
	int n,t;
	int tmp;
	while(1){
	scanf("%d%d",&n,&t);
	if(n==0&&t==0)
		break;
	for(int i=0;i<n;i++)
	{
		scanf("%d",&num[i]);
	}
	num[n] = t;
	for(int i=0;i<n+1;i++)
		for(int j=0;j<n-i;j++)
		{
			if(num[j]>num[j+1])
			{
				tmp = num[j];
				num[j] = num[j+1];
				num[j+1] = tmp;
			}
		}
	for(int i=0;i<n+1;i++)
	{
		printf("%d",num[i]);
		if(i==n)
		{
			printf("\n");
			continue;
		}
		printf(" ");
	}
	}
}
