#include <iostream>
#include <string>
#include <numeric>

int main(int argc, char *argv[])
{
    int cas;
    std::cin >> cas;
    for (std::string str; cas--; ) {
        std::cin >> str;
        std::cout << (str == std::accumulate(str.rbegin(), str.rend(), std::string(),
            [] (std::string a, char b) { return a + b; }) ? "yes" : "no") << std::endl;
    }

    return 0;
}