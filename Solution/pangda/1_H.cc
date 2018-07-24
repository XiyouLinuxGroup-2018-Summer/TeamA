#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>

bool judge(std::string &st)
{
    if (!std::isalpha(*st.begin()) && *st.begin() != '_') return false;
    if (std::count_if(st.begin(), st.end(), [] (const char a) {
        return !(std::isalpha(a) || std::isdigit(a) || a == '_');}) > 0)
        return false; 
    return true;
}


int main(int argc, char *argv[])
{
    int cas;
    std::scanf("%d%*c", &cas);

    for (std::string st; cas--; ) {
        std::getline(std::cin, st);
        std::cout << (judge(st) ? "yes" : "no") << std::endl;
    }
}