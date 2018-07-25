#include<stdio.h>
int main (void)
{
     int m,i = 0,k,j,n;
    char st[50];
    while(scanf("%d",&m) && m > 0)
    {
    for(k = 1;k<m;k++)
    {
        j = 0;
        scanf("%s",st);
        for(i = 0;st[i] != '\0';i++)
        ;
        n = i/2;
        i = i-1;
        while(n)
        {
            if(st[i] != st[j])
            {
              printf("no\n");
              break;
            }
              n--;
        }
        if(n <=0)
        printf("yes\n");
    }
    }
	return 0;
}
