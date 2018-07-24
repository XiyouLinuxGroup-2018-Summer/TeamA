#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[])
{
    for (int n, m; std::cin >> n >> m, n || m; ) {
        std::vector<int> nums(n);
        for (int i = 0; i < n; i++) {
            std::cin >> nums[i];
        }
        auto it = nums.begin();
        std::cout << (m < *it ? m : *it++);
        while (*it <= m) std::cout << ' ' << *it++;
        if (m >= nums.front()) std::cout << ' ' << m;
        while (it != nums.end()) std::cout << ' ' << *it++;
        std::cout << std::endl;
    }
    return 0;
}