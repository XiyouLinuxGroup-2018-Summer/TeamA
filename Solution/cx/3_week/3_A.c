#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
int a[105][105];
int d[105][105];
int m,n;
int main (void)
{
    int i,j,c;
        scanf("%d",&m);
        while(m)
        {
            scanf("%d",&n);
            for(i = 1;i<= n;i++)
            {
                for(j = 1;j<=i;j++)
                    scanf("%d",&a[i][j]);
            }
            for(j=1;j<=n;j++)
            d[n][j] = a[n][j];
            for(i = n-1;i>=1;i--)
            {
                if(i == 0)
                break;
                for(j = 1;j<=i;j++)
                {
                    d[i][j] = a[i][j] + max(d[i+1][j],d[i+1][j+1]);
                    c = d[i][j];
                }
            }
            if(n == 1)
                printf("%d\n",d[n][j-1]);
            else
                printf("%d\n",c);
         m--;
    }
    return 0;
}

