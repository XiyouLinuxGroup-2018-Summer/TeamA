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
    double  num[1005][2];
    double u,j,k;

    scanf("%d",&c);
    getchar();

    while(i<c)
    {
        scanf("%c%d%d",&h,&m,&n);
        setbuf(stdin,NULL);
        if(h=='+')
            num[i][0]=m+n;
            num[i][1]=0;
        if(h=='-')
            num[i][0]=m-n;
            num[i][1]=0;
        if(h=='*')
            num[i][0]=m*n;
            num[i][1]=0;
        if(h=='/')
        {    
            j=(double)m;
            k=(double)n;
            num[i][0]=j/k;
            
            num[i][1]=1;    
        }
        i++;
    }

    for(int y=0;y<c;y++)
    {
    
        if(num[y][1]==0)
            printf("%.0lf\n",num[y][0]);
        else
        {  
            v=num[y][0];
            u=num[y][0]-(int)v;
            //printf("%lf\n",u);
            if(u==0.000000)
            {
                printf("%.0lf\n",num[y][0]);
            }
            else
                printf("%.2lf\n",num[y][0]);
        }
    }
    
    return 0;
}
