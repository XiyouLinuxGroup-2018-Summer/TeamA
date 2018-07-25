/*************************************************************************
	> File Name: A.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 00时58分21秒
 ************************************************************************/


//还是特别需要注意进制转换，atoi是字符串转整形数函数

#include<stdio.h>

#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>

int main(int argc,char **argv)
{
    int mode;
    int mode_u;
    int mode_g;
    int mode_o;
    char *path;

    //检验参数个数的合法性

    if(argc<3)
    {
        printf("%s <mode num> <target file>\n",argv[0]);
        exit(0);
    }
    
    //获取命令行参数
    mode = atoi(argv[1]);
    if(mode>777||mode<0)
    {
        printf("mode num error!\n");
        exit(0);
    }
    
    mode_u = mode /100;
    mode_g = (mode_u*100) / 10;
    mode_o =mode - (mode_u*100)-(mode_g*10);
    mode = (mode_u*8*8)+(mode_g*8)+mode_o;

    path=argv[2];

    if(chmod(path,mode)==-1){
        perror("chmod error");
        exit(1);
    }

    return 0;
}

