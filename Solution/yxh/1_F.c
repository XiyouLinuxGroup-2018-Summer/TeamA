#include<stdio.h>
int main(void)
{
	int n,flag=0,i=0;
	scanf("%d",&n);
	char str[100];
	while(n--)
	{
		scanf("%s",str);
		while(str[i]!='\0')
		{
			if(str[i]>='0'&&str[i]<='9')
				flag+=1;
			i++;
		}
		printf("%d\n",flag);
		flag=0;
		i=0;
	}
}
