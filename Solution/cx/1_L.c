#include<stdio.h>i
#include<string.h>
int main(void)
{
	char st[100];
	int i = 0;
	while(fgets(st,100,stdin))
	{
		st[0] -= 32;
		for(i = 1;st[i] != '\0';i++)
	         {
		            if(st[i-1] == ' ')
		            {
		                st[i] = st[i] -32;
		                continue;
               		    }
		  }
         puts(st);
	}
	return 0;
}
