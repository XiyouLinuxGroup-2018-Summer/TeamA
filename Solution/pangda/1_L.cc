#include <iostream>
#include <string>
#include <cctype>

int main(int argc, char *argv[])
{
    for (std::string st; std::getline(std::cin, st); ) {
        bool f = true;
        for (auto &i: st) {
            if (f) {
                std::cout << char(toupper(i));
                f = false;
            } else {
                std::cout << i;
            }
            if (i == ' ') f = true;
        }
        std::cout << std::endl;
    }
}