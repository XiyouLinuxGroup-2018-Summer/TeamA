#include<stdio.h>
#include<string.h>
int convert(const char* num)
{
	int len = strlen(num);
	int sum=0;
	int t = 1;
	int t2 = 10;
	for(int i=0;i<len;i++)
	{
		for(int j=1;j<len-i;j++)
		{
			t *= t2;
		}

		sum+=((num[i]-48)*t);
		t = 1;
	}
	return sum;
}
int main()
{
	char str[10] = "12433";
	printf("%d\n",convert(str));
}

