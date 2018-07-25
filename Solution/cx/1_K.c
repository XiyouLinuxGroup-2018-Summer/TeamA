#include<stdio.h>
int main (void)
{
int a,b,i,k,m;
while(~scanf("%d %d",&a,&b) && a != 0 || b != 0)
{
    for(i = a;i<b;i++)
    {
         m = i*i+i+41;
        for(k = 2;k<m;k++)
        {
            if(m % k == 0)
            break;
        }
    }
    if(k >= m)
         printf("OK\n");
         else
         printf("Sorry\n");
}
	return 0;
}
