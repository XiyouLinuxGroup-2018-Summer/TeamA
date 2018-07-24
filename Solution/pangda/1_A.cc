#include <iostream>
#include <algorithm>

int main(int argc, char *argv[])
{
    for (char ch[4]; std::scanf(" %s", ch) != EOF; ) {
        std::sort(ch, ch + 3);
        std::printf("%c %c %c\n", ch[0], ch[1], ch[2]);
    }

    return 0;
}