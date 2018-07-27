/*************************************************************************
	> File Name: L.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 16时40分41秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main(void)
{
    int te;
    char ch[100];


    while(1)
    {

        fgets(ch,100,stdin);
        setbuf(stdin,NULL);
        if(ch[0]==EOF)
            break;

        te=strlen(ch);

        ch[te-1]='\0';
        te--;
        
        puts(ch);
        
        printf("%c",ch[1]-32);

        for(int i=0;i<te;i++)
        {
            if(i!=0&&(ch[i-1]==' ')&&(ch[i]!=' '))
                ch[i]=ch[i]-32;
            if((i==0)&&(i!=' '))
                ch[i]=ch[i]-32;
        }

        puts(ch);


    }

}
