#include <iostream>

int dp[32777];

int main(int argc, char *argv[])
{
    dp[0] = 1;
    for (int i = 1; i <= 3; i++) {
        for (int j = i; j <= 32768; j++) {
            dp[j] += dp[j - i];
        }
    }

    for (int N; std::cin >> N; ) {
        std::cout << dp[N] << std::endl;
    }

    return 0;
}