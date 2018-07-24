#include <iostream>

int main(int argc, char *argv[])
{
    int cas;
    std::cin >> cas;

    for (double a, b, c; cas--; ) {
        std::cin >> a >> b >> c;
        std::cout << ((a + b > c && a + c > b && b + c > a) ? "YES" : "NO") << std::endl;
    }
    return 0;
}