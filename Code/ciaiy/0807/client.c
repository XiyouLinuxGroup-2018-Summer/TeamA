#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
 #include <arpa/inet.h>

int main(void) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0) {
        perror("socket error");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6220);
    if(inet_aton("127.0.0.1",  &serv_addr.sin_addr) < 0) {
        perror("inet_aton error : ");
        exit(1);
    }
    if(connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("connect error : ");
        exit(1);
    }

    char msg[1024];
    recv(socket_fd, msg, 1024, 0);
    printf("from : %s", msg);
}