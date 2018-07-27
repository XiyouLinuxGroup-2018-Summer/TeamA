/*************************************************************************
	> File Name: S.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 22时26分20秒
 ************************************************************************/


#include<stdio.h>

#define PE 3.1415927

int main(void)
{
    int i=0;
    double num[50],te;
    double m=4,n=3;

    

    while(scanf("%lf",&te)!=EOF)
    {
        num[i]=(m/n)*3.1415927*te*te*te;
        printf("%.3lf\n",num[i]);
    }

    return 0;
}

