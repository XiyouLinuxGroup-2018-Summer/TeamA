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
	char ch[105];
	int ca;


	while (1)
	{
		int k = 0;

		
		while (1)
		{
            setbuf(stdin,NULL);
			ch[k] = getchar();
			if (ch[k]== '\n'|| ch[k]==EOF)
				break;
			k++;
		}
		setbuf(stdin, NULL);

		if (ch[k] == EOF)
			break;

		//te = strlen(ch);

		//ch[te - 1] = '\0';
		//te--;
        ch[k]='\0';


		for (int i = 0; i<k; i++)
		{
			if ((i != 0) && (ch[i - 1] == ' ') && (ch[i] != ' ')&&(ch[i]>'Z'))
				ch[i] = ch[i] - 32;
			if ((i == 0) && (ch[0]!= ' ')&&(ch[i]>'Z'))
				ch[i] = ch[i] - 32;
		}
		printf("%s\n",ch);
	}

}
