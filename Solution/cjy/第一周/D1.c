/*************************************************************************
	> File Name: D1.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 22时55分48秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        char log;
        float a,b;
        scanf("%c %f %f",&log,&a,&b);
        switch(log)
        {
            case'+':printf("%d\n",(int)(a+b));break;
            case'-':printf("%d\n",(int)(a-b));break;
            case'*':printf("%d\n",(int)(a*b));break;
            case'/':printf("%.2f\n",a/b);break;
        }
    }
    return 0;
}
