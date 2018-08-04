/*************************************************************************
	> File Name: c.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月04日 星期六 17时08分48秒
 ************************************************************************/

/*
exec系列函数

#include <unistd.h>
       extern char **environ;

       int execl(const char *path, const char *arg, ...
                       /* (char  *) NULL );
       int execlp(const char *file, const char *arg, ...
                       /* (char  *) NULL );
       int execle(const char *path, const char *arg, ...
                       /*, (char *) NULL, char * const envp[] );
       int execv(const char *path, char *const argv[]);
       int execvp(const char *file, char *const argv[]);
       int execvpe(const char *file, char *const argv[],
                       char *const envp[]);
*/
/*
execv函数 通过路径名的方式调用可执行文件作为新的进程映像。它的argv参数用来提供给main函数的argv参数
         argv参数是一个以NULL结尾的字符串数组。
execve函数 参数path要执性程序路径名,参数argv,envp与main函数的参数相对应
esecl函数  该函数与execv用法类似，只是在传递argv参数的时候，每个命令行参数都声明为一个单独的参数，
	       以空指针结尾
execvp函数 在参数filename中，该参数如果含"/",就相当于路径名，如果不含“/”，就到PATH环境变量定义的
		  目录中寻找可执行文件
execlp函数 类似于execl函数

*/
