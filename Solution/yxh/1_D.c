#include<stdio.h>
int main(void)
{
	int a,b;
	int t,t2;
	float d,e;
	while(~scanf("%d%d",&a,&b))
	{
		d = a<b?a:b;
		e = a>b?a:b;
		t = (int)(e/d);
		if(e/d-t==0)
		{
			printf("%.0f\n",e);
			continue;
		}
		t2=d;
		while(1)
		{
			d+=t2;
			t = (int)(d/e);
			if(d/e-t==0)
			{
				printf("%.0f\n",d);
				break;
			}
		}
	}

}
