#include<stdio.h>
int main (void)
{
    int arr[1200];
    int a[1200];
    int n,temp, max;
    while(~scanf("%d",&n)   && n != 0)
    {
        for(int i = 0;i< n;i++)
        {
            scanf("%d",&a[i]);
        }
        arr[0] = a[0];
        for(int i = 0;i< n;i++)
        {
            temp = 0;
            for(int j = 0;j<i;j++)
            {
                if(a[j] < a[i])
                {
                    if(temp < a[j])
                    temp = arr[j];
                }
            }
            arr[i] = temp+a[i];
        }
        max = -1;
        for(int i = 0;i< n;i++)
        {
            if(max < arr[i])
              max = arr[i];
        }
        printf("%d\n",max);
    }
    return 0;
}