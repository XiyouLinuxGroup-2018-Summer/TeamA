/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月07日 星期二 21时57分23秒
 ************************************************************************/

#include<stdio.h>

int main(void)
{
    int k;

    while(scanf("%d",&k)!=EOF)
    {

        int w[3]={1,2,3};           //对应每个物品的重量
    
        int ff[35000];              //方案数组

        for(int t=0;t<35000;t++)    
            ff[t]=0;

        ff[0]=1;

        for(int i=0;i<3;i++)
        {
            for(int j=w[i];j<=k;j++)
            {
                ff[j]=ff[j-w[i]]+ff[j];   
            }
        }

        printf("%d\n",ff[k]);
    }
        return 0; 
}



