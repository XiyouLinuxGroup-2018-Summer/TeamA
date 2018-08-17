/*************************************************************************
	> File Name: public.h
	> Author: 
	> Mail: 
	> Created Time: 2018年08月17日 星期五 15时17分06秒
 ************************************************************************/

#ifndef _PUBLIC_H
#define _PUBLIC_H
#endif


//打包发送函数   属于公共函数
void pk_s(cJSON * smessage,int fd);

//收包函数   属于公共函数
void pk_r(char **name,int fd);

int getch();