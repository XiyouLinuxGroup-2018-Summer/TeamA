#include <iostream>
#include <cstring>
#include <algorithm>

int K[101][101];
int dp[101][101];
int N, M;

int search(int x = 0, int y = 0)
{
    static int dir[4][2] = { 0, 1, 0, -1, 1, 0, -1, 0 };

    if (dp[x][y] != 0) return dp[x][y];

    int res = 0;
    for (int z = 1; z <= M; z++) {
        for (int i = 0; i < 4; i++) {
            int newx = x + z * dir[i][0],
                newy = y + z * dir[i][1];
            if (newx < 0 || newx >= N || newy < 0 || newy >= N) continue;
            if (K[newx][newy] <= K[x][y]) continue;
            res = std::max(res, search(newx, newy));
        }
    }

    return dp[x][y] = K[x][y] + res;
}

int main(int argc, char *argv[])
{
    for (; std::cin >> N >> M, N != -1 && M != -1; memset(K, 0, sizeof(K)), memset(dp, 0, sizeof(dp))) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cin >> K[i][j];
            }
        }

        std::cout << search() << std::endl;
    }
    return 0;
}