#include <iostream>
#include <cstdio>
#include <cstring>

const char INVALID = '.';
const char VALID = '#';

char g[10][10];
bool vis[10];
int n, k, val = 0;

void dfs(int row, int d)
{
    if (d == k) {
        val++;
        return;
    }

    if (row >= n) return;

    for (int i = 0; i < n; i++) {
        if (g[row][i] == VALID && !vis[i]) {
            vis[i] = true;
            dfs(row + 1, d + 1);
            vis[i] = false;
        }
    }
    dfs(row + 1, d);
}


int main(int argc, char *argv[])
{
    for (; std::cin >> n >> k, n != -1 && k != -1; val = 0, memset(vis, 0, sizeof(vis))) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::scanf(" %c", &g[i][j]);
            }
        }

        dfs(0, 0);
        std::cout << val << std::endl;
    }
}
