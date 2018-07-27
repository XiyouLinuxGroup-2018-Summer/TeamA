#include<stdio.h>
int main(void)
{
	int n;
	float a,b;
	int s1=0,s2=0,t;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%f%f",&a,&b);
		for(int i=2;i<a;i++)
		{
			t =(int)(a/i);
			if(a/i-t==0)
			{		s1+=i;
			}
		}
		for(int i=2;i<b;i++)
		{
			t = (int)(b/i);
			if(b/i-t==0)

			{	
					s2+=i;
			}
		}
		if(s1+1==b&&s2+1==a)
			printf("YES\n");
		else
			printf("NO\n");
		s1 = 0;
		s2 = 0;

	}
}
