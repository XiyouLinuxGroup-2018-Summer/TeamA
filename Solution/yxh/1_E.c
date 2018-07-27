#include<stdio.h>
int main(void)
{
	char c;
	float a,b;
	int n;
	scanf("%d",&n);
	while(n--)
	{
		getchar();
		scanf("%c%f%f",&c,&a,&b);
		if(a==0||(c=='*'&&b==0))
		{
			printf("0\n");
			continue;
		}
		if(a/b<=0.009999)
		{
			printf("0\n");
			continue;
		}
		if(c=='+')
			printf("%.0f\n",a+b);
		if(c=='-')
			printf("%.0f\n",a-b);
		if(c=='*')
			printf("%.0f\n",a*b);
		if(c=='/')
		{	
			if((int)(a/b)==a/b)
				printf("%.0f\n",(a/b));
			else
				printf("%.2f\n",a/b);
		}
	}
}
