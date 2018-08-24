/* 本文件包含服务器运行的大部分函数 */

#include "server.h"

void retAddGrp(cJSON *root)
{
    int *arr;
    int ret = cJSON_GetObjectItem(root, "return")->valueint;
    int recvID = cJSON_GetObjectItem(root, "recvID")->valueint;
    int sendID = cJSON_GetObjectItem(root, "sendID")->valueint;
    int groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
    if (ret)
    {
        sql_add_user_to_grp(recvID, groupID, 1);

        cJSON *sendData = cJSON_CreateObject();
        char *name = sql_get_name(groupID);
        cJSON *grpMem = sql_get_memList(groupID);
        cJSON_AddNumberToObject(sendData, "groupID", groupID);
        cJSON_AddStringToObject(sendData, "name", name);
        cJSON_AddItemToObject(sendData, "memlist", grpMem);
        cJSON_AddNumberToObject(sendData, "recvID", recvID);
        cJSON_AddNumberToObject(sendData, "type", ADD_GRP_SUCCESS);
        addSendQue(sendData);
        int num = sendToMem(recvID, groupID, &arr);
        for (int i = 0; i < num; i++)
        {
            if (sql_is_online(arr[i]))
            {
                cJSON *data = sql_get_info(arr[i], groupID, sendID);
                cJSON_AddNumberToObject(data, "type", FRESH_GRP_MEM);
                cJSON_AddNumberToObject(data, "groupID", groupID);
                cJSON_AddNumberToObject(data, "recvID", arr[i]);
                addSendQue(data);
            }
        }
    }
    else
    {
        cJSON *sendData = cJSON_CreateObject();
        cJSON_AddNumberToObject(sendData, "recvID", recvID);
        cJSON_AddNumberToObject(sendData, "type", ADD_GRP_FAILD);
        addSendQue(sendData);
    }
}

void addGrp(cJSON *root)
{
    int sendID = cJSON_GetObjectItem(root, "sendID")->valueint;
    int ctlID = cJSON_GetObjectItem(root, "ctlID")->valueint;
    int *arr;

    int num = sendToMem(ctlID, ctlID, &arr);
    for (int i = 0; i < num; i++)
    {
        printf("%d是群员\n", arr[i]);
        if (sql_has_power(arr[i], ctlID, ADMIN) && sql_is_online(arr[i]))
        {
            cJSON *data = cJSON_CreateObject();
            cJSON_AddNumberToObject(data, "sendID", sendID);
            cJSON_AddNumberToObject(data, "ctlID", ctlID);
            cJSON_AddStringToObject(data, "name", sql_get_name(sendID));
            cJSON_AddNumberToObject(data, "recvID", arr[i]);
            cJSON_AddNumberToObject(data, "type", REQUEST_ADD_GRP);
            addSendQue(data);
        }
    }
}

void createGrp(cJSON *root)
{
    int userID = cJSON_GetObjectItem(root, "sendID")->valueint;
    char *name = cJSON_GetObjectItem(root, "name")->valuestring;
    int grpID = sql_create_grp(userID, name);
    sql_add_user_to_grp(userID, grpID, BOSS);
    serr(&sql, "initNewGrp", __LINE__);
    printf("发送initNewGrp\n");
    cJSON *data = cJSON_CreateObject();
    cJSON_AddNumberToObject(data, "type", INIT_GRP);
    cJSON_AddNumberToObject(data, "grpID", grpID);
    cJSON_AddStringToObject(data, "name", name);
    cJSON_AddNumberToObject(data, "recvID", userID);
    addSendQue(data);
    printf("发送initMem\n");
    data = sql_get_info(userID, grpID, userID);
    cJSON_AddNumberToObject(data, "type", INIT_MEM);
    cJSON_AddNumberToObject(data, "grpID", grpID);
    cJSON_AddNumberToObject(data, "recvID", userID);
    addSendQue(data);
}

void quitGrp(cJSON *root)
{
    printf("进入了quitGrp\n\n\n\n\n");
    int *arr;
    int groupID = cJSON_GetObjectItem(root, "ctlID")->valueint;
    int userID = cJSON_GetObjectItem(root, "sendID")->valueint;
    sql_quit_grp(userID, groupID);
    int num = sendToMem(userID, groupID, &arr);
    for (int j = 0; j < num; j++)
    {
        printf("给 %d 发消息\n", arr[j]);
        cJSON *data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "type", C_QUIT_GRP);
        cJSON_AddNumberToObject(data, "recvID", arr[j]);
        cJSON_AddNumberToObject(data, "groupID", groupID);
        cJSON_AddNumberToObject(data, "ctlID", userID);
        printf("\n\n%s\n\n", cJSON_PrintUnformatted(data));
        addSendQue(data);
    }
    free(arr);
}

