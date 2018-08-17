#include "client.h"

void start(int port) {

    /* 初始化socket部分 */
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0) {
        err("socket error", __LINE__);
    }
    addr.sin_addr.s_addr = atoi("localhost");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    saLen = sizeof(struct sockaddr);
    if(connect(clientSocket, (struct sockaddr*)&addr, saLen) < 0) {}
        err("connect error", __LINE__);
    }

    /* 登录界面 */
    printf("***********************************************\n");
    printf("*                NWCHAT                       *\n");
    printf("*               a. login                      *\n");
    printf("*              b. register                    *\n");
    printf("*               ESC. quit                     *\n");
    printf("***********************************************\n");
    
}

/* 错误输出函数 */
void err(char *msg, int len) {
    fprintf(stderr, "%s %s in %d", msg, strerror(errno), len);
    exit(1);
}
