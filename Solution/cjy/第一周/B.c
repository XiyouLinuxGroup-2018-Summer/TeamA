/*************************************************************************
	> File Name: B.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 10时46分15秒
 ************************************************************************/

#include<stdio.h>

#define PE 3.1415927

int main(void)
{
    int i=0;
    float num[50],te;
    double m=4,n=3;

    

    while(scanf("%f",&te)!=EOF)
    {
        num[i]=(m/n)*3.1415927*te*te*te;
        i++;
    }

    for(int j=0;j<i;j++)
    {
        if(j!=i-1)
            printf("%.3f\n",num[j]);
        else
            printf("%.3f",num[j]);
    }

    return 0;
}

