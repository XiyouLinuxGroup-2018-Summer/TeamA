#include<stdio.h>
int okemm(int n);
int main(void)
{
	int a,b;
	int t,flag=0;
	while(1)
	{
		scanf("%d%d",&a,&b);
		if(a==0&&b==0)
			break;
		for(int i=a;i<b+1;i++)
		{
			t = i*i+i+41;
			if(okemm(t)==-1)
			{
				flag = 1;
			}
		}
		if(flag==1)
			printf("Sorry\n");
		else
			printf("OK\n");
		flag = 0;
		
	}
}

int okemm(int n)
{
	int flag=0;
	for(int i=2;i<n/2;i++)
	{
		if(n==n/i*i)
		{
				flag = 1;
		}
	}
	if(flag==0)
		return 1;
	else
		return -1;
}
