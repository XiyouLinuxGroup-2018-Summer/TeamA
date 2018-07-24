#include<stdio.h>
#define PI 3.1415927
int main(void)
{
	double r,s;
	while(~scanf("%lf",&r))
	{
		s = (4/3.0)*PI*r*r*r;
		printf("%.3lf\n",s);
	}
}
