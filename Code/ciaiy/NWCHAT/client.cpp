#include "client.h"

void start(int port) {
    int choice;
    
    /* 初始化socket部分 */
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        err("socket error", __LINE__);
    }
    addr.sin_addr.s_addr = atoi("localhost");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    saLen = sizeof(struct sockaddr);

    if (connect(clientSocket, (struct sockaddr *)&addr, saLen) < 0) {
        err("connect error", __LINE__);
    }

    printf("***********************************************\n");
    printf("*                NWCHAT                       *\n");
    printf("*               a. login                      *\n");
    printf("*              b. register                    *\n");
    printf("*               ESC. quit                     *\n");
    printf("***********************************************\n");

    do {
        choice = getch();
        if(choice == 'a') {
            login();
            break;
        }
        if(choice == 'b') {
            registerID();
            break;
        }
        if(choice == KEY_ESC) {
            exit(0);
        }
    }while(choice != 'a' && choice != 'b' && choice != KEY_ESC);

}

/* 登录函数 */
void login() {
    char *passTemp;
    char passwd[128];
    cJSON *root;

    /* 录入信息 */
    system("clear");
    printf("ID:");
    scanf("%d", &myID);
    passTemp = getpass("PASSWORD:");
    strcpy(passwd, crypt(passTemp, "$1$ciaiy$"));
    free(passTemp);

    /* 生成json数据 */
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", LOGIN);
    cJSON_AddNumberToObject(root, "sendID", myID);
    cJSON_AddStringToObject(root, "passwd", passwd);

    char *sendPack;
    int len = cJSON_ToPackage(root, &sendPack);
    send(clientSocket, sendPack, len, 0);
    free(sendPack);
    printf("结束发送\n");
    len = 0;
    int status;
    printf("查收数据\n");
    recv(clientSocket, &len, 4, 0);
    char *recvPack = (char *)malloc(len + 1);
    recvPack[len] = 0;
    recv(clientSocket, recvPack, len, 0);
    root = cJSON_Parse(recvPack);
    status = cJSON_GetObjectItem(root, "status")->valueint;
    if(status) {
        printf("登陆成功\n");
    }else {
        printf("登录失败\n");
        sleep(1);
        exit(1);
    }
    cJSON_Delete(root);
}

/* 注册函数 */
void registerID() {
    char *passTemp;
    char passwd[128];
    
    /* 录入信息 */
    system("clear");
    printf("请输入昵称(30字符以内):");
    scanf("%s", myName);
    passTemp = getpass("请输入密码(8字符以内):");
    strcpy(passwd, crypt(passTemp, "$1$ciaiy$"));
    free(passTemp);

    /* 生成json数据 */
    char sendPack[512];
    char *temp;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", myName);
    cJSON_AddStringToObject(root, "passwd", passwd);
    cJSON_AddNumberToObject(root, "type", REGISTER);
    temp = cJSON_PrintUnformatted(root);
    strcpy(sendPack + 4, temp);
    *(int *)sendPack = strlen(temp);

    /* 发送数据 */
    send(clientSocket, sendPack, strlen(temp) + 4, 0);
    printf("fasong : %s\n", cJSON_PrintUnformatted(root));
    free(temp);
    cJSON_Delete(root);

    /* 查收数据 */
    int len = 0;
    printf("查收数据\n");
    recv(clientSocket, &len, 4, 0);
    char *recvPack = (char *)malloc(len + 1);
    recvPack[len] = 0;
    recv(clientSocket, recvPack, len, 0);
    root = cJSON_Parse(recvPack);
    myID = cJSON_GetObjectItem(root, "recvID")->valueint;
    printf("收到ID %d\n", myID);
    cJSON_Delete(root);

}

/* 错误输出函数 */
void err(char *msg, int len)
{
    fprintf(stderr, "%s %s in %d", msg, strerror(errno), len);
    exit(1);
}

/* getch函数 */
int getch(void) {
    char ch;
    struct termios oldt,newt;

    tcgetattr(STDIN_FILENO,&oldt);  // 保留终端设置
	newt = oldt;
	newt.c_lflag &=~(ECHO | ICANON); // 标识相应标识位不回显
	tcsetattr(STDIN_FILENO,TCSANOW,&newt); // 设置标识位
	ch = getchar();
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt); // 将标识位恢复

    return ch;
}

int main(void) {

    start(1024);
    
    return 0;
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