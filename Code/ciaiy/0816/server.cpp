#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>
#include <queue>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "cJSON.h"

sockaddr_in addr;
int serverSocket;

void setNonblock(int serverSocket);
void *run(void *clientSocket);
void err(const char *msg, const int line);

void err(const char *msg, const int line) {
    fprintf(stderr, "ERROR : %s : %s in %d\n", msg, strerror(errno), line);
    exit(1);
}

void *run(void *clientSocket) {

}

void setNonblock(int serverSocket) {
    int opts = fcntl(serverSocket, F_GETFL);
    if (opts < 0) {
        err("opts error", __LINE__);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(serverSocket, F_SETFL, opts) < 0) {
        err("serverSocket fcntl error", __LINE__);
    }
}

int main(void) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1666);

    //setNonblock(serverSocket);
    if(bind(serverSocket, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
        perror("bind");
    }

    if(listen(serverSocket, 100) < 0) {
        perror("listen");
    }
    while(true) {
        int num;
        char *str;
        int clientSocket;
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(struct sockaddr);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &len);
        if(clientSocket < 0) {
            perror("accept error");
        }
        printf("ERR : %d %s \n", errno, strerror(errno));
        if(recv(clientSocket, &num, 4, 0) < 0) {
            perror("error");
        }
        printf("json 数据长度 : %d\n", num);
        str = (char *)malloc(num);
        if(recv(clientSocket, str, num, 0) < 0) {
            perror("error");
        }
        printf("json数据: %s\n", str);
    }

    return 0;
}