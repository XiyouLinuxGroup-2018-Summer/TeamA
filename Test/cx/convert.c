#include<stdio.h>
#include<string.h>
int main (int argc,char **argv)
{
	if(argc != 2)
	{
		printf("wrong\n");
		return(1);
	}
	int sum = 0;
	int count = 1,i;
	char *p = argv[1];
	while(*p != '\0')
	{
		count++;
		p++;
	}
	int n = 1;
	p--;
	for(i= count-1 ;i>0;i--)
	{
		*p = *p-'0';
		printf("%d\t",*p);
		sum = sum + *p*n;
		*p = *p -'0';
		n *= 10;
		p--;
	}
	printf("%d\n",sum);
	return 0;
}
