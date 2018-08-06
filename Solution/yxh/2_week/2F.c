#include<stdio.h>


int flag = 0;
int m,n;
int book[101][101] = {0};
char str[101][101];
char ch;
int next[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{-1,-1},{1,-1},{-1,1}};

void dfs(int q,int p)
{
	int tx,ty;
	for(int i=0;i<8;i++)
	{
		tx = q + next[i][0];
		ty = p + next[i][1];


		if(tx<0||tx>=m||ty<0||ty>=n){
			if(i==7)
				return;
			continue;
		}


		if(str[tx][ty]=='@'&&book[tx][ty]==0)
		{
			book[tx][ty] = 1;
			dfs(tx,ty);
		}
	}

}



int main(void)
{
	while(1){
	scanf("%d%d",&m,&n);
	if(m==0&&n==0)
		break;
	getchar();
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++) {		
			scanf("%1s",&str[i][j]);
		}
		getchar();
	}


	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{
			if(str[i][j]=='@'&&book[i][j]==0)
			{
				book[i][j] = 1;
				dfs(i,j);
				flag++;
			}
		}
	
			printf("%d\n",flag);
			flag = 0;
			for(int i=0;i<100;i++)
				for(int j=0;j<100;j++)
					book[i][j] = 0;
	}
}
