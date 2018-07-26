#include<stdio.h>
int main(void)
{
	float a,b,c;
	int n;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%f%f%f",&a,&b,&c);
		if(a+b>c&&a+c>b&&b+c>a)
			printf("YES\n");
		else
			printf("NO\n");
	}
}
