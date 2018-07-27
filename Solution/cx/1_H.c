#include<stdio.h>
int main (void)
{
    int m,i,j;
    char st[50];
    scanf("%d",&m);
    for(i = 0;i< m;i++)
    {
        getchar();
        fgets(st,50,stdin);
        for(j = 0;st[j] != '\0';j++)
        {
            if(st[0] >= '0' && st[0] <= '9')
            {
                printf("no\n");
                break;
            }
             if(st[j] == ' ')
            {
                printf("no\n");
                break;
            }
            if (st[j] != ' ')
            continue;
        }
        if(st[j] == '\0')
        printf("yes\n");


    }	
	return 0;
}
