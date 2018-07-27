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
#define NO_PATH     1
#define HAVE_PATH   0
#define LINE_MAX    120

typedef struct fileListNode {
    char name[256]; // 文件名最多只有255个字符
    struct stat buf;    // 存放文件的stat结构体
}NODE, *PNODE;

typedef struct ctrlNode{
    char *pathname; // 存放的是路径名
    int mode;   // 命令的模式
    int MAX_LIST;  // 最初的数组最大长度
    int MAX_PATH; // 最初的路径最大长度
    PNODE PFList;   // 存放文件数据的指针
    int fileNameMax;    // 所有文件中最长的数值
    int FLIndex;    // 文件数据的个数
    int sumBlock;   // 总用量
}CTRLNODE, *PCTRLNODE;

void analysisArgv(int argc, char *argv[], int *mode, char *pathname); 
int isDir(char *pathname);
void getList(char *pathname, int *MAX_PATH, int *FLIndex,  
    PNODE PFList, int mode, int *MAX_LIST); 
void sort(int mode, PNODE PFList, int FLIndex);
int sort_cmp_t(NODE A, NODE B);
int sort_cmp_d(NODE A, NODE B);
int sort_cmp_S(NODE A, NODE B);
int otherWork(int FLIndex, PNODE PFList, int *sumBlock);
void show(int FLIndex, int mode, PNODE PFList, int fileNameMax, int sumBlock);
char *uidToStr(uid_t uid);
char *gidToStr(gid_t gid);
void init(PCTRLNODE pcnode);

char *gidToStr(gid_t gid) {
    struct passwd *pw; // 创建passwd结构体指针

    setpwent(); // 将文件指针调整到首
    while((pw = getpwent()) != NULL) {
        if(gid == pw->pw_gid) { // 如果此gid 等于 要搜索的gid 则退出
            break;
        }
    }
    endpwent(); // 关闭文件

    return pw->pw_name; // 返回字符串
}

char *uidToStr(uid_t uid) {
    struct passwd *pw;

    setpwent();
    while((pw = getpwent()) != NULL) { // 匹配到 则退出
        if(uid == pw->pw_uid) {
            break;
        }
    }
    endpwent();

    return pw->pw_name;
}

