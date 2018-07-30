// POJ不支持C++11
#include <iostream>
#include <cstring>
#include <queue>

struct path {
    int pos, d;
    path(int pos, int d): pos(pos), d(d) {}
};

int min_path(int src, int dst)
{
    bool *vis = new bool[100001];
    memset(vis, 0, 100001);
    std::queue<path> Q;
    Q.push(path(src, 0 ));

    for (; !Q.empty(); Q.pop()) {
        path &u = Q.front();
        if (u.pos == dst) {
            delete[] vis;
            return u.d;
        }
        if (u.pos < 0 || u.pos > 100000) continue;
        if (vis[u.pos]) continue;
        Q.push(path(u.pos * 2, u.d + 1));
        Q.push(path(u.pos + 1, u.d + 1));
        Q.push(path(u.pos - 1, u.d + 1));
        vis[u.pos] = true;
    }

    throw "Unexcepted end.";
}

int main(int argc, char *argv[])
{
    for (int n, k; std::cin >> n >> k; ) {
        std::cout << min_path(n, k) << std::endl;
    }
}