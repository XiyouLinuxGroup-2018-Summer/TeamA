#include<stdio.h>


int main(void)
{
	int num[101] = {0};
	int tmp[101] = {0};
	int n,sum=0;
	while(~scanf("%d",&n))
	{
		sum = 0;

		for(int i=0;i<n;i++){
			scanf("%d",&num[i]);
			tmp[i] = 1;
		}
		for(int i=1;i<n;i++)
		{
			for(int j=0;j<i;j++)
			{
				if(num[i]>num[j])
				{
					if(tmp[j]+1>tmp[i])
						tmp[i]++;
				}
			}
			sum = sum>tmp[i]?sum:tmp[i];
		}
		printf("%d\n",sum);
	}
}
