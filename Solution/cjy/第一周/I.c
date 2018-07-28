/*************************************************************************
	> File Name: I.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 11时43分41秒
 ************************************************************************/

#include<stdio.h>

int main(void)
{
    int flag,c,i=0,a,b,k;
    scanf("%d",&k);
    getchar();

    while(i<k)
    {
        flag=0;
        scanf("%d%d%d",&a,&b,&c);
        setbuf(stdin,NULL);

        if((a+b)<=c||(a+c)<=b||(b+c)<=a||(a-b)>=c||(a-c)>=b||(b-c)>=a||(b-a)>=c||(c-a)>=b||(c-b)>=a)
            flag=1;

        if(flag==1)
            printf("no\n");
        else
            printf("yes\n");

        i++;
    }

    
}
