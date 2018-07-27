#include<stdio.h>
int main (void)
{
    double a;
    double v = 0;
    while(~scanf("%lf",&a))
    {
        v = 4.0/3*pi*a*a*a;
        printf("%.3lf\n",v);
    }
    return 0;
}
