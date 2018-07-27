/*************************************************************************
	> File Name: J.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月27日 星期五 19时30分45秒
 ************************************************************************/

#include<stdio.h>

int suan(int la)
{
    int num[2000];
    int i=1;
    int j=1;
    int t=0;
    int sum=0;

    while(1)
    {
        if(j==la)
            break;
        if((la%j)==0)
        {
            num[t]=j;
            t++;
        }
        j++;
    }

    for(int h=0;h<t;h++)
    {
        sum=sum+num[h];        
    }

    return sum;
}



int main(void)
{
    int i=0,j,k,m,n,ch,a,b;
    
    scanf("%d",&ch);
    getchar();
    while(i<ch)
    {
        scanf("%d%d",&m,&n);
        if(m==0&&n==0)
        {
            printf("YES\n");
            i++;
            continue;
        }
        if(m==0||n==0)
        {
            printf("NO\n");
            i++;
            continue;
        }



        a=suan(m);
        b=suan(n);
        if(a==n&&b==m)
            printf("YES\n");
        else
            printf("NO\n");
        i++;
    }

    return 0;
}
