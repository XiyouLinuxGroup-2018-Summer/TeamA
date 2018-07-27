#include<stdio.h>
int main(void)
{
	char str[101];
	while(gets(str)){
	for(int i=1;str[i]!='\0';i++)
	{
		if(str[0]<='z'&&str[0]>='a')
			str[0]-=32;
		if(str[i-1]==' ')
			if(str[i]<='z'&&str[i]>='a')
				str[i]-=32;
	}
	puts(str);
	}
}
