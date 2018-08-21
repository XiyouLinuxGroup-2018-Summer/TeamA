#include "server.h"

cJSON *sql_get_memList(int grpID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int status = UNBLOCK;
    int ID;
    int online;
    char *name;
    cJSON *memArr;

    sprintf(sqlMsg, "select frdID from memList where ID = %d;", grpID);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        cJSON *item = cJSON_CreateObject();
        ID = atoi(sqlRow[0]);
        name = sql_get_name(ID);
        status = sql_get_status(0, grpID, ID);
        online = sql_is_online(ID) ? 1 : 0;

        cJSON_AddNumberToObject(item, "ID", ID);
        cJSON_AddStringToObject(item, "name", name);
        cJSON_AddNumberToObject(item, "status", status);
        cJSON_AddNumberToObject(item, "online", online);
        cJSON_AddItemToArray(memArr, item);
    }
    mysql_free_result(sqlRes);

    return memArr;
}

int sql_get_grpList(cJSON *arr, int userID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int grpID;
    int grpNum = 0;

    sprintf(sqlMsg, "select groupID from memList where frdID = %d;", userID);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        cJSON *item;
        grpID = atoi(sqlRow[0]);
        char *name = sql_get_name(grpID);
        cJSON *grpMem = sql_get_memList(grpID);

        cJSON_AddNumberToObject(item, "groupID", grpID);
        cJSON_AddStringToObject(item, "name", name);
        cJSON_AddItemToObject(item, "members", grpMem);
        cJSON_AddItemToArray(arr, item);
        grpNum++;
    }
    mysql_fetch_row(sqlRes);

    return grpNum;
}

int sql_get_status(int userID, int groupID, int ctlID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int status = UNBLOCK;

    if (groupID)
    {
        // 群好友状态
        sprintf(sqlMsg, "select status from memList where groupID = %d and frdID = %d;", groupID, ctlID);
        sqlRes = sql_run(&sql, 1, sqlMsg);
        while ((sqlRow = mysql_fetch_row(sqlRes)))
        {
            status = atoi(sqlRow[0]);
        }
        mysql_free_result(sqlRes);
    }
    else
    {
        // 好友状态
        sprintf(sqlMsg, "select status from frdList where frdID = %d and ID = %d;", ctlID, userID);
        sqlRes = sql_run(&sql, 1, sqlMsg);
        while ((sqlRow = mysql_fetch_row(sqlRes)))
        {
            status = atoi(sqlRow[0]);
        }
        mysql_free_result(sqlRes);
    }

    return status;
}

int sql_get_frdList(cJSON *arr, int userID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int ID;
    int status;
    int online;
    char *name;
    int frdNum = 0;

    sprintf(sqlMsg, "select frdID from frdList where ID = %d", userID);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    serr(&sql, "get frdLis", __LINE__);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        cJSON *item = cJSON_CreateObject();
        ID = atoi(sqlRow[0]);
        name = sql_get_name(ID);
        status = sql_get_status(userID, 0, ID);
        online = sql_is_online(ID) ? 1 : 0;

        cJSON_AddNumberToObject(item, "ID", ID);
        cJSON_AddStringToObject(item, "name", name);
        cJSON_AddNumberToObject(item, "status", status);
        cJSON_AddNumberToObject(item, "online", online);
        cJSON_AddItemToArray(arr, item);
        frdNum++;
    }
    mysql_free_result(sqlRes);

    return frdNum;
}

int sql_is_online(int ID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int sendfd = 0;

    sprintf(sqlMsg, "select fd from onlineList where ID = %d", ID);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        sendfd = atoi(sqlRow[0]);
    }
    mysql_free_result(sqlRes);

    return sendfd;
}

char *sql_get_name(int ID)
{
    char *name;
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;

    if (ID > 0)
    {
        sprintf(sqlMsg, "select name from userID where ID = %d;", ID);
    }
    else
    {
        sprintf(sqlMsg, "select name from groupID where ID = %d;", ID);
    }

    sqlRes = sql_run(&sql, 1, sqlMsg);
    serr(&sql, "sqlrun", __LINE__);

    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        name = (char *)malloc(strlen(sqlRow[0]));
        strcpy(name, sqlRow[0]);
    }
    mysql_fetch_row(sqlRes);

    return name;
}

int sql_verify_passwd(cJSON *root, int userID)
{
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;
    int flag = 0;

    sprintf(sqlMsg, "select passwd from userID where id = %d;", userID);
    serr(&sql, "select", __LINE__);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    serr(&sql, "sqlres", __LINE__);

    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        if (strcmp(sqlRow[0], cJSON_GetObjectItem(root, "passwd")->valuestring) == 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }
    }
    mysql_free_result(sqlRes);

    return flag;
}

/* 用户表是否为空, 返回数据个数 */
int sql_is_register(void)
{
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;
    int dataNum = 0;

    sprintf(sqlMsg, "select count(ID) from userID;");
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        serr(&sql, "sqlRow", __LINE__);
        dataNum = atoi(sqlRow[0]);
    }
    mysql_free_result(sqlRes);

    return dataNum;
}

/* 得到当前最大ID,  没有返回10000 */
int sql_get_userID(int dataNum)
{
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;
    int ID;

    if (dataNum == 0)
    { // 没有用户
        ID = 10000;
    }
    else
    {
        sprintf(sqlMsg, "SELECT max(ID) from userID;");
        sqlRes = sql_run(&sql, 1, sqlMsg);
        while ((sqlRow = mysql_fetch_row(sqlRes)))
        {
            serr(&sql, "sqlrow", __LINE__);
            ID = atoi(sqlRow[0]) + 1;
        }
        mysql_free_result(sqlRes); // 释放结果集
    }

    return ID;
}

/* 加入用户表 */
int sql_add_userID(int ID, char *passwd, char *name)
{
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;

    sprintf(sqlMsg, "INSERT INTO userID (ID, passwd, name)VALUE(%d,'%s','%s');", ID, passwd, name);
    sql_run(&sql, 0, sqlMsg);
}

/* 加入在线用户表 */
void sql_add_onlineList(int ID, int fd)
{
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;

    sprintf(sqlMsg, "INSERT INTO onlineList (ID, fd)VALUE(%d, %d);", ID, fd);
    sql_run(&sql, 0, sqlMsg);
}

/* 执行sql语句函数 */
MYSQL_RES *sql_run(MYSQL *sql, int flag, const char *sqlMsg)
{
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;

    mysql_real_query(sql, sqlMsg, strlen(sqlMsg));
    printf("** %s\n", sqlMsg);
    serr(sql, "mysql qu...", __LINE__);
    if (flag)
    {
        sqlRes = mysql_store_result(sql);
        serr(sql, "sqlRes", __LINE__);
        return sqlRes;
    }
    else
    {
        return NULL;
    }
}

/* MYSQL错误输出函数 */
void serr(MYSQL *sql, const char *msg, int line)
{
    if (mysql_errno(sql))
    {
        fprintf(stderr, "Connection error %d: %s in %d\n", mysql_errno(sql),
                mysql_error(sql), line);
        exit(1);
    }
}