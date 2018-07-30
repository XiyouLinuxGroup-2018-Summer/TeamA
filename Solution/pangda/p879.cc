#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int profitableSchemes(int G, int P, vector<int>& group, vector<int>& profit) {
        constexpr int MOD = 1000000007;

        vector<vector<int> > dp(G + 1, vector<int>(P + 1));
        dp[0][0] = 1;

        for (int i = 0; i < group.size(); i++) {
            for (int j = G; j >= 0; j--) {
                for (int k = P; k >= 0; k--) {
                    if (dp[j][k] == 0 || j + group[i] > G) continue;
                    (dp[j + group[i]][min(k + profit[i], P)] += dp[j][k]) %= MOD;
                }
            }
        }

        int res = 0;
        for (int i = 0; i < dp.size(); i++) {
            (res += dp[i][P]) %= MOD;
        }

        return res;
    }
};