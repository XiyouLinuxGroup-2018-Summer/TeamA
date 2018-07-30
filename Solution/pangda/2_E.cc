#include <iostream>
#include <cstdio>
#include <queue>
#include <algorithm>
#include <cstring>

bool vis[110][110][110];
struct situation {
    int s, n, m, d;
    situation(int s, int n, int m, int d): s(s), n(n), m(m), d(d) {}
};

int times(int S, int N, int M)
{
    if (S & 1) return -1;

    memset(vis, 0, sizeof(vis));
    std::queue<situation> Q;
    Q.push(situation(S, 0, 0, 0));

    for ( ; !Q.empty(); Q.pop()) {
        situation &u = Q.front();
        
        int judge = 0;
        if (u.s == S / 2) judge++;
        if (u.n == S / 2) judge++;
        if (u.m == S / 2) judge++;
        if (judge == 2) return u.d;

        if (vis[u.s][u.n][u.m]) continue;
        vis[u.s][u.n][u.m] = true;

        if (u.s > 0) {
            if (u.n < N) {
                int val = std::min(u.s, N - u.n);
                Q.push({ u.s - val, u.n + val, u.m, u.d + 1 });
            }
            if (u.m < M) {
                int val = std::min(u.s, M - u.m);
                Q.push({ u.s - val, u.n, u.m + val, u.d + 1 });
            }
        }

        if (u.n > 0) {
            if (u.s < S) {
                int val = std::min(u.n, S - u.s);
                Q.push({ u.s + val, u.n - val, u.m, u.d + 1 });
            }
            if (u.m < M) {
                int val = std::min(u.n, M - u.m);
                Q.push({ u.s, u.n - val, u.m + val, u.d + 1 });
            }
        }

        if (u.m > 0) {
            if (u.s < S) {
                int val = std::min(u.m, S - u.s);
                Q.push({ u.s + val, u.n, u.m - val, u.d + 1 });
            }
            if (u.n < N) {
                int val = std::min(u.m, N - u.n);
                Q.push({ u.s, u.n + val, u.m - val, u.d + 1 });
            }
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    for (int S, N, M; std::cin >> S >> N >> M, S && N && M; ) {
        int res = times(S, N, M);
        if (res == -1) std::cout << "NO" << std::endl;
        else std::cout << res << std::endl;
    }

    return 0;
}