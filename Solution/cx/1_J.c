#include<stdio.h>
int main (void)
{
	int m,a,b,i,sum1,sum2,k;
	scanf("%d",&m);
	for(i = 0;i<m;i++)
	{
	    sum1 = 0,sum2 = 0;
	    scanf("%d %d",&a,&b);
	    for(k = 1;k<a;k++)
	    {
	        if(a % k == 0)
	        sum1 += k;
	    }
	    if(sum1 == b)
	    {
	       for(k = 1;k<b;k++)
       		{
  		      if(b % k == 0)
  		        sum2 += k;
     	 	  }
     	  	 if(sum2 == a)
    	   	     printf("YES\n");
    	    else
		    printf("NO\n");
	    }
	    else
		    printf("NO\n");

	}
		return 0;
}
