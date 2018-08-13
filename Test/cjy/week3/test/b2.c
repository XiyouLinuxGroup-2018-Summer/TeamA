/*************************************************************************
	> File Name: b2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月10日 星期五 15时30分25秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex, mutex2;
pthread_cond_t cond1, cond2, cond3;

typedef struct
{
	char name[1000];
} fname;

int flag = 0, flag2 = 0;

void jian1()
{
	while (1)
	{
		fprintf(stderr, "lock mutex in jian1\n");
		pthread_mutex_lock(&mutex);
		fprintf(stderr, "unlock mutex in 1, waiting cond1\n");
		pthread_cond_wait(&cond1, &mutex);
		flag = 1;
		/*{
			FILE *fp = fopen("tmp", "a+");
			fseek(fp,0,SEEK_SET);
			int i=0;
			char c;
			char nani[100];
			//long int a=ftell(fp);
			printf("THREAD jian1\n");

			//c=getchar()!='\n'
			while(1)
			{	
				scanf("%c",&c);
				if(c=='\n')
					break;
				nani[i]=c;
				//printf("haha\n");
				i++;
			}
			printf("scanf over!\n");
			nani[i]='\0';
			printf("  %s\n",nani);
			int t=strlen(nani);

			fname caca;

			int h;
			for(h=0;h<t;h++)
			{
				caca.name[h]=nani[h];
			}
			caca.name[t]='\0';
		
			fwrite(&caca, sizeof(fname), 1, fp);
			
			fclose(fp);
		}*/
		printf("1\n");

		pthread_mutex_unlock(&mutex);
		while (flag2 == 0)
		{
			pthread_cond_signal(&cond2);
		}
	}
}

void jian2()
{
	while (1)
	{
		fprintf(stderr, "lock mutex in jian2\n");
		pthread_mutex_lock(&mutex);
		fprintf(stderr, "unlock mutex int 2, waiting cond2\n");
		pthread_cond_wait(&cond2, &mutex);
		flag2 = 1;
		/*{
			fname kaka;
			printf("jian2\n");
			FILE *fp = fopen("tmp", "a+");

			fread(&kaka, sizeof(fname), 1, fp);
			//fseek(fp, -((int)sizeof(fname)), SEEK_CUR);
			printf("%s\n",kaka.name);
			fclose(fp);
			printf("haha\n");
		}*/
		printf("2\n");

		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond1);
	}
}

int main(void)
{
	pthread_t tid1, tid2, tid3;

	//FILE *fp = fopen("tmp", "a+");
	//close(fp);
	pthread_cond_init(&cond1, NULL);
	pthread_cond_init(&cond2, NULL);
	pthread_mutex_init(&mutex, NULL);

	pthread_create(&tid1, NULL, (void *)jian1, NULL);
	pthread_create(&tid2, NULL, (void *)jian2, NULL);

	while (flag == 0)
	{
		pthread_cond_signal(&cond1);
	}

	sleep(1000);
	pthread_exit(0);

	return 0;
}
