#include <iostream>

using namespace std;

struct point {
    int x;
    int y;
};

bool isempty(char map[1000][1000], int x, int y, struct point & seed) {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            if(map[i][j] == '@') {
                seed.x = i;
                seed.y = j;
                return false;
            }
        }
    }
    return true;
}

void dfs(char map[1000][1000], struct point now, int x, int y);

void dfs(char map[1000][1000], struct point now, int x, int y) {
    if(map[now.x][now.y] == '@') {
        map[now.x][now.y] = '*';
    }

    if(now.x + 1 < x && map[now.x + 1][now.y] == '@') {
        struct point next = now;
        next.x = now.x + 1;
        dfs(map, next, x, y);
    }

   if(now.x - 1 >= 0 && map[now.x - 1][now.y] == '@') {
        struct point next = now;
        next.x = now.x - 1;
        dfs(map, next, x, y);
    }

    if(now.y + 1 < y && map[now.x][now.y + 1] == '@') {
        struct point next = now;
        next.y = now.y + 1;
        dfs(map, next, x, y);
    }

    if(now.y - 1 >= 0 && map[now.x][now.y - 1] == '@') {
        struct point next = now;
        next.y = now.y - 1;
        dfs(map, next, x, y);
    }

    if(now.x + 1 < x && now.y + 1 < y && map[now.x + 1][now.y + 1] == '@') {
        struct point next = now;
        next.y = now.y + 1;
        next.x = now.x + 1;
        dfs(map, next, x, y);
    }

        if(now.x - 1 >= 0 && now.y + 1 < y && map[now.x - 1][now.y + 1] == '@') {
        struct point next = now;
        next.y = now.y + 1;
        next.x = now.x - 1;
        dfs(map, next, x, y);
    }

        if(now.x + 1 < x && now.y - 1 >= 0 && map[now.x + 1][now.y - 1] == '@') {
        struct point next = now;
        next.y = now.y - 1;
        next.x = now.x + 1;
        dfs(map, next, x, y);
    }

        if(now.x - 1 >= 0 && now.y - 1 >= 0 && map[now.x - 1][now.y - 1] == '@') {
        struct point next = now;
        next.y = now.y - 1;
        next.x = now.x - 1;
        dfs(map, next, x, y);
    }

}

int main(void) {
    char map[1000][1000];
    int x, y;
    struct point seed;

    while(cin>>x>>y) {
        int num = 0;

        if(x == 0 && y == 0) {
            break;
        }

    // 赋值
        for(int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                cin>>map[i][j];
            }
        }

        for(num = 0; !isempty(map, x, y, seed); num++) {
            dfs(map, seed, x, y);
            // for(int i = 0; i < x; i++) {
            //     for(int j = 0; j < y; j++) {
            //         printf("%c", map[i][j]);
            //     }
            //     printf("\n");
            // }
        }
        cout<<num<<endl;
        }

}