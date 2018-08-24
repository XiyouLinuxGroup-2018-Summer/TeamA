#include "server.h"

int main(int argc, char *argv[]) {
    int port = atoi(argv[1]);
    start(port);
    return 0;
}
