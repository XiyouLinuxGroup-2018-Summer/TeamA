#include<stdio.h>
#include<string.h>
int main(void)
{
	char str[100];
	int n;
	int len,flag=0;
	scanf("%d",&n);
	getchar();
	while(n--)
	{
		gets(str);
		len = strlen(str);
		for(int i=0;i<len;i++)
		{
			if(str[i]==' ')
			{
				flag = 1;
				break;
			}
			if(((str[i]>='a'&&str[i]<='z')||(str[i]>='A'&&str[i]<='Z')||(str[i]>='0'&&str[i]<='9')||(str[i]=='_')))
			{
					;
			}
			else
			{
				flag = 1;
				break;
			}
			if(str[0]<='9'&&str[0]>='0')
			{
				flag = 1;
				break;
			}
		}
		if(flag==1)
			printf("no\n");
		else
		{
			printf("yes\n");
		}
		flag = 0;
	}
}
