#include <iostream>

int gcd(int a, int b)
{
    return (b > 0) ? gcd(b, a % b) : a;
}


int main(int argc, char *argv[])
{
    for (int a, b; std::cin >> a >> b; ) {
        std::cout << a * b / gcd(a, b) << std::endl;
    }

    return 0;
}