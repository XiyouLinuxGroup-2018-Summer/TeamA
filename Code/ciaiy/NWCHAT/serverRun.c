/* 本文件包含服务器运行的大部分函数 */

#include "server.h"
  
/* 登录函数 */
void login(cJSON *root, int fd) {
    int flag = 0;   
    char sqlMsg[512];
    MYSQL_ROW sqlRow;
    MYSQL_RES *sqlRes;
    pthread_t temp;

    int userID = cJSON_GetObjectItem(root, "sendID")->valueint;

    flag = sql_verify_passwd(root, userID);
    cJSON_Delete(root);

    if(flag) {
        sql_add_onlineList(userID, fd); 
        root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "name", sql_get_name(userID));
        cJSON_AddNumberToObject(root, "status", 1);
        cJSON_AddNumberToObject(root, "recvID", userID);
        cJSON_AddNumberToObject(root, "sendID", 0);
        cJSON_AddNumberToObject(root, "fd", 0);
        pthread_create(&temp, 0, addSendQue, (void*)root);
        sendInitInfo(userID);
    }else{
        root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "status", 0);
        cJSON_AddNumberToObject(root, "recvID", userID);
        cJSON_AddNumberToObject(root, "sendID", 0);
        cJSON_AddNumberToObject(root, "fd", fd);
        pthread_create(&temp, 0, addSendQue, (void*)root);
    }
}

/* 注册函数 */
void registerID(cJSON *root, int fd) {
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
void *addSendQue(void *data) {
    /* 可能会有线程安全问题 */
    int sendfd;
    int recvID = cJSON_GetObjectItem((cJSON *)data, "recvID")->valueint;

    sendfd = sql_is_online(recvID);

    if(sendfd ==  0) {
        if(cJSON_HasObjectItem((cJSON *)data, "fd")) {
            sendfd = cJSON_GetObjectItem((cJSON *)data, "fd")->valueint;
        }else {
            printf("用户不在线");
            cJSON_Delete((cJSON *)data);
            return NULL;
        }
    }

    char *sendPack;
    int len = cJSON_ToPackage((cJSON *)data, &sendPack);
    send(sendfd, sendPack, len, 0);
    free(sendPack);
    printf("end send");

    return NULL;
}

/* 将json数据转换成字符串 */
int cJSON_ToPackage(cJSON *root, char **sendPack) {
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
sendInitInfo(userID) {
    
}

