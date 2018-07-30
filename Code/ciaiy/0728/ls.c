#include <pwd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define HAVE_l 1   
#define HAVE_S 2    
#define HAVE_a 4  
#define HAVE_i 8   
#define HAVE_t 16   
#define HAVE_U 32   
#define HAVE_s 64   
#define HAVE_R 128
#define NO_PATH     1
#define HAVE_PATH   0

typedef char QUENODE[256]; // 队列节点
QUENODE que[4096000];    // 队列
int rear;
int front;

void err(const char *err, int line);
void ls(const char *pathname, const int mode);
int show(const char *fileName, const int mode, const struct stat buf);
void initQue(const char *pathname);
void analysisArgv(int argc, char *argv[], int *mode, char *pathname);
char *gidToStr(gid_t gid);
char *uidToStr(uid_t uid);


void err(const char *err, int line) {
    fprintf(stderr, "line : %d\t", line);
    perror(err);
}

void ls(const char *pathname, const int mode) {
    char pathTemp[512]; // 创建一个数组 用于存放原路径和文件名
    struct stat buf; // 存放stat结构体信息
    DIR *dp; 
    struct dirent *dirp;
    int len;
    int charNum = 120;

    strcpy(pathTemp, pathname);     // 先将pathname存入数组中

    if(lstat(pathTemp, &buf) == -1) { // 如果lstat有错误 则输出 
        err("lstat error", __LINE__);
        fprintf(stderr, "%s 有问题\n", pathTemp);
        rear++;
        return;
    }

    if(S_ISDIR(buf.st_mode)) {  // 如果是目录

        if(pathTemp[strlen(pathTemp)- 1] != '/') { // 如果原路径最后一个字符不为/ 则加上
            pathTemp[strlen(pathTemp) + 1] = '\0';
            pathTemp[strlen(pathTemp)] = '/';
        }

        if((dp = opendir(pathTemp)) == NULL) {  // 打开路径
            err("opendir error", __LINE__);
            fprintf(stderr, "%s \n", pathTemp);            
            rear++;
            return ;
        }
    
        while((dirp = readdir(dp)) != NULL) { // 进行遍历
            char Temp[512] = {0};
            struct stat tempBuf;

            len = 0;

        if(!(mode & HAVE_a)) {  // 如果不是a的话, 则不输出隐藏文件
            if(dirp->d_name[0] == '.') {
                continue;
            }
        }

            strcpy(Temp, pathTemp);
            strcat(Temp, dirp->d_name); // 将路径和文件名连接在一起



            if(lstat(Temp, &tempBuf) == -1) {
                err("lstat error", __LINE__);
                putchar('\n');
            }
            
            if((tempBuf.st_mode & __S_IFDIR) && (mode & HAVE_R) && (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) ) { // 如果文件为目录的话 则入队列
                strcpy(que[front], Temp);
                front++;
            }

            len = show(dirp->d_name, mode, tempBuf); //统计输出的字符个数

            if(((charNum -= len) < len) || mode & HAVE_l) {  // 如果剩下的字符不够输出, 则换行
                printf("\n");
                charNum = 120;
            }

        }   
    
    closedir(dp); // 输出完该目录下的文件后, 关闭
    }else {
        show(pathname, mode, buf);
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    char pathname[256] = {0};
    int mode;
    analysisArgv(argc, argv, &mode, pathname);

    initQue(pathname);

    while(rear != front) {
        printf("%s\n", que[rear]);
        ls(que[rear], mode);
        printf("\n");
        rear++;
    }

    return 0;

}

int show(const char *fileName, const int mode, const struct stat buf) {
    int len = 0; // 每一个文件数据输出的字符个数
    
    if(!(mode & HAVE_a)) {  
        if(fileName[0] == '.') {
            return 0;
        }
    }

    if(mode & HAVE_i && !(mode & HAVE_R)) { // 如果有i  则输出inode
        len += printf("\033[;35m%-8d\033[0m", buf.st_ino);
    }
    if(mode & HAVE_s && !(mode & HAVE_R)) { // 如果有s 则输出数据块数
        len += printf("\033[;36m%-3d\033[0m", buf.st_blocks);
    }
    if(mode & HAVE_l && !(mode & HAVE_R)) { // 如果有l 则要输出好多数据)
    // 判断文件类型
        switch(buf.st_mode & S_IFMT) {
            case S_IFSOCK   :   printf("s");    break;
            case S_IFLNK    :   printf("l");    break;
            case S_IFREG    :   printf("-");    break;
            case S_IFBLK    :   printf("b");    break;
            case S_IFDIR    :   printf("d");    break;
            case S_IFCHR    :   printf("c");    break; 
            case S_IFIFO    :   printf("f");    break;
            default : printf("?");
        }
            // 判断权限
        buf.st_mode & S_IRUSR ? putchar('r') : putchar('-');
        buf.st_mode & S_IWUSR ? putchar('w') : putchar('-');
        buf.st_mode & S_IXUSR ? putchar('x') : putchar('-');
        buf.st_mode & S_IRGRP ? putchar('r') : putchar('-');
        buf.st_mode & S_IWGRP ? putchar('w') : putchar('-');
        buf.st_mode & S_IXGRP ? putchar('x') : putchar('-');
        buf.st_mode & S_IROTH ? putchar('r') : putchar('-');
        buf.st_mode & S_IWOTH ? putchar('w') : putchar('-');
        buf.st_mode & S_IXOTH ? putchar('x') : putchar('-');

        printf("\033[;34m%3d\033[0m", buf.st_nlink); // 输出硬链接数
        printf("\033[;35m%6s\033[0m", uidToStr(buf.st_uid)); // 输出user 
        printf("\033[;36m%6s\033[0m", gidToStr(buf.st_gid)); // 输出group
        printf(" %6d ", buf.st_size);   // 输出文件大小
        // 输出文件的ctime
        printf("%d ", buf.st_blksize);
        struct tm *localTime = localtime(&buf.st_ctime);  // 这里是将time_t 数据读取成struct tm的操作
        if(localTime == NULL) {
            err("err \n", __LINE__);
            exit(1);
        }
        printf("\033[;33m%2d月 %2d日\033[0m ", localTime->tm_mon + 1, localTime->tm_mday);
        printf("\033[;32m%02d:%02d\033[0m ", localTime->tm_hour, localTime->tm_min);       
    }

    len += printf("\033[;36m%-*s\033[0m", 30, fileName); // 输出文件名 不管什么类型 都要输出
    return len;
}

char *gidToStr(gid_t gid) {
    struct passwd *pw; // 创建passwd结构体指针
    char *str  =  (char *)malloc(32);
    setpwent(); // 将文件指针调整到首
    while((pw = getpwent()) != NULL) {
        if(gid == pw->pw_gid) { // 如果此gid 等于 要搜索的gid 则退出
            break;
        }
    }
    endpwent(); // 关闭文件

    strcpy(str, pw->pw_name);
    return str;
}

char *uidToStr(uid_t uid) {
    struct passwd *pw;
    char *str  =  (char *)malloc(32);
    setpwent();
    while((pw = getpwent()) != NULL) { // 匹配到 则退出
        if(uid == pw->pw_uid) {
            break;
        }
    }
    endpwent();
    strcpy(str, pw->pw_name);
    return str;
}

void initQue(const char *pathname) {
    rear = 0;
    front = 1;
    strcpy(que[rear], pathname);    
}

void analysisArgv(int argc, char *argv[], int *mode, char *pathname) {
    int fileflag = NO_PATH;

    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') { // 参数
            for(int index = 1; argv[i][index] != '\0'; index++) {   // 遍历参数字符串
                switch(argv[i][index]) {
                    case 'l' : *mode |= HAVE_l;  continue;
                    case 'S' : *mode |= HAVE_S;  continue;
                    case 'a' : *mode |= HAVE_a;  continue;
                    case 'i' : *mode |= HAVE_i;  continue;
                    case 't' : *mode |= HAVE_t;  continue;
                    case 'U' : *mode |= HAVE_U;  continue;
                    case 's' : *mode |= HAVE_s;  continue;
                    case 'R' : *mode |= HAVE_R;  continue;
                    default :
                        fprintf(stderr, "\nmode error\n");
                        exit(1);
                }
            }
        }else { // 路径
            if(fileflag) {
                strcpy(pathname, argv[i]);
                fileflag = HAVE_PATH;   // 将flag改为已有目录
            }else {
                printf("\n本ls只支持对一个路径进行操作\n");
            }
        }

    }
    
    if(fileflag) {  // 如果没有文件路径的话, 默认输出当前路径
        strcpy(pathname, "."); 
        return ;
    }
}