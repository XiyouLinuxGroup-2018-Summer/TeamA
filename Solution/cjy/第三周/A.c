/*************************************************************************
	> File Name: A.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月08日 星期三 20时13分05秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int num[100][100];

int main(void)
{
    int key=0;
    int c;
    scanf("%d",&c);

    while(key<c)
    {
        memset(num,0,sizeof(num));     
        int k;
        scanf("%d",&k);

        for(int i=1;i<=k;i++)
        {
            for(int j=0;j<i;j++)
            {
                scanf("%d",&num[i-1][j]);
            }
        }

    /*

    for(int i=k-1;i>=0;i--)
    {
        for(int j=0;j<=i;j++)
        {
            printf("%d",num[i][j])
        }
    }
    */


        for(int i=k-2;i>=0;i--)
        {
            for(int j=0;j<=i;j++)
            {
                int a;
                if(num[i+1][j]>num[i+1][j+1])
                {
                    a=num[i+1][j];
                }
                else
                    a=num[i+1][j+1];
                num[i][j]+=a;
            }
        }
    
        printf("%d\n",num[0][0]);
	    key++;
    }
    return 0;
}
