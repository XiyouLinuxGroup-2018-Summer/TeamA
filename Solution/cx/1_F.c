#include<stdio.h>
int main (void)
{
	 int m,i;
   	 char st[15][100];
  	  scanf("%d",&m);
  	  for(i = 0;i<m;i++)
  	  {
  		      scanf("%s",st[i]);
  		      int count = 0;
  	      for(int j = 0;st[i][j] != '\0';j++)
  	          if(st[i][j] >= '0'  && st[i][j] <= '9')
  		          count++;
		      printf("%d\n",count);
  	  }
	return 0;
}
