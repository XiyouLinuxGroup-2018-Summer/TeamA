#include <stdio.h>

typedef struct point {
    int num;
    int index;
}PNODE;

int main(void) {
    int x, y;
    while(scanf("%d%d", &x, &y) != EOF) {
        int load[1000000] = {0};
        PNODE que[100000] = {0};
        int now = 0;
        que[0].index = x;
        que[0].num = 0;
        int front = 1;
        while(que[now].index != y) {

            if(load[que[now].index - 1] == 0 && que[now].index - 1 >= 0) {
                //printf("<");
                que[front].index = que[now].index - 1;
                load[que[front].index] = 1;
                que[front].num = que[now].num + 1;
                //printf("index : %d num : %d\n", que[front].index, que[front].num);
                front++;
            }
            if(load[que[now].index + 1] == 0 && que[now].index + 1 <= 1000000) {
               // printf(">");
                que[front].index = que[now].index + 1;
                
                load[que[front].index] = 1;            
                que[front].num = que[now].num + 1;
                //printf("index : %d num : %d\n", que[front].index, que[front].num);
                front++;
            }
            if(load[que[now].index * 2] == 0 && que[now].index * 2 <= 1000000) {
                //printf("*");
                que[front].index = que[now].index * 2;
                load[que[front].index] = 1;                        
                que[front].num = que[now].num + 1;
                //printf("index : %d num : %d\n", que[front].index, que[front].num);
                front++;
            }
            now++;
        }
        printf("%d\n", que[now].num);
    }
return 0;
}