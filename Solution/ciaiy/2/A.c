#include <stdio.h>

typedef struct point {
    int row;
    int cow;
    int num;
    int his[100];
}point;

point Que[100000] = {0};

int front;

int main(void) {
    Que[0].row = 0;
    Que[0].cow = 0;
    int map[5][5] = {0};
int now, front;
now = 0; 
front = 1;
Que[0].row = 0;
Que[0].cow = 0;
Que[0].his[0]= 0;
Que[0].num = 0;

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            scanf("%d", &(map[i][j]));
        }
    }

    for(;Que[now].cow != 4 || Que[now].row != 4; now++) {
        
        if(Que[now].row + 1 <= 4 && Que[now].row + 1 >= 0) {
            if(map[Que[now].row + 1][Que[now].cow] == 0) {
                Que[front].row = Que[now].row + 1;
                Que[front].cow = Que[now].cow;
                for(int i = 0; i <= Que[now].num; i++) {
                    Que[front].his[i] = Que[now].his[i];
                }
                Que[front].num = Que[now].num + 1;
                Que[front].his[Que[front].num] = now;
                map[Que[front].row][Que[front].cow] = -1;
                front++;
                if(Que[front].row == 4 && Que[front].cow == 4) {
                    break;
                }
            }
        }

        if(Que[now].row - 1 <= 4 && Que[now].row - 1>= 0) {
            if(map[Que[now].row - 1][Que[now].cow] == 0) {
                Que[front].row = Que[now].row - 1;
                Que[front].cow = Que[now].cow;
                for(int i = 0; i <= Que[now].num; i++) {
                    Que[front].his[i] = Que[now].his[i];
                }
                Que[front].num = Que[now].num + 1;
                Que[front].his[Que[front].num] = now;
                map[Que[front].row][Que[front].cow] = -1;
                front++;
                if(Que[front].row == 4 && Que[front].cow == 4) {
                    break;
                }
            }
        }


        if(Que[now].cow + 1 <= 4 && Que[now].cow + 1 >= 0) {
            if(map[Que[now].row][Que[now].cow + 1] == 0) {
                Que[front].row = Que[now].row;
                Que[front].cow = Que[now].cow + 1;
                for(int i = 0; i <= Que[now].num; i++) {
                    Que[front].his[i] = Que[now].his[i];
                }
                Que[front].num = Que[now].num + 1;
                                Que[front].his[Que[front].num] = now;
                map[Que[front].row][Que[front].cow] = -1;     
                front++;
                if(Que[front].row == 4 && Que[front].cow == 4) {
                    break;
                }
            }
        }


        if(Que[now].cow - 1 <= 4 && Que[now].cow - 1>= 0) {
            if(map[Que[now].row][Que[now].cow - 1] == 0) {
                Que[front].row = Que[now].row;
                Que[front].cow = Que[now].cow - 1;
                for(int i = 0; i <= Que[now].num; i++) {
                    Que[front].his[i] = Que[now].his[i];
                }
                Que[front].num = Que[now].num + 1;
                Que[front].his[Que[front].num] = now;
                map[Que[front].row][Que[front].cow] = -1;
                front++;
                if(Que[front].row == 4 && Que[front].cow == 4) {
                    break;
                }
            }
        }
    }

    front -= 1;
    for(int i = 1; i <= Que[front].num; i++) {
        int index =  Que[front].his[i];
        printf("(%d, %d)\n", Que[index].row,  Que[index].cow);
    }

    printf("(4, 4)\n");
        
    return 0;
}