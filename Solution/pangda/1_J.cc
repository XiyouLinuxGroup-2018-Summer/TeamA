#include <iostream>

int factor_sum(int n)
{
    int sum = 0;
    for (int i = n / 2; i >= 1; i--) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum;
}

int main(int argc, char *argv[])
{
    int cas;
    std::cin >> cas;

    for (int a, b; cas--; ) {
        std::cin >> a >> b;
        std::cout << (a == factor_sum(b) && b == factor_sum(a) ? "YES" : "NO") << std::endl;
    }
    
    return 0;
}