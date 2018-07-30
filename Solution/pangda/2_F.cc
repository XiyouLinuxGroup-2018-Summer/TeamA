#include <iostream>
#include <cstdio>
#include <cstring>
char g[110][110];
int m, n;

inline bool valid(int x, int y)
{
    if (x >= m || y >= n || x < 0 || y < 0) return false;
    return g[x][y] == '@';
}

void dfs(int x, int y)
{
    static int move[8][2] = { 0, 1, 0, -1, 1, 0, -1, 0, 1, 1, 1, -1, -1, 1, -1, -1 };
    g[x][y] = '*';
    for (int i = 0; i < 8; i++) {
        int newx = x + move[i][0], newy = y + move[i][1];
        if (valid(newx, newy)) {
            dfs(newx, newy);
        }
    }
}

int main(int argc, char *argv[])
{
    for ( ; std::cin >> m >> n, m && n; memset(g, 0, sizeof(g))) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                scanf(" %c", &g[i][j]);
            }
        }

        int res = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (g[i][j] == '@') {
                    dfs(i, j);
                    res++;
                }
            }
        }
        std::cout << res << std::endl;
    }
}