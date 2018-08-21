/* 本文件包含服务器运行的大部分函数 */

#include "server.h"

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
        printf("%s\n", cJSON_PrintUnformatted(root));
        root = sql_get_info(recvID, 0, sendID);
        cJSON_AddNumberToObject(root, "type", FRESHFRD);
        cJSON_AddNumberToObject(root, "recvID", recvID);
        addSendQue(root);
        printf("%s\n", cJSON_PrintUnformatted(root));
    }
    else
    {
        //cJSON_Delete(root);
        //root = cJSON_CreateObject();
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

    if (flag)
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
    }
    else
    {
        root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "status", 0);
        cJSON_AddNumberToObject(root, "recvID", userID);
        cJSON_AddNumberToObject(root, "sendID", 0);
        cJSON_AddNumberToObject(root, "fd", fd);
        addSendQue(root);
    }
}

/* 广播自己状态 */
void sendFrdOnline(int userID) {
    int num = 0;
    int *arr;
    
    // 给群广播
    num = sql_get_onlineFrd(userID, 0, &arr);
    printf("进入了sendFrdOnline 当前用户在线%d\n", num);
    for(int i = 0;i < num; i++) {
        cJSON *root = sql_get_info(arr[i], 0, userID);
        cJSON_AddNumberToObject(root, "type", FRESHFRD);
        cJSON_AddNumberToObject(root, "recvID", arr[i]);
        cJSON_AddNumberToObject(root, "sendID", userID);
        printf("当当当~%s\n", cJSON_PrintUnformatted(root));
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
    printf("end send\n");
}

/* 将json数据转换成字符串 */
int cJSON_ToPackage(cJSON *root, char **sendPack)
{
    char *temp;
    int len;

    temp = cJSON_PrintUnformatted(root);
    len = strlen(temp) + 4;
    *sendPack = (char *)malloc(len);
    strcpy((*sendPack) + 4, temp);
    *(int *)(*sendPack) = len - 4;
    free(temp);

    cJSON_Delete(root);
    return len;
}

/* 登录成功发送信息 */
void sendInitInfo(int userID)
{
    printf("进入send Init Info函数\n");
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
    cJSON_AddItemToObject(grpRoot, "grpInfo", grpArr);
    cJSON_AddNumberToObject(grpRoot, "grpNum", grpNum);
    cJSON_AddNumberToObject(grpRoot, "recvID", userID);
    cJSON_AddNumberToObject(grpRoot, "type", INITGRP);
    //addSendQue(grpRoot);
}
