/* 本文件包含服务器解析命令的函数 */

#include "server.h"

void *analysis(void *arg)
{
    cJSON *root = ((analysisArg_t *)arg)->data;
    int fd = ((analysisArg_t *)arg)->fd;
    int type = cJSON_GetObjectItem(root, "type")->valueint;
    if (type == REGISTER)
    {
        registerID(root, fd);
    }
    if (type == LOGIN)
    {
        login(root, fd);
    }
    if (type == REQUEST_ADD_FRD)
    {
        addFrd(root);
    }
    if (type == RETURN_ADD_FRD)
    {
        retAddFrd(root);
    }
    if (type == PRIVATE_MSG || type == GROUP_MSG)
    {
        sendMsg(root);
    }
    if (type == BLOCK_FRD || type == UNBLOCK_FRD)
    {
        ctlBlockFrd(root);
    }
    if (type == QUIT_GRP)
    {   
        quitGrp(root);
    }
    if(type == CREATE_GRP) {
        createGrp(root);
    }
    if(type == REQUEST_ADD_GRP) {
        addGrp(root);
    }
    if(type == RETURN_ADD_GRP) {
        retAddGrp(root);
    }
    if(type == SEND_FILE) {
        addSendQue(root);
    }
}