/* 给群广播信息 */
void sendGrpOnline(int userID)
{
    cJSON *grpList = cJSON_CreateArray();
    int grpNum = sql_get_grpList(grpList, userID);
    cJSON *item = cJSON_GetArrayItem(grpList, 0);
    for (int i = 0; i < grpNum; i++)
    {
        int *arr;
        int groupID = cJSON_GetObjectItem(item, "groupID")->valueint;
        int num = sendToMem(userID, groupID, &arr);
        for (int j = 0; j < num; j++)
        {
            cJSON *data = sql_get_info(userID, groupID, userID);
            cJSON_AddNumberToObject(data, "type", FRESH_GRP_MEM);
            cJSON_AddNumberToObject(data, "recvID", arr[j]);
            cJSON_AddNumberToObject(data, "groupID", groupID);
            addSendQue(data);
        }
        free(arr);
    }
}

/* 给群成员发 */
int sendToMem(int userID, int grpID, int **arr)
{
    int index;
    *arr = (int *)malloc(4 * MEM_MAX);
    memset(*arr, 0, 4 * MEM_MAX);
    cJSON *memList = sql_get_memList(grpID);
    cJSON *item = cJSON_GetArrayItem(memList, 0);
    int memNum = cJSON_GetArraySize(memList);

    for (index = 0; index < memNum; index++)
    {
        int ID = cJSON_GetObjectItem(item, "ID")->valueint;
        if (!ID)
        {
            break;
        }
        (*arr)[index] = ID;
        item = item->next;
    }
    return index;
}

/* 发消息 */
void sendMsg(cJSON *root)
{
    int userID = cJSON_GetObjectItem(root, "sendID")->valueint;
    if (cJSON_GetObjectItem(root, "type")->valueint == PRIVATE_MSG)
    {
        int recvID = cJSON_GetObjectItem(root, "recvID")->valueint;
        if (!sql_is_blocked(recvID, userID))
        {
            addSendQue(root);
        }
        else
        {
            printf("%d -> %d has blocked\n", userID, recvID);
        }
    }
    else
    {
        int grpID = cJSON_GetObjectItem(root, "recvID")->valueint;
        int *arr = (int *)malloc(4 * MEM_MAX);
        int num = sendToMem(userID, grpID, &arr);
        for (int i = 0; i < num; i++)
        {
            cJSON *temp = cJSON_CreateObject();
            cJSON_AddNumberToObject(temp, "sendID", cJSON_GetObjectItem(root, "sendID")->valueint);
            cJSON_AddNumberToObject(temp, "recvID", arr[i]);
            cJSON_AddNumberToObject(temp, "ctlID", grpID);
            cJSON_AddNumberToObject(temp, "type", cJSON_GetObjectItem(root, "type")->valueint);
            cJSON_AddStringToObject(temp, "msg", cJSON_GetObjectItem(root, "msg")->valuestring);
            addSendQue(temp);
            printf("发送了\n\n");
        }
    }
}

/* 阻塞某人 */
void ctlBlockFrd(cJSON *root)
{
    int flag = cJSON_GetObjectItem(root, "type")->valueint;
    int userID = cJSON_GetObjectItem(root, "sendID")->valueint;
    int ctlID = cJSON_GetObjectItem(root, "ctlID")->valueint;
    sql_ctlblock_frd(userID, ctlID, flag);
}

/* 添加好友返回函数 */
void retAddFrd(cJSON *root)
{
    int ret = cJSON_GetObjectItem(root, "return")->valueint;
    int recvID = cJSON_GetObjectItem(root, "recvID")->valueint;
    int sendID = cJSON_GetObjectItem(root, "sendID")->valueint;

    if (cJSON_GetObjectItem(root, "return")->valueint)
    {
        sql_be_frd(recvID, sendID);
        cJSON_Delete(root);
        root = cJSON_CreateObject();
        root = sql_get_info(sendID, 0, recvID);
        cJSON_AddNumberToObject(root, "type", FRESHFRD);
        cJSON_AddNumberToObject(root, "recvID", sendID);
        addSendQue(root);
        root = sql_get_info(recvID, 0, sendID);
        cJSON_AddNumberToObject(root, "type", ADD_FRD_SUCCESS);
        cJSON_AddNumberToObject(root, "recvID", recvID);
        addSendQue(root);
    }
    else
    {
        cJSON *data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "recvID", recvID);
        cJSON_AddNumberToObject(data, "type", ADD_FRD_FAILD);
        addSendQue(data);
    }
}

/* 添加好友请求函数 */
void addFrd(cJSON *root)
{
    int sendID = cJSON_GetObjectItem(root, "sendID")->valueint;
    int recvID = cJSON_GetObjectItem(root, "ctlID")->valueint;
    cJSON_Delete(root);
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "sendID", sendID);
    cJSON_AddNumberToObject(root, "recvID", recvID);
    cJSON_AddNumberToObject(root, "type", REQUEST_ADD_FRD);
    cJSON_AddStringToObject(root, "name", sql_get_name(sendID));
    addSendQue(root);
}

