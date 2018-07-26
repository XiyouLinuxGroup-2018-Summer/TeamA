#include<stdio.h>
int main (void)
{
int m,n,i,j,flag;
    int st[110];
    while(~scanf("%d %d",&m,&n) && m!=0 && n !=0)
    {
        for(i = 0;i<m;i++)
        scanf("%d",&st[i]);
        for(i = 0;i<m;i++)
        {
            if(st[i] < n)
            flag = i;
        }
        for( j = m - 1;j>=flag;j--)
                st[j+1] = st[j];
                st[flag+1] = n;
        m++;
        for(i = 0;i<m;i++)
        printf("%d ",st[i]);
        printf("\n");

    }
return 0;
}
