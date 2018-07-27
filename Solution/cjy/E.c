/*************************************************************************
	> File Name: E.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月27日 星期五 17时52分36秒
 ************************************************************************/

#include<stdio.h>

int main(void)
{
    int i,j,k,te,ce;

    while(scanf("%d%d",&i,&j)!=EOF)
    {
        te=1;
        if(j>i)
        {
            k=i;
            i=j;
            j=k;
        }

        while(1)
        {
            if(((ce=i*te)%j)==0)
                break;

            te++;
        }

        printf("%d\n",ce);
    }

}
