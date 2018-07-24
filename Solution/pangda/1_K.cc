#include <iostream>
#include <algorithm>

bool prime[3000] { false, false, false };

bool judge(int x, int y)
{
    for (int i = x; i <= y; i++) {
        if (!prime[i * i + i + 41])
            return false;
    } 
    return true;
}

int main(int argc, char *argv[])
{
    std::fill(prime + 2, prime + 3000, true);
    for (int i = 2; i < 3000; i++) {
        for (int j = i * 2; j < 3000; j += i) {
            prime[j] = false;
        }
    }

    for (int x, y; std::cin >> x >> y, x || y; ) {
        std::cout << (judge(x, y) ? "OK" : "Sorry") << std::endl;
    }
}