/* 登录函数 */
void login(cJSON *root, int fd)
{
    int flag = 0;
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;
    pthread_t temp;

    int userID = cJSON_GetObjectItem(root, "sendID")->valueint;

    flag = sql_verify_passwd(root, userID);
    cJSON_Delete(root);

    if (flag) /* 登录成功 */
    {
        sql_add_onlineList(userID, fd);
        root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "name", sql_get_name(userID));
        cJSON_AddNumberToObject(root, "status", 1);
        cJSON_AddNumberToObject(root, "recvID", userID);
        cJSON_AddNumberToObject(root, "sendID", 0);
        cJSON_AddNumberToObject(root, "fd", 0);
        addSendQue(root);
        sendInitInfo(userID);
        sendFrdOnline(userID);
        printf("\033[31ｍ 给在线好友广播\n\033[0m");
        sendGrpOnline(userID);
        printf("\033[31ｍ 给在线群广播\n\033[0m");
    }
    else
    {
        printf("登录失败\n");
        root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "status", 0);
        cJSON_AddNumberToObject(root, "recvID", userID);
        cJSON_AddNumberToObject(root, "sendID", 0);
        cJSON_AddNumberToObject(root, "fd", fd);
        addSendQue(root);
        printf("发送成功\n");
    }
}

/* 广播自己状态 */
void sendFrdOnline(int userID)
{
    int num = 0;
    int *arr;

    num = sql_get_onlineFrd(userID, 0, &arr);
    for (int i = 0; i < num; i++)
    {
        cJSON *root = sql_get_info(arr[i], 0, userID);
        cJSON_AddNumberToObject(root, "type", FRESHFRD);
        cJSON_AddNumberToObject(root, "recvID", arr[i]);
        cJSON_AddNumberToObject(root, "sendID", userID);
        addSendQue(root);
    }
}

/* 注册函数 */
void registerID(cJSON *root, int fd)
{
    int nowID;
    int dataNum;
    char name[32];
    char passwd[128];
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;

    /* 解析json数据 */
    strcpy(passwd, cJSON_GetObjectItem(root, "passwd")->valuestring);
    strcpy(name, cJSON_GetObjectItem(root, "name")->valuestring);
    cJSON_Delete(root); // 释放json数据

    /* 查看是否有注册 */
    dataNum = sql_is_register();

    /* 得到分配的userID */
    nowID = sql_get_userID(dataNum);

    /* 加入用户表 */
    sql_add_userID(nowID, passwd, name);

    /* 加入在线用户表 */
    sql_add_onlineList(nowID, fd);

    /* 包装json文件 加入发送队列 */
    cJSON *send = cJSON_CreateObject();
    cJSON_AddNumberToObject(send, "recvID", nowID);
    cJSON_AddNumberToObject(send, "sendID", 0);
    cJSON_AddNumberToObject(send, "type", REGISTER_SUCCESS);
    addSendQue(send);
}

/* 添加发送队列函数 */
void addSendQue(cJSON *data)
{
    int sendfd;
    int recvID = cJSON_GetObjectItem((cJSON *)data, "recvID")->valueint;
    printf("发送:%s\n", cJSON_PrintUnformatted(data));
    sendfd = sql_is_online(recvID);

    if (sendfd == 0)
    {
        if (cJSON_HasObjectItem((cJSON *)data, "fd"))
        {
            sendfd = cJSON_GetObjectItem((cJSON *)data, "fd")->valueint;
        }
        else
        {
            printf("用户不在线");
            cJSON_Delete((cJSON *)data);
            return;
        }
    }

    char *sendPack;
    int len = cJSON_ToPackage((cJSON *)data, &sendPack);
    send(sendfd, sendPack, len, 0);
    free(sendPack);
}

/* 将json数据转换成字符串 */
int cJSON_ToPackage(cJSON *root, char **sendPack)
{
    char *temp;
    int len;

    temp = cJSON_PrintUnformatted(root);
    len = strlen(temp) + 5;
    *sendPack = (char *)malloc(len);
    sendPack[len - 1] = 0;
    strcpy((*sendPack) + 4, temp);
    *(int *)(*sendPack) = len - 4;
    free(temp);

    cJSON_Delete(root);
    return len;
}

/* 登录成功发送信息 */
void sendInitInfo(int userID)
{
    /* 发送好友信息 */
    cJSON *frdArr = cJSON_CreateArray();
    int frdNum = sql_get_frdList(frdArr, userID);
    cJSON *frdRoot = cJSON_CreateObject();
    cJSON_AddItemToObject(frdRoot, "frdInfo", frdArr);
    cJSON_AddNumberToObject(frdRoot, "frdNum", frdNum);
    cJSON_AddNumberToObject(frdRoot, "recvID", userID);
    cJSON_AddNumberToObject(frdRoot, "type", INITFRD);
    addSendQue(frdRoot);

    /* 发送群信息 */
    cJSON *grpArr = cJSON_CreateArray();
    int grpNum = sql_get_grpList(grpArr, userID);
    cJSON *grpRoot = cJSON_CreateObject();
    cJSON_AddItemToObject(grpRoot, "grplist", grpArr);
    cJSON_AddNumberToObject(grpRoot, "grpNum", grpNum);
    cJSON_AddNumberToObject(grpRoot, "recvID", userID);
    cJSON_AddNumberToObject(grpRoot, "type", INITGRP);
    addSendQue(grpRoot);
}
