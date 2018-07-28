/*************************************************************************
	> File Name: K.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月27日 星期五 20时03分08秒
 ************************************************************************/

#include<stdio.h>

int main(void)
{
    int i,j,k,m,a,b,ao,flag=1;

    while(1)
    {
        scanf("%d%d",&a,&b);
        if(a==0&&b==0)
            break;


        for(i=a;i<=b;i++)
        {
            //判断是不是素数
            ao=i*i+i+41;

            
            for(m=2;m<ao;m++)
            {
                if((ao%m)==0)
                {
                    flag=0;
                    break;
                }       
            }
            
        }

        if(flag!=0)
            printf("OK\n");
        else
            printf("Sorry\n");

        flag=1;
    }
    return 0;
}
