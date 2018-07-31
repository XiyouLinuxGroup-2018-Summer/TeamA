#include <stdio.h>

typedef struct point {
    int row;
    int cow;
    int num;
}point;

point Que[100000];

int front;



int main(void) {
    Que[0].row = 0;
    Que[0].cow = 0;
    int map[5][5] = {

        0, 1, 0, 0, 0,

        0, 1, 0, 1, 0,

        0, 0, 0, 0, 0,

        0, 1, 1, 1, 0,

        0, 0, 0, 1, 0,

    };
    int rear = 0;
    front = 1;
    for(; Que[rear].row != 4 && Que[rear].cow != 4; rear++) {
        printf("rear : %d front : %d que[rear] : %d %d \n", rear, front, Que[rear].row, Que[rear].cow);
    if(Que[rear].row + 1 > 0 && Que[rear].row +1 < 5)
        if(map[Que[rear].row + 1][Que[rear].cow] == 0) {
            printf("v\n");
            Que[front].row = Que[rear].row + 1;
            Que[front].cow = Que[rear].cow;
            Que[front].num = Que[rear].num + 1;
            front++;
        }

    if(Que[rear].row - 1 > 0 && Que[rear].row -1 < 5)
        if(map[Que[rear].row - 1][Que[rear].cow] != 1) {
            printf("^\n");
            Que[front].row = Que[rear].row - 1;
            Que[front].cow = Que[rear].cow;
            Que[front].num = Que[rear].num + 1;
            front++;
        }

    if(Que[rear].cow + 1 < 5 && Que[rear].cow >= 0)
        if(map[Que[rear].row][Que[rear].cow + 1] != 1) {
            printf(">\n");
            Que[front].row = Que[rear].row;
            Que[front].cow = Que[rear].cow + 1;
            Que[front].num = Que[rear].num + 1;
            front++;
        }

    if(Que[rear].cow - 1 < 5 && Que[rear].cow - 1 >= 0)
        if(map[Que[rear].row][Que[rear].cow - 1] != 1) {
            printf("<\n");
            Que[front].row = Que[rear].row;
            Que[front].cow = Que[rear].cow - 1;
            Que[front].num = Que[rear].num + 1;
            front++;
        }
            
    }

    printf("%d %d %d \n", Que[rear - 1].row, Que[rear - 1].cow , Que[rear - 1].num);
    return 0;
}