void show(int FLIndex, int mode, PNODE PFList, int fileNameMax, int sumBlock) {
    int len; // 每一个文件数据输出的字符个数
    int charnum = 0; // 一行输出的字符个数

    if(mode & HAVE_l) { // 如果有l的话  则要输出总用量 (在otherWork()里统计过)
        printf("总用量 %d\n", sumBlock);
    }

    for(int i = 0; i < FLIndex; i++, len = 0) {
        if(mode & HAVE_i) { // 如果有i  则输出inode
            len += printf("\033[;35m%-8ld\033[0m", PFList[i].buf.st_ino);
        }
        if(mode & HAVE_s) { // 如果有s 则输出数据块数
            len += printf("\033[;36m%-3ld\033[0m", PFList[i].buf.st_blocks);
        }
        if(mode & HAVE_l) { // 如果有l 则要输出好多数据
            // 判断文件类型
            switch(PFList[i].buf.st_mode & S_IFMT) {
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
            PFList[i].buf.st_mode & S_IRUSR ? putchar('r') : putchar('-');
            PFList[i].buf.st_mode & S_IWUSR ? putchar('w') : putchar('-');
            PFList[i].buf.st_mode & S_IXUSR ? putchar('x') : putchar('-');
            PFList[i].buf.st_mode & S_IRGRP ? putchar('r') : putchar('-');
            PFList[i].buf.st_mode & S_IWGRP ? putchar('w') : putchar('-');
            PFList[i].buf.st_mode & S_IXGRP ? putchar('x') : putchar('-');
            PFList[i].buf.st_mode & S_IROTH ? putchar('r') : putchar('-');
            PFList[i].buf.st_mode & S_IWOTH ? putchar('w') : putchar('-');
            PFList[i].buf.st_mode & S_IXOTH ? putchar('x') : putchar('-');

            printf("\033[;34m%3ld\033[0m", PFList[i].buf.st_nlink); // 输出硬链接数
            printf("\033[;35m%6s\033[0m", uidToStr(PFList[i].buf.st_uid)); // 输出user 
            printf("\033[;36m%6s\033[0m", gidToStr(PFList[i].buf.st_gid)); // 输出group
            printf("%6ld", PFList[i].buf.st_size);   // 输出文件大小
            // 输出文件的ctime
            struct tm *localTime = localtime(&PFList[i].buf.st_ctime);  // 这里是将time_t 数据读取成struct tm的操作
            printf("\033[;33m%2d月 %2d日\033[0m ", localTime->tm_mon + 1, localTime->tm_mday);
            printf("\033[;32m%02d:%02d\033[0m ", localTime->tm_hour, localTime->tm_min);       
        }

        len += printf("\033[;36m%-*s\033[0m", fileNameMax + 1, PFList[i].name); // 输出文件名 不管什么类型 都要输出
        if(mode & HAVE_l) { // 如果参数有l 则忽略掉下面的操作
            putchar('\n');
            continue;
        }

        charnum += len; // 输出一个文件  查看一次这一行输出了多少字符
        if(LINE_MAX - charnum <= len){ // 如果剩下的字符过少 则直接换行
            printf("\n");
            charnum = 0;
        }
    }
        putchar('\n'); // 最后一行的换行
}

int otherWork(int FLIndex, PNODE PFList, int *sumBlock) {
    int max = strlen(PFList[0].name);

    for(int i = 0; i < FLIndex; i++) {
        *sumBlock += PFList[i].buf.st_blocks; // 统计文件的数据块数

        if(max < strlen(PFList[i].name)) { // 得到最大的文件名数值
            max = strlen(PFList[i].name); 
        }
    }

    return max;
} 

int sort_cmp_S(NODE A, NODE B) {    
    return A.buf.st_blocks - B.buf.st_blocks;
}

int sort_cmp_d(NODE A, NODE B) { // 默认比较函数
    return strcmp(A.name, B.name);
}

int sort_cmp_t(NODE A, NODE B) {
    return B.buf.st_ctime - A.buf.st_ctime ;
}

void sort(int mode, PNODE PFList, int FLIndex) {
    if(mode & HAVE_U) { // 如果有U 则直接退出 不需要排序
        return ;
    }
    int (*sort_cmp)(NODE A, NODE B)  = sort_cmp_d; // 声明一个函数指针

    if(mode & HAVE_t){
        sort_cmp = sort_cmp_t;
    }
    if(mode & HAVE_S){
        sort_cmp = sort_cmp_S;
    }

    // 排序
    for(int i = 0; i < FLIndex - 1; i++) {
        for(int j = i + 1; j < FLIndex; j++) {
            NODE temp;
            if(sort_cmp(PFList[i], PFList[j]) > 0) {
                temp = PFList[i];
                PFList[i] = PFList[j];
                PFList[j] = temp;
            }            
        }
    }

}

void getList(char *pathname, int *MAX_PATH, int *FLIndex, PNODE PFList, int mode, int *MAX_LIST) {
    DIR *dp = NULL;
    struct dirent *dirp;
    int index; // 保存原始字符串下标
    struct stat buf;

    if(pathname[strlen(pathname) -1] != '/') {  // 如果最后一个字符不为/ 则加上
        if(strlen(pathname) + 2 < *MAX_PATH) { // 如果空间不够 则扩容
            *MAX_PATH *= 2;
            if((pathname = realloc(pathname, *MAX_PATH)) == NULL) {
                perror("pathname realloc error");
                exit(1);                
            }
        }

        // 加/
        pathname[strlen(pathname) + 1] = '\0';
        pathname[strlen(pathname)] = '/';

    }

    index = strlen(pathname);
    if((dp = opendir(pathname)) == NULL) {
        perror("opendir error");
        exit(1);
    }

    for((*FLIndex) = 0; (dirp = readdir(dp)) != NULL; // 循环
        (*FLIndex)++, pathname[index] = '\0') {
        
        if(!(mode & HAVE_a) && dirp->d_name[0] == '.') { // 如果没有a 则跳过 . ..
            (*FLIndex)--;
            continue;
        }

        strcat(pathname, dirp->d_name); // 连接路径和文件名
// printf("pathname : %s\n", pathname);
        if(lstat(pathname, &buf) == -1) {
            printf("\npathname : %s\t", pathname);
            perror("lstat error");
            putchar('\n');
        }

        if(*FLIndex > *MAX_LIST) { // 扩容
            (*MAX_LIST) *= 2;
            if((PFList = realloc(PFList, LINE_MAX * sizeof(NODE))) == NULL) {
                perror("PFList realloc error");
                exit(1);
            }
        }
// printf("flindex : %d\n", *FLIndex);
        // 将该文件的数据放入数组中
        PFList[*FLIndex].buf = buf; 
        strcpy(PFList[*FLIndex].name, dirp->d_name);
    }
}

int isDir(char *pathname) { // 判断是否为目录
    struct stat buf;

    if(lstat(pathname, &buf) == -1) {
        perror("lstat error");
        exit(1);
    }

    return S_ISDIR(buf.st_mode);
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

void init(PCTRLNODE pcnode) {
    pcnode->MAX_LIST = 32;  // 最初的数组最大长度
    pcnode->MAX_PATH = 256; // 最初的路径最大长度
    pcnode->FLIndex = 0;
    pcnode->mode = 0;
    pcnode->sumBlock = 0;

    // 给pathname申请空间
    if((pcnode->pathname = (char *)malloc(sizeof(char) * pcnode->MAX_PATH)) == NULL) {
        perror("init pathname malloc error");
        exit(1);
    }
    memset(pcnode->pathname, 0, pcnode->MAX_PATH);

    // 给数组申请空间
    if((pcnode->PFList = (PNODE)malloc(sizeof(NODE) * pcnode->MAX_LIST)) == NULL) {
        perror("init fileList malloc error");
        exit(1);
    }
    memset(pcnode->PFList, 0, pcnode->MAX_LIST);
}

int main(int argc, char *argv[]) {

    CTRLNODE cnode;
    init(&cnode);
    analysisArgv(argc, argv, &cnode.mode, cnode.pathname);

    if(isDir(cnode.pathname)) {
        getList(cnode.pathname, &(cnode.MAX_PATH), &(cnode.FLIndex), cnode.PFList, cnode.mode, &(cnode.MAX_LIST));
        sort(cnode.mode, cnode.PFList, cnode.FLIndex);
    }else {
        strcpy(cnode.PFList[0].name, cnode.pathname);
        lstat(cnode.pathname, &cnode.PFList[0].buf);
        cnode.FLIndex = 1;        
    }

    cnode.fileNameMax = otherWork(cnode.FLIndex, cnode.PFList, &(cnode.sumBlock));
    show(cnode.FLIndex, cnode.mode, cnode.PFList, cnode.fileNameMax, cnode.sumBlock);
    return 0;
}
