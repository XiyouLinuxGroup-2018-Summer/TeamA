#include<stdio.h>
int main(void)
{
	char a[4],tmp;
	while(~scanf("%c%c%c",&a[0],&a[1],&a[2]))
	{
		for(int i=0;i<3;i++)
			for(int j=0;j<2;j++)
			{
				if(a[j]>a[j+1])
				{
					tmp = a[j];
					a[j] = a[j+1];
					a[j+1] = tmp;
				}
			}
		printf("%c %c %c\n",a[0],a[1],a[2]);
		getchar();
	}
}
