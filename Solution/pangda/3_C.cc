#include <iostream>
#include <cstring>
#include <algorithm>

int dp[1024];
int K[1024];

int main(int argc, char *argv[])
{
    for (int N; std::cin >> N, N; memset(dp, 0, sizeof(dp)), memset(K, 0, sizeof(K))) {
        for (int i = 1; i <= N; i++) {
            std::cin >> K[i];
        }

        dp[0] = 0;
        for (int i = 1; i <= N + 1; i++) {
            for (int j = 0; j < i; j++) {
                if (K[j] < K[i] || i == N + 1)
                    dp[i] = std::max(dp[j] + K[i], dp[i]);
            }
        }

        std::cout << dp[N + 1] << std::endl;
    }
}