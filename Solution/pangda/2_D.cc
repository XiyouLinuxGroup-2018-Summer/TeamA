#include <iostream>

bool done = false;
long long res;
int n;

void binum(long long last = 1, int depth = 1)
{
    if (depth >= 20 || done) return;
    if (last % n == 0) {
        done = true;
        res = last;
    }

    binum(last * 10, depth + 1);
    binum(last * 10 + 1, depth + 1);
}

int main(int argc, char *argv[])
{
    for (; std::cin >> n, n; done = false) {
        binum();
        std::cout << res << std::endl;
    }
    return 0;    
}