// 熟悉ls原理

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    struct stat buf;

    if((dp = opendir("/root")) == NULL) {
        perror("error");
        exit(1);
    }

    chdir("/root");
    
    while((entry = readdir(dp)) != NULL) {
        if(stat(entry->d_name, &buf) == -1) {
            perror("error");
        }
        printf("%-15s\t ino: %d\t", entry->d_name, buf.st_ino);
        printf("\n");
    }

    closedir(dp);
    return 0;
}
