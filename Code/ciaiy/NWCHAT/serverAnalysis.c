/* 本文件包含服务器解析命令的函数 */

#include "server.h"

void analysis(cJSON *root, int fd) {
    int type = cJSON_GetObjectItem(root,  "type")->valueint;

    if(type == REGISTER) { 
        registerID(root, fd);
    }

        // case ADD_FRIEND : addFrd(sendID, recvID, msg); break;
        // case ADD_GROUP : addGrp(sendID, recvID, msg); break; 
        // case MSG_PRIVATE : sendMsg(sendID, recvID, msg); break;
        // case MSG_GROUP : sendMsg(sendID, recvID, msg); break;
        // case SEND_FILE : sendFile(sendID, recvID, fileName, fileIndex, flieSum, fileBlock); break;
        // case DEL_FRIEND : delFrd(sendID, ctlID); break;
        // case QUIT_GROUP : quitGrp(sendID, ctlID); break;
        // case DEL_GROUP : delGrp(sendID, ctlID); break;
        // case SET_ADMIN : setAdmin(sendID, recvID, ctlID); break;
        // case REMOVE_USER : removeUser(sendID, recvID, ctlID); break;
        // case BLOCK_USER : block(sendID, recvID, ctlID); break;
        // case UNBLOCK_USER : unblock(sendID, recvID, ctlID); break;
        // case CREATE_GRP : createGrp(sendID, msg); break;
        // case AGREE : agree(sendID, recvID, _bool);
    //     default : break;
    // }

}