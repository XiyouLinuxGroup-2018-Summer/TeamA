#include <stdio.h>

typedef strcut point {
    int row;
    int cow;
    int num;
}

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
    int i = 0;
    for(; Que[i][0] != 4 && Que[i][1] != 4; i++) {
        Que[front] = Que[i][0];
        Que[front][1] = Que[i][1];
        if(i % 4 == 0) {
            
            if(Que[front][0] != 4) {
                if(map[Que[front][0] + 1][Que[front][1]] != 1)
                    Que[front][0] += 1;                                                                                                   
            }
        }
        if(i % 4 == 1) {
            if(Que[front][0] != 0) {
                if(map[Que[front][0] - 1][Que[front][1]] != 1)
                    Que[front][0] -= 1;
            }
        }
        if(i % 4 == 2) {
            if(Que[front][1] != 4) {
                if(map[Que[front][0]][Que[front][1] + 1] != 1)
                    Que[front][1] += 1;
            }
        }
        if(i % 4 == 3) {
            if(Que[front][1] != 4) {
                if(map[Que[front][0]][Que[front][1] - 1] != 1)
                    Que[front][1] -= 1;
            }
        }
    }

    printf("%d %d\n", Que[i], Que[i]);
    return 0;
}