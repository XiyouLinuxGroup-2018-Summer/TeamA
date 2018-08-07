#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

int dp[2018];
int K[2018], S[2018];

void tmfmtout(int t)
{
    int h = (t / 3600 + 8) % 24, m = (t / 60) % 60, s = t % 60;
    std::printf("%02d:%02d:%02d %s\n", h, m, s, (h >= 12) ? "pm" : "am");
}

int main(int argc, char *argv[])
{
    int C;
    std::cin >> C;

    for (int N; C--; ) {
        memset(dp, 0, sizeof(dp));
        std::cin >> N;
        for (int i = 1; i <= N; i++) {
            std::cin >> K[i];
        }

        for (int i = 2; i <= N; i++) {
            std::cin >> S[i];
        }

        dp[0] = 0;
        dp[1] = K[1];
        for (int i = 2; i <= N; i++) {
            dp[i] = std::min(dp[i - 1] + K[i], dp[i - 2] + S[i]);
        }
        
        tmfmtout(dp[N]);
    }

    return 0;
}