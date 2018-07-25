#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {

    int abc;
    struct stat buf;
    stat("/home", &buf);
    abc = buf.st_mode & S_IFDIR;//与对应的标志位相与
    if(abc == S_IFDIR)          //结果与标志位比较
        printf("It's a directory.\n");
    return 0;

}