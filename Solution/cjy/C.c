/*************************************************************************
	> File Name: C.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 11时42分17秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main(void)
{
    int c,i=0,j,te,m,n;
    int flag=1;   
    char *h;

    scanf("%d",&c);
    getchar();


    int num[500];

    char ch[500];


    while(i<c)
    {
        fgets(ch,500,stdin);
        te=strlen(ch);
        
        if(te==2)
            flag=0;
        

        ch[te-1]='\0';
        te--;
        

        m=0,n=te-1;
        
    
        while(flag)
        {
            if(ch[m]==ch[n])
            {
                m++;
                n--;
                if((m==n)||(m>n))
                {
                    num[i]=1;
                    break;
                }    
                
            }
            else
            {
                num[i]=0;
                break;
            }    
        }

        if(flag==0)
            num[i]=0;

        i++;
    }


    for(int y=0;y<c;y++)
    {
        if(num[y]==1)
            printf("yes\n");
        if(num[y]==0)
            printf("no\n");
    }

    return 0;
}
