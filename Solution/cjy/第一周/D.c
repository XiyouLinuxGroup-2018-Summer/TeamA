/*************************************************************************
	> File Name: D.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 16时38分31秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main(void)
{
    int c;
    int i=0,m,n,v;
    char h;
    float num;
    int hh;
    float u,j,k;

    scanf("%d",&c);
    getchar();

    while(i<c)
    {
        scanf("%c%d%d",&h,&m,&n);
        setbuf(stdin,NULL);
        if(h=='+')
        {
            hh=m+n;
            printf("%d\n",hh);
        }    
        if(h=='-')
        {
            hh=m-n;
            printf("%d\n",hh);
        }
        if(h=='*')
        {
            hh=m*n;
            printf("%d\n",hh);
        }
        if(h=='/')
        {   

            char uu[500],fb,gb,sh;

            j=(float)m;
            k=(float)n;

            num=j/k;
            
            sprintf(uu,"%f",num);
            int te=1;

            while(1)
            {
                if(uu[te]=='.')
                {
                    fb=uu[te+1];
                    if(uu[te+3]>='4')
                        gb=uu[te+2]+1;
                    else
                        gb=uu[te+2];
                    break;
                }
                te++;
            }
            printf("%d.%c%c\n",m/n,fb,gb);
        }
        i++;
    }
    
    return 0;
}
