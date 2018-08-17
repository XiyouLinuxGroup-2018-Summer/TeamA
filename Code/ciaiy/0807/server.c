#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>

void *run(void *arg);


int main(void) {
    int serverSocket;
    int clientSocket;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(6220);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        perror("socket error : ");
        exit(1);
    }
    if(bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind error : ");
        exit(1);
    }

    if(listen(serverSocket, 1) < 0) {
        perror("listen error : ");
        exit(1);
    }
    struct sockaddr_in clientSockAddr;
    int clientSize = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientSockAddr, &clientSize);
    pthread_t test;
    pthread_create(&test, NULL, run, (void *)&clientSocket);
    if(clientSocket < 0) {
        perror("accpet error : ");
        exit(1);
    }
    pthread_join(test, NULL);
    return 0;
}
void *run(void *clientSocket) {
    char *hello = malloc(1024);
    strcpy(hello, "hi client~");
    send(*(int *)clientSocket, hello, strlen(hello), 0);
    sleep(1000);
}