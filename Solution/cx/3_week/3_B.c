#include<stdio.h>
#include<string.h>
int m,n;
int d[32780];
void dp(int m);
int main(void)
{
    while(~scanf("%d",&m))
    {
        memset(d,0,sizeof(d));
        d[0] = 1;
        dp(m);
    }
    return 0;
}
void dp(int m)
{
    for(int i = 1;i<=3;i++)
    {
        for(int j = i;j<=32768;j++)
           d[j] += d[j - i];
    }
    printf("%d\n",d[m]);
}