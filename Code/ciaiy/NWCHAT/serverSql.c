#include "server.h"

extern MYSQL sql;

int sql_has_power(int userID, int groupID, int power) {
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int temp;
    
    sprintf(sqlMsg, "select status from memList where groupID = %d and frdID = %d;", groupID, userID);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while((sqlRow = mysql_fetch_row(sqlRes))) {
        temp = atoi(sqlRow[0]);
    }
    if(temp >= power) {
        printf("~~%d 的权限够了 ~~\n", userID);
        return 1;
    }else {
        printf("~~%d  的权限不够~~\n", userID);
        return 0;
    }
}

int sql_add_user_to_grp(int userID, int groupID, int power)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;

    sprintf(sqlMsg, "insert into memList (groupID, frdID, status) value (%d, %d, %d);", groupID, userID, power);
    sql_run(&sql, 0, sqlMsg);
    printf("结束 sql_adduserTogRP\n");
}

int sql_create_grp(int userID, char *name)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int groupID = -1;

    sprintf(sqlMsg, "select min(groupID) from groupID");
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        if (sqlRow[0] != NULL)
        {
            groupID = atoi(sqlRow[0]) - 1;
        }
        else
        {
            break;
        }
    }
    mysql_free_result(sqlRes);
    sprintf(sqlMsg, "insert into groupID (groupID, name)value(%d, '%s');", groupID, name);
    sql_run(&sql, 0, sqlMsg);

    return groupID;
}

void sql_quit_grp(int userID, int ctlID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;

    sprintf(sqlMsg, "delete from memList where frdID = %d and groupID = %d;", userID, ctlID);
    sql_run(&sql, 0, sqlMsg);
}

/* 判断好友是否被阻塞 */
int sql_is_blocked(int userID, int ctlID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int num = 0;

    sprintf(sqlMsg, "select count(status) from frdList where ID = %d and frdID = %d and status = 1;", userID, ctlID);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        num = atoi(sqlRow[0]);
    }

    return num;
}

/* 阻塞某人 */
void sql_ctlblock_frd(int userID, int ctlID, int flag)
{
    char sqlMsg[512];

    sprintf(sqlMsg, "update frdList SET status = %d where ID = %d and frdID = %d", flag == BLOCK_FRD ? 1 : 0, userID, ctlID);
    sql_run(&sql, 0, sqlMsg);
}

/* 通过fd得到ID */
int sql_get_ID_by_fd(int fd)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int ID = 0;

    sprintf(sqlMsg, "select ID from onlineList where fd = %d;", fd);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        ID = atoi(sqlRow[0]);
    }
    mysql_free_result(sqlRes);

    return ID;
}

/* 得到在线好友  arr为ID数组 返回数目 */
int sql_get_onlineFrd(int userID, int groupID, int **arr)
{
    int onlineFrdNum = 0;
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    (*arr) = (int *)malloc(4 * GRP_MAX * MEM_MAX);
    if ((*arr) == NULL)
    {
        perror("malloc");
        exit(1);
    }
    memset(*arr, 0, 4 * GRP_MAX * MEM_MAX);

    if (groupID)
    {
        sprintf(sqlMsg, "select frdID from memList where groupID = %d", groupID);
    }
    else
    {
        sprintf(sqlMsg, "select frdID from frdList where ID = %d", userID);
    }
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        if(sqlRow[0] == NULL) {
            return 0;
        }
        int frdID = atoi(sqlRow[0]);
        if (sql_is_online(frdID))
        {
            (*arr)[onlineFrdNum++] = frdID;
            printf("好友/群友 %d在线\n", frdID);
        }
    }
    mysql_free_result(sqlRes);

    return onlineFrdNum;
}

/* 得到某个成员的信息 */
cJSON *sql_get_info(int userID, int groupID, int ctlID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int status;
    int online;
    char *name;
    cJSON *item = cJSON_CreateObject();

    name = sql_get_name(ctlID);
    status = sql_get_status(userID, groupID, ctlID);
    online = sql_is_online(ctlID) ? 1 : 0;
    printf("查询完成\n");
    cJSON_AddNumberToObject(item, "ID", ctlID);
    cJSON_AddStringToObject(item, "name", name);
    cJSON_AddNumberToObject(item, "status", status);
    cJSON_AddNumberToObject(item, "online", online);
    printf("返回json : %s\n", cJSON_PrintUnformatted(item));
    return item;
}

