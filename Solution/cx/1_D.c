#include<stdio.h>
int main (void)
{
	int a,b,r;
        while(~scanf("%d %d",&a,&b) && a> 0 && b> 0)
   	 {
        if(a<b)
        {
            r = a;
            a = b;
            b = r;
        }
        int m = a*b;
        r = a%b;
        while(r)
        {
            a = b;
            b = r;
            r = a%b;
       	 }
       	 printf("%d\n",m/b);
   	 }
	return 0;
}
