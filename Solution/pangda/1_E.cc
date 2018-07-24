#include<iostream>

int main(int argc, char *argv[])
{
    int cas;
    std::cin >> cas;
    char op;
    for (int a, b; cas--; ) {
        std::cin >> op >> a >> b;
        switch (op)
        {
        case '+':
            std::cout << a + b << std::endl;
            break;
        case '-':
            std::cout << a - b << std::endl;
            break;
        case '*':
            std::cout << a * b << std::endl;
            break;
        case '/':
            if (a % b == 0) 
                std::cout << a / b << std::endl;
            else 
                std::printf("%.2lf\n", double(a) / b);
            break;
        }
    }
    return 0;
}