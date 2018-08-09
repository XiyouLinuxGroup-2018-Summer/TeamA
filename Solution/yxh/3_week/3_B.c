#include<stdio.h>
int b(int v);

int main(void)
{
	 int money;
	 int vari = 0;
	while(~scanf("%d",&money))
	{
		vari = 1 + money/2 + b(money);	
		printf("%d\n",vari);
		vari = 0;
	}
}

int b(int v)
{
	int t = 1,sum = 0;
	for(int j=1,i=3;i<=v;i+=3,j++)
	{
		t=((v-i)/2+1);
		sum+=(t);
	}
	return sum;
}
