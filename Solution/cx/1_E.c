#include<stdio.h>
int main (void)
{
	int m,a,b,i;
       char c;
    while(~scanf("%d",&m) && m > 0)
    {
    for(i = 0;i<m;i++)
    	{
       	 getchar();
       	 scanf("%c %d %d",&c,&a,&b);
       	 if(c == '+')
       	 printf("%d\n",a+b);
       	 if(c == '-')
       	 printf("%d\n",a-b);
       	 if(c == '*')
       	 printf("%d\n",a*b);
       	 if(c == '/')
	 {
		 if((a%b) != 0)
       	             printf("%.2lf\n",(double)a/b);
		 else
			  printf("%.0lf\n",(double)a/b);
	 }
    	}
      }
	return 0;
}
