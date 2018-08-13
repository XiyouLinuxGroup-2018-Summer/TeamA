/*************************************************************************
	> File Name: ah.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月10日 星期五 19时56分43秒
 ************************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex1,mutex2,mutex3;
pthread_cond_t cond1,cond2,cond3;

void jian1(void)
{
    while(1)
    {
        pthread_mutex_lock(&mutex1);
        //pthread_cond_wait(&cond1,&mutex);
        printf("%c",'A');
        //pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mutex2);
    }
}

void jian2(void)
{
    while(1)
    {
        pthread_mutex_lock(&mutex2);
        //pthread_cond_wait(&cond2,&mutex);
        printf("%c",'B');
       // pthread_cond_signal(&cond3);
        pthread_mutex_unlock(&mutex3);
    }
}

void jian3(void)
{
    while(1)
    {
        pthread_mutex_lock(&mutex3);
        //pthread_cond_wait(&cond3,&mutex);
        printf("%c",'C');
        //pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&mutex1);
    }
}

int main(void)
{
    pthread_t tid1,tid2,tid3;
    int t;

    pthread_mutex_init(&mutex1,NULL);
	pthread_mutex_init(&mutex2,NULL);
	pthread_mutex_init(&mutex3,NULL);
  
	pthread_mutex_lock(&mutex2);
	pthread_mutex_lock(&mutex3);
	//pthread_mutex_lock(&mutex);
    pthread_create(&tid1,NULL,(void *)jian1,NULL);
    
    pthread_create(&tid2,NULL,(void *)jian2,NULL);

    pthread_create(&tid3,NULL,(void *)jian3,NULL);
    
    //pthread_cond_signal(&cond1);

    sleep(100);
    pthread_exit(0);

    return 0;
}

