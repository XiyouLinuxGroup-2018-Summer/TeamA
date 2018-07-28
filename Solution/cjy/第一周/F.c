/*************************************************************************
	> File Name: F.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 09时56分04秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main(void)
{
    int c,i=0,num,te;
    char ch[500];


    scanf("%d",&c);
    getchar();    
 
    while(i<c)
    {
        num=0;

        fgets(ch,500,stdin);
        te=strlen(ch);
        
        ch[te-1]='\0';
        te--;
        for(int k=0;k<te;k++)
        {
            if((ch[k]>='0')&&(ch[k]<='9'))
            {
                num++;
            }
        }
        printf("%d\n",num);
        i++;
    }   
}
