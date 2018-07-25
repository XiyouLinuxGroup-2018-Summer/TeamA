 // 遍历路径下的文件, 不跟随链接文件
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH_MAX 4096 

#define FTW_F // 一般文件
#define FTW_D // 目录
#define FTW_DNR // 不可读取的 目录，此 目录以下将不被遍历
#define FTW_SL // 符号连接
#define FTW_NS // 无法取得stat结构数据，有可能是 权限问题

static char *pathName;   // 指向一个数组栈, 里面存放着目录 
static int pathLen; // 存放数组栈的长度

/* 存放着7种类型文件数量的数组 */
static int sum[7];

#define REG     0
#define DIRE    1
#define BLK     2
#define CHR     3
#define FIFO    4 
#define LINK    5
#define SOCK    6

int infoFunc(struct stat buf);    // 将每个文件在此统计
int traverse(int stackIndex); // 遍历函数

int infoFunc(struct stat buf) {
    switch(buf.st_mode & S_IFMT) {
        case S_IFSOCK   :   return SOCK;
        case S_IFLNK    :   return LINK;
        case S_IFREG    :   return REG;
        case S_IFBLK    :   return BLK;
        case S_IFDIR    :   return DIRE;
        case S_IFCHR    :   return CHR;
        case S_IFIFO    :   return FIFO;
        default :   
            fprintf(stderr, "file : %s infoFunc error unknown mode\n", pathName);
    }    
}

int traverse(int stackIndex) {
    DIR *dirp;
    struct dirent *dp;
    struct stat buf;

    if((dirp = opendir(pathName)) == NULL) {
        perror("opendir error");
        exit(1);
    }

    if(pathName[stackIndex -1] != '/') {
        pathName[stackIndex++] = '/';
        pathName[stackIndex] =  '\0';
    }
    while((dp = readdir(dirp)) != NULL) {
        struct stat buf;
        int len = strlen(pathName) + strlen(dp->d_name);
        int type;

        if(len > pathLen) {    // 如果数组不够存放路径, 则扩容
            pathLen *= 2;
            if(realloc(pathName, pathLen) == NULL) {
                perror("realloc error");
                exit(1);
            }
        }
        if(strcmp(dp->d_name, ".") == 0
            || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        pathName[stackIndex - 1] = '/';    // 给文件名字后加/
        pathName[stackIndex] = '\0';
        strcat(pathName, dp->d_name);   // 连接pathName 和 文件名字

        if(lstat(pathName, &buf) == -1) {  // 获取该文件的stat信息
            printf("lstat : %s", pathName);
            perror("error ");
        exit(1);
        }
        type = infoFunc(buf);        
        printf("%-10s %3d\n", dp->d_name, type);// 输出文件名
        if(type == DIRE) {   // 如果类别为目录, 则递归
            traverse(len);
        }
        sum[type]++;    //　若是其他类别, 则该元素下的值累加
        pathName[stackIndex] = '\0';    // 将数组的下标又回到刚开始
    }
    closedir(dirp); // 关闭dirp指针
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "error!\n./traverse <PATH>\n");
        exit(1);
    }
    
    if((pathName = (char *)malloc(sizeof(char) * PATH_MAX)) == NULL) {
        perror("malloc error");
        exit(1);
    }
    memset(pathName, 0, sizeof(char) * PATH_MAX);
    pathLen = PATH_MAX;
    strcpy(pathName, argv[1]);
    traverse(strlen(pathName));
    return 0;
}
