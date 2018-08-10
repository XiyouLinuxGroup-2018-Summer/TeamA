/*************************************************************************
	> File Name: c.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月08日 星期三 19时31分24秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main(void)
{
    int c;
    int visited[1000];
    int num[1000];//物品按顺序的价值
    int max[1000];
    
    while(1)
    {
        memset(num,0,1000);
        scanf("%d",&c);

        if(c==0)
            break;

        for(int i=0;i<c;i++)
            scanf("%d",&num[i]);

        //memset(visited,0,1000);
        //memset(max,0,1000);

        int tmp[1000];
        int tt=0;
        int caca;

        max[0]=num[0];
        
        for(int j=1;j<c;j++)
        {
            //max[j]=num[j];
            caca=0;
            for(int k=0;k<j;k++)
            {
                if(num[j]>num[k])
                {
                    if(caca<max[k])   //caca是上一个最大子序列和，max[k]是当前最大子序列和
                        caca=max[k];  //此时caca是第j个数前的最大子序列
                }    
            }
            max[j]=caca+num[j];       //最大子序列加上自己为新的最大子序列
        }

        int ha=max[0];
        
        
        for(int i=1;i<c;i++)
        {
            if(max[i]>ha)
                ha=max[i];
        }

        printf("%d\n",ha);
    }

    return 0;
}
