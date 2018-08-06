#include<stdio.h>

int book[9] = {0};
int a,b,c;
char str[9][9];
int total = 0;

void dfs(int cu)
{
	if(c==b)
	{
		total++;
		return;
	}

	if(cu>=a)
		return;

	for(int i=0;i<a;i++)
		if(book[i]==0&&str[cu][i]=='#')
		{
			book[i] = 1;
			c++;
			dfs(cu+1);
			book[i] = 0;
			c--;
		}
	dfs(cu+1);
}


int main()
{
	while(1){
	scanf("%d %d",&a,&b);
	getchar();
	if(a==-1&&b==-1)
			break;
	for(int i=0;i<a;i++){
		for(int j=0;j<a;j++){
			scanf("%1s",&str[i][j]);
		}
		getchar();
	}
	for(int i=0;i<9;i++)
		book[i] = 0;

	dfs(total);
	printf("%d\n",total);
	total = 0;
	}
}
