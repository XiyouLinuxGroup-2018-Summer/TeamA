#include<stdio.h>
#include<string.h>
int n,k,sum,m;;
char st[10][10];
int memory[10] =  {0};
void dfc(int a);
int main (void)
{
    int i;
    while(~scanf("%d %d",&n,&k))
    {
         sum = 0;
         m = 0;
        if(n == -1 && k == -1)
        break;
        for(i = 0;i< n;i++)
            scanf("%s",st[i]);
        dfc(0);
         printf("%d\n",sum);
    }
    return 0;
}
void dfc(int a)
{
    if(m == k)
    {
        sum++;
        return;
    }
    if(a >= n)
    return;
    for(int i = 0;i< n;i++)
    {
        if(st[a][i] == '#'  && memory[i] == 0)
        {
            memory[i] = 1;
            m++;
            dfc(a+1);
            memory[i] = 0;
            m--;
        }
    }
    dfc(a+1);
}