/*************************************************************************
	> File Name: H.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 10时57分45秒
 ************************************************************************/

#include<stdio.h>

#include<stdio.h>
#include<string.h>

int main(void)
{
    int c,i=0,num,te,flag;
    char ch[60];


    scanf("%d",&c);
    getchar();

    while(i<c)
    {
        flag=0;
        num=0;

        fgets(ch,60,stdin);
        te=strlen(ch);

        ch[te-1]='\0';
        te--;


        if((ch[0]>='a'&&ch[0]<='z')||(ch[0]>='A'&&ch[0]<='Z')||ch[0]=='_')
            flag=0;
	    else
	        flag=1;


        for(int k=0;(k<te)&&(flag==0);k++)
        {
            if((ch[k]>='a'&&ch[k]<='z')||(ch[k]>='A'&&ch[k]<='Z')||ch[k]=='_'||(ch[k]>='0'&&ch[k]<='9'))
            {
            }
	        else
	            flag=1;
		
        }
        
        if(flag==1)
            printf("no\n");
        else
            printf("yes\n");

        i++;
    }
}
