#include<stdio.h>
#include<string.h>
int main(void)
{
	char str[100];
	int len,n,flag;
	scanf("%d",&n);
	while(n--)
	{
		flag = 0;
		scanf("%s",str);
		len = strlen(str);
		for(int i=0,j=len-1;i<len/2;i++,j--)
		{
			if(str[i]!=str[j])
				flag = 1;
		}
		if(flag==0)
			printf("yes\n");
		else
			printf("no\n");
	}
}
