#include <iostream>
#include <algorithm>
int dp[101][101];

int main(int argc, char *argv[])
{
    int C;
    std::cin >> C;
    for (int N; C--; ) {
        std::cin >> N;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j <= i; j++) {
                std::cin >> dp[i][j];
            }
        }

        for (int i = N - 2; i >= 0; i--) {
            for (int j = 0; j <= i; j++) {
                dp[i][j] += std::max(dp[i + 1][j], dp[i + 1][j + 1]);
            }
        }

        std::cout << dp[0][0] << std::endl;
    }
}
