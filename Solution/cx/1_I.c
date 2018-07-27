#include<stdio.h>
int main (void)
{
int m,a,b,c,i;
while(~scanf("%d",&m))
{
for(i = 0;i<m;i++)
{
scanf("%d %d %d",&a,&b,&c);
if(a > c+b && b > a+c && c> a+b && a<b -c && b < a-c&&c<a-b || a == b ||b ==c || a == c)
printf("YES\n");
else
printf("NO\n");
}
}
	return 0;
}
