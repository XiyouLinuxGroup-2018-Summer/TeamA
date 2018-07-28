/*************************************************************************
	> File Name: A.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 07时30分16秒
 ************************************************************************/

#include<stdio.h>

int main(void)
{
    int num[50][3];

    int i=0,j,k,m,n,ch,t;

    while(1)
    {
        j=0;
        while((ch=getchar())!='\n')
        {
            if(ch==EOF)
                break;
            num[i][j]=ch;
            j++;

        }
        if(ch==EOF)
            break;
        else
            i++;     
    }
    

    for(m=0;m<i;m++)
    {
        for(n=0;n<3;n++)
        {
            for(t=n+1;t<3;t++)
            {
                if(num[m][n]>num[m][t])
                {
                    k=num[m][n];
                    num[m][n]=num[m][t];
                    num[m][t]=k;
                }
            }
        }
    }

    for(int y=0;y<i;y++)
    {
        for(int u=0;u<3;u++)
        {
            if(u!=2)
                printf("%c ",num[y][u]);
            else
                printf("%c\n",num[y][u]);
        }
    }


    return 0;
}
