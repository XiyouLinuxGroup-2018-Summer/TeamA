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

using namespace std;

int clientSocket;
struct sockaddr_in addr;

int main(void) {
    char *txt = (char *)malloc(1024);
    cJSON *json;
    char *jsonTxt;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = atoi("localhost");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1666);
    int salen = sizeof(struct sockaddr);
    if(connect(clientSocket, (struct sockaddr*)&addr, salen) < 0) {
        perror("error");
    }

    json = cJSON_CreateObject();
    scanf("%s", txt);
    cJSON_AddStringToObject(json, "txt", txt);
    jsonTxt = cJSON_PrintUnformatted(json);
    int allLen = strlen(jsonTxt) + 4;
    char package[allLen];
    strcpy(package+4, jsonTxt);
    *(int *)package = strlen(jsonTxt);
    printf("%d %d %s\n", allLen, *(int *)package, package + 4);
    int n = send(clientSocket, &package,allLen , 0);
    return 0;
}