#include<stdio.h>
#include<string.h>
int main (void)
{
    char a,b,c,t;

    while(~scanf("%c%c%c",&a,&b,&c))
    {
        if(a>b)
        {
            t = a;
            a = b;
            b = t;
        }
        if(a > c)
        {
            t = a;
            a = c;
            c = t;
        }
         if (b>c)
        {
            t = b;
            b = c;
            c = t;
        }
        printf("%c %c %c\n",a,b,c);
        getchar();
    }
    return 0;
}
