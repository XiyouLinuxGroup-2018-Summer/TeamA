#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>

int main(int argc, char *argv[])
{
    int cas;
    std::cin >> cas;

    for (std::string st; cas--; ) {
        std::cin >> st;
        std::cout << std::count_if(st.begin(), st.end(), 
            [] (const char a) { return std::isdigit(a); }) << std::endl;
    }

    return 0;
}