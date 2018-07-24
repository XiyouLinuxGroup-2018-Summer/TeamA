#include <iostream>
#include <algorithm>

int main(int argc, char *argv[])
{
    constexpr double PI = 3.1415927;
    for (double R; std::cin >> R; ) {
        std::printf("%.3lf\n", R * R * R * PI * 4 / 3);
    }

    return 0;
}