#include<stdio.h>

int max(int,int);

int main()
{
	int n,N;
	int tree[100][100];
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d",&N);
		for(int i=0;i<N;i++)
			for(int j=0;j<=i;j++)
				scanf("%d",&tree[i][j]);
		for(int i=0;i<N;i++)
			for(int j=0;j<=i;j++)
				tmp[i][j] = 0;

		for(int i=N-2;i>=0;i--)
			for(int j=0;j<=i;j++)
			{
				tree[i][j] += max(tree[i+1][j],tree[i+1][j+1]) ;
			}
		printf("%d\n",tree[0][0]);
	}


}


int max(int a,int b)
{
	return a>b?a:b;
}
