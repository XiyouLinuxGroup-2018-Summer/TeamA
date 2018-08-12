#include<stdio.h>

int max(int a,int b)
{
	return a>b?a:b;
}


int main(void)
{
	int num[101] = {0};
	int tmp[101] = {0};
	int n,sum=0;
	while(1)
	{
		sum = 0;
		scanf("%d",&n);
		if(n==0)
			break;
		for(int i=0;i<n;i++)
		{
			scanf("%d",&num[i]);
			tmp[i] = num[i];
		}
		for(int i=1;i<n;i++)
		{
			for(int j=0;j<i;j++)
			{
				if(num[i]>num[j])
				{
					tmp[i] = max((num[i]+tmp[j]),tmp[i]);
				}
			}
			sum = max(sum,tmp[i]);
		}
		printf("%d\n",sum);
	}
}
