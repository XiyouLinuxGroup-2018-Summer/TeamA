/*************************************************************************
	> File Name: A.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月28日 星期六 16时12分18秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct tag_unimportant {
    char *name;
    int num;
} A;

void func(A **a)
{
    *a = (A *)malloc(sizeof(A));
    (*a)->name=malloc(strlen("Xiyou Linux Group")*sizeof(char));
    strcpy((*a)->name, "Xiyou Linux Group");
    (*a)->num = 20180728;
}

int main(int argc, char *argv[])
{
    A *a;
    func(&a);
    printf("%s %d\n", a->name, a->num);
    return 0;
}
