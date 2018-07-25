#include<stdio.h>
int main (void)
{
	int m,n,i,k=0,a,b,temp,j,flag;
    char st[100],t[100];
    while(~scanf("%d %d",&m,&n) && m!=0 && n !=0)
    {
        getchar();
        for(i = 0;i<m*2;i++)
            st[i] = getchar();
        for(i = 0;i<m*2;i+=2)
        t[k++] = st[i] - '0';
        for(i = 0;i<k;i++)
        {
            if(t[i] < n)
            flag = i;
        }
        for( j = k-1;j>=flag;j--)
                t[j+1] = t[j];
                t[flag+1] = n;
        k++;
        for(i = 0;i<k;i++)
        printf("%d ",t[i]);
        printf("\n");

    }
	return 0;
}
