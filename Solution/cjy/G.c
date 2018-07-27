/*************************************************************************
	> File Name: G.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 10时12分02秒
 ************************************************************************/

#include<stdio.h>

void sort(int num[],int n,int te)
{
    int i,j,k;

    i=n-1;

    while(i>0&&num[i]>te)
    {
        num[i+1]=num[i];
        i--;
    }
    
    num[i+1]=te;

 }        



int main(void)
{
    int num[150];
    int i,j,k,te,m;
    
    while(1)
    {
        scanf("%d%d",&m,&te);
        
        if((m==0)&&(te==0))
            break;

        for(i=0;i<m;i++)
        {
            scanf("%d",&num[i]);
        }

        sort(num,m,te);

        for(j=0;j<m+1;j++)
        {
            if(j!=m)
            printf("%d ",num[j]);
        else
            printf("%d\n",num[j]);
        }
    

    }

    return 0;
}
