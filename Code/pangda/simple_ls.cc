#include <iostream>
#include <filesystem>

namespace sfs = std::filesystem;

int main(int argc, char *argv[])
{
    sfs::directory_iterator iter { { sfs::current_path() } };
    
    for (auto &i: iter) {
        std::cout << sfs::path(i).filename().c_str() << "  ";
    }

    std::cout << std::endl;
    
    return 0;
}