/* 成为好友 */
void sql_be_frd(int AID, int BID)
{
    char sqlMsg[512];

    sprintf(sqlMsg, "insert into frdList (ID, frdID, status)value(%d, %d, 0);", AID, BID);
    sql_run(&sql, 0, sqlMsg);
    sprintf(sqlMsg, "insert into frdList (ID, frdID, status) value (%d, %d, 0);", BID, AID);
    sql_run(&sql, 0, sqlMsg);
}

/* 得到群成员列表 (cjson数组)*/
cJSON *sql_get_memList(int grpID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    MYSQL_ROW sqlRow;
    int status = UNBLOCK;
    int ID;
    int online;
    char *name;
    cJSON *memArr = cJSON_CreateArray();

    sprintf(sqlMsg, "select frdID from memList where groupID = %d;", grpID);
    serr(&sql, "get memlist", __LINE__);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        cJSON *item;
        printf("获得成员信息\n");
        item = sql_get_info(0, grpID, atoi(sqlRow[0]));
        printf("得到群成员信息%s\n", cJSON_PrintUnformatted(item));
        cJSON_AddItemToArray(memArr, item);
    }
    mysql_free_result(sqlRes);
    printf("获取所有群成员信息%s\n", cJSON_PrintUnformatted(memArr));
    return memArr;
}

/* 得到群列表 arr为json数组, 返回值为数目 */
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
        printf("进入群中\n");
        cJSON *item = cJSON_CreateObject();
        grpID = atoi(sqlRow[0]);
        char *name = sql_get_name(grpID);
        printf("获得群成员信息中\n");
        cJSON *grpMem = sql_get_memList(grpID);
        printf("grpMem : %s\n", cJSON_PrintUnformatted(grpMem));
        cJSON_AddNumberToObject(item, "groupID", grpID);
        cJSON_AddStringToObject(item, "name", name);
        cJSON_AddItemToObject(item, "memlist", grpMem);
        cJSON_AddItemToArray(arr, item);
        printf("整个群信息%s\n", cJSON_PrintUnformatted(item));
        grpNum++;
    }
    mysql_fetch_row(sqlRes);
    printf("结束得到群列表, %s\n", cJSON_PrintUnformatted(arr));
    return grpNum;
}

/* 得到成员状态 */
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

/* 得到好友列表信息 */
int sql_get_frdList(cJSON *arr, int userID)
{
    char sqlMsg[512];
    MYSQL_RES *sqlRes;
    int frdNum = 0;
    MYSQL_ROW sqlRow;

    sprintf(sqlMsg, "select frdID from frdList where ID = %d", userID);
    sqlRes = sql_run(&sql, 1, sqlMsg);
    serr(&sql, "get frdLis", __LINE__);
    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        cJSON *item;
        item = sql_get_info(userID, 0, atoi(sqlRow[0]));
        cJSON_AddItemToArray(arr, item);
        frdNum++;
    }
    mysql_free_result(sqlRes);

    return frdNum;
}

/* 检查是否在线 */
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
    printf("结束sql_isonline\n");
    return sendfd;
}

/* 得到昵称 */
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
        sprintf(sqlMsg, "select name from groupID where groupID = %d;", ID);
    }

    sqlRes = sql_run(&sql, 1, sqlMsg);
    serr(&sql, "sqlrun", __LINE__);

    while ((sqlRow = mysql_fetch_row(sqlRes)))
    {
        name = (char *)malloc(strlen(sqlRow[0]) + 1);
        name[strlen(sqlRow[0])] = 0;
        strcpy(name, sqlRow[0]);
    }
    mysql_fetch_row(sqlRes);

    return name;
}

/* 检查密码 */
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
    printf("执行 %s\n", sqlMsg);
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