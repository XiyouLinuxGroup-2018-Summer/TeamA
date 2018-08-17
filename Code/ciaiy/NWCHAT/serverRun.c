/* 本文件包含服务器运行的大部分函数 */

#include "server.h"

/* 注册函数 */
void registerID(cJSON *root, int fd) {
printf("进入registerID\n");
    int nowID;
    int dataNum;
    char name[32];
    char passwd[128];

    /* 解析json数据 */
    strcpy(passwd, cJSON_GetObjectItem(root, "passwd")->valuestring);
    strcpy(name, cJSON_GetObjectItem(root, "name")->valuestring);
    cJSON_Delete(root); // 释放json数据

    /* 查看是否有注册 */
printf("查看注册\n");
    sprintf(sqlMsg, "select count(ID) from userID;");
    sqlRes = sqlRun(&sql, 1, sqlMsg);
    while((sqlRow = mysql_fetch_row(sqlRes))) {
        serr(&sql, "sqlRow", __LINE__);
        dataNum = atoi(sqlRow[0]);
    }mysql_free_result(sqlRes); // 释放结果集

    /* 得到分配的userID */
printf("得到分配\n");
    if (dataNum == 0) { // 没有用户
        nowID = 10000;
    }else {
        sprintf(sqlMsg, "SELECT max(ID) from userID;");
        sqlRes = sqlRun(&sql, 1, sqlMsg);
        while((sqlRow = mysql_fetch_row(sqlRes))) {
            serr(&sql, "sqlrow", __LINE__);
            nowID = atoi(sqlRow[0]) + 1;
        }
    }mysql_free_result(sqlRes); // 释放结果集

    /* 加入用户表 */
printf("加入用户表");
    sprintf(sqlMsg, "INSERT INTO userID (ID, passwd, name)VALUE(%d,'%s','%s');"
        , nowID, passwd, name);
    sqlRun(&sql, 0, sqlMsg);

    /* 加入在线用户表 */
printf("加入在线用户表");
    sprintf(sqlMsg, "INSERT INTO onlineList (ID, fd)VALUE(%d, %d)"
        , nowID, fd);
    sqlRun(&sql, 0, sqlMsg);

    /* 包装json文件 加入发送队列 */
    cJSON *send = cJSON_CreateObject();
    cJSON_AddNumberToObject(send, "recvID", nowID);
    cJSON_AddNumberToObject(send, "sendID", 0);
    cJSON_AddNumberToObject(send, "type", REGISTER_SUCCESS);
    //addSendQue(send);
}

/* 添加发送队列函数 */
void addSendQue(cJSON *data) {

}

/* 执行sql语句函数 */
MYSQL_RES* sqlRun(MYSQL *sql, int flag, const char *sqlMsg) {
    printf("in sqlRun\n");
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;
    printf("--%d--sqlMsg : %s\n", __LINE__, sqlMsg);
    mysql_real_query(sql, sqlMsg, strlen(sqlMsg));
    serr(sql, "mysql qu...", __LINE__);
    if(flag) {

        sqlRes = mysql_store_result(sql);
        serr(sql, "sqlRes", __LINE__);
        printf("end the fun\n");
        return sqlRes;
    }else {
        printf("end the fun\n");
        return (MYSQL_RES *)0;
    }
 }
