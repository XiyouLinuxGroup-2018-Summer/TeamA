#include "client.h"

using namespace std;

vector<cJSON *> noticeBox;
map<int, vector<cJSON *>> msgBox;

void ctlMsg(cJSON *data)
{
    printf("%d : %s\n", cJSON_GetObjectItem(data, "sendID")->valueint, cJSON_GetObjectItem(data, "msg")->valuestring);
}

void sendMsg(int ctlID)
{
    char str[256];
    char *sendPack;
    int len;
    for (int i = 0; i < msgBox[ctlID].size(); i++)
    {
        cJSON *temp = msgBox[i].at(i);
        printf("%d : %s\n", cJSON_GetObjectItem(temp, "sendID")->valueint, cJSON_GetObjectItem(temp, "msg")->valuestring);
    }
    printf("~~~\n");
    while (1)
    {
        printf("%d :", myID);
        fgets(str, 256, stdin);
        cJSON *data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "sendID", myID);
        cJSON_AddNumberToObject(data, "recvID", ctlID);
        cJSON_AddStringToObject(data, "msg", str);
        cJSON_AddNumberToObject(data, "type", PRIVATE_MSG);
        len = cJSON_ToPackage(data, &sendPack);
        send(clientSocket, sendPack, len, 0);
        free(sendPack);
        if (strcmp(str, "bye") == 0)
        {
            break;
        }
    }
}

void ctlFrd(int ctlID)
{
    printf("a. 发消息\n");
    printf("b. 屏蔽好友\n");
    printf("c. 发文件\n");
    printf("x. 退出\n");
    char ch;
    do
    {
        ch = getch();
    } while (ch != 'a' && ch != 'b' && ch != 'c' && ch != 'x');
    printf("%c\n", ch);
    if (ch == 'a')
    {
        sendMsg(ctlID);
    }
}

/* 好友界面操作函数 */
void frdFun(void)
{
    int ctlID;
    for (int i = 0; frdList[i].userID != 0; i++)
    {
        if (frdList[i].online)
        {
            printf("\33[35m %s [%d] %s \033[0m\n", frdList[i].name, frdList[i].userID, frdList[i].status ? "<blocked>" : "");
        }
        else
        {
            printf(" %s [%d] %s \n", frdList[i].name, frdList[i].userID, frdList[i].status ? "<blocked>" : "");
        }
    }
    printf("请选择要操作的好友ID:");
    scanf("%d", &ctlID);
    if (msgBox.count(ctlID))
    {
        ctlFrd(ctlID);
    }
    else
    {
        printf("%d 没有找到该好友\n", ctlID);
        sleep(1);
    }
}

/* 刷新好友 */
void freshfrd(cJSON *root)
{
    int i = 0;
    int ID = cJSON_GetObjectItem(root, "ID")->valueint;
    int online = cJSON_GetObjectItem(root, "online")->valueint;
    int status = cJSON_GetObjectItem(root, "status")->valueint;
    char *name = cJSON_GetObjectItem(root, "name")->valuestring;
    for (i = 0; frdList[i].userID != 0; i++)
    {
        if (frdList[i].userID == ID)
        {
            frdList[i].status = status;
            frdList[i].online = online;
            strcpy(frdList[i].name, name);
            return;
        }
    }

    frdList[i].userID = ID;
    frdList[i].status = status;
    frdList[i].online = online;
    strcpy(frdList[i].name, name);
}

/* 桌面 */
void desktop(void)
{
    int ch;
    int freshflag = 0;
    while (1)
    {
        system("clear");
        printf("a. 好友列表\n");
        printf("b. 群列表\n");
        printf("c. 添加好友\n");
        printf("d. 找回密码\n");
        printf("e. 新通知 (%d)\n", noticeBox.size());
        printf("ESC 退出\n");
        // usleep(5000);
        // pthread_mutex_lock(&freshLock);
        // freshflag = needFresh;
        // needFresh = 0;
        // pthread_mutex_unlock(&freshLock);
        // if(freshflag) {
        // printf("有新消息\n");
        // sleep(1);
        //     freshflag = 0;
        //     continue;
        // }

        ch = getch();
        switch (ch)
        {
        case 'a':
            frdFun();
            break;
        case 'b':
            //grpFun();
            break;
        case 'c':
            addFrd();
            break;
        case 'd':
            //resetPasswd();
            break;
        case 'e':
            showNotice();
        default:
            ch = getch();
        }
    }
}

/* 返回状态 */
void returnRequest(int recvID, int sign, int ret)
{
    cJSON *root = cJSON_CreateObject();
    char *sendPack;
    int len;
    cJSON_AddNumberToObject(root, "type", sign);
    cJSON_AddNumberToObject(root, "recvID", recvID);
    cJSON_AddNumberToObject(root, "return", ret);
    cJSON_AddNumberToObject(root, "sendID", myID);
    printf("发送: %s\n", cJSON_PrintUnformatted(root));
    len = cJSON_ToPackage(root, &sendPack);
    send(clientSocket, sendPack, len, 0);
    printf("结束发送\n");
}

/* 添加好友功能 */
void ctlAddFrd(cJSON *root)
{
    int recvID = cJSON_GetObjectItem(root, "sendID")->valueint;
    int ch;
    printf("添加好友:\n%d 请求加你为好友[Y/N]\n", recvID);
    do
    {
        ch = getch();
    } while (ch != 'Y' && ch != 'N');
    returnRequest(recvID, RETURN_ADD_FRD, (ch == 'Y') ? 1 : 0);
}

/* 显示通知 */
void showNotice()
{
    system("clear");

    if (noticeBox.size() == 0)
    {
        printf("这里好像什么都没有\n");
        sleep(1);
        return;
    }

    for (int i = 0; i < noticeBox.size(); i++)
    {
        if (cJSON_GetObjectItem(noticeBox.at(i), "type")->valueint == REQUEST_ADD_FRD)
        {
            ctlAddFrd(noticeBox.at(i));
            noticeBox.erase(noticeBox.begin() + i, noticeBox.begin() + i + 1); // 删除该数据
        }
    }
}

/* 添加好友函数 */
void addFrd(void)
{
    int ctlID;
    int len;
    cJSON *root = cJSON_CreateObject();
    char *sendPack;

    system("clear");
    printf("请输入要加好友ID:");
    scanf("%d", &ctlID);

    cJSON_AddNumberToObject(root, "sendID", myID);
    cJSON_AddNumberToObject(root, "ctlID", ctlID);
    cJSON_AddNumberToObject(root, "type", REQUEST_ADD_FRD);
    len = cJSON_ToPackage(root, &sendPack);
    send(clientSocket, sendPack, len, 0);
}

/* 初始化群信息 */
void initGrp(cJSON *root)
{
    // printf("进入了initGrp\n");
    status = INITSUCCESS;
}

/* 初始化好友信息 */
void initFrd(cJSON *root)
{
    cJSON *list = cJSON_GetObjectItem(root, "frdInfo");
    cJSON *item = cJSON_GetArrayItem(list, 0);
    int num = cJSON_GetObjectItem(root, "frdNum")->valueint;
    for (int i = 0; i < num; i++)
    {
        //  printf("%d : \n", i);
        // printf("lallala \n%s\n", cJSON_PrintUnformatted(item));
        frdList[i].userID = cJSON_GetObjectItem(item, "ID")->valueint;
        frdList[i].status = cJSON_GetObjectItem(item, "status")->valueint;
        frdList[i].online = cJSON_GetObjectItem(item, "online")->valueint;
        strcpy(frdList[i].name, cJSON_GetObjectItem(item, "name")->valuestring);
        // printf("%4d %4d %4d %4d %9s\n", i, frdList[i].userID, frdList[i].status, frdList[i].online, frdList[i].name);
        vector<cJSON *> temp;
        msgBox[frdList[i].userID] = temp;
        item = item->next;
        // printf("进行下一个循环\n");
    }
    //  printf("结束initfrd\n");
    status = INITSUCCESS;
}

/* 分析函数 */
void analysis(cJSON *root)
{
    int type = cJSON_GetObjectItem(root, "type")->valueint;

    if (type == INITFRD)
    {
        initFrd(root);
    }
    if (type == INITGRP)
    {
        initGrp(root);
    }
    if (type == REQUEST_ADD_FRD)
    {
        noticeBox.push_back(root);
    }
    if (type == FRESHFRD)
    {
        freshfrd(root);
    }

    if (type == PRIVATE_MSG)
    {
        ctlMsg(root);
    }
}

/* 登录函数 */
void login()
{
    cJSON *root;

    /* 录入信息 */
    char *passTemp;
    char passwd[128];
    // system("clear");
    printf("ID:");
    scanf("%d", &myID);
    passTemp = getpass("PASSWORD:");
    strcpy(passwd, crypt(passTemp, "$1$ciaiy$"));
    free(passTemp);

    /* 生成json数据 */
    char *sendPack;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", LOGIN);
    cJSON_AddNumberToObject(root, "sendID", myID);
    cJSON_AddStringToObject(root, "passwd", passwd);
    int len = cJSON_ToPackage(root, &sendPack);
    send(clientSocket, sendPack, len, 0);
    free(sendPack);

    /* 收到json */
    int isSuccess;
    root = recvPack();

    /* 判断 */
    isSuccess = cJSON_GetObjectItem(root, "status")->valueint;
    if (isSuccess)
    {
        printf("密码正确\n");
        pthread_t recvThread;
        pthread_create(&recvThread, NULL, recvFun, NULL);
        // system("clear");

        printf("正在登录\n");
        for (int i = 0; status != INITSUCCESS; i++)
        {
            usleep(50000);
        }
        desktop();
    }
    else
    {
        printf("登录失败\n");
        sleep(1);
        exit(1);
    }
    cJSON_Delete(root);
}

/* 注册函数 */
void registerID()
{
    char *passTemp;
    char passwd[128];

    /* 录入信息 */
    // system("clear");
    printf("请输入昵称(30字符以内):");
    scanf("%s", myName);
    passTemp = getpass("请输入密码(8字符以内):");
    strcpy(passwd, crypt(passTemp, "$1$ciaiy$"));
    free(passTemp);

    /* 生成json数据 */
    int len;
    char *sendPack;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", myName);
    cJSON_AddStringToObject(root, "passwd", passwd);
    cJSON_AddNumberToObject(root, "type", REGISTER);
    printf("生成json : %s\n", cJSON_PrintUnformatted(root));
    len = cJSON_ToPackage(root, &sendPack);

    /* 发送数据 */
    printf("发送数据\n");
    send(clientSocket, sendPack, len, 0);

    /* 查收数据 */
    len = 0;
    printf("查收数据\n");
    root = recvPack();
    myID = cJSON_GetObjectItem(root, "recvID")->valueint;
    printf("收到ID %d\n", myID);
    cJSON_Delete(root);
}

/* 初始化及初始界面登录界面 */
void start(int port)
{
    int choice;

    /* 初始化socket部分 */
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        err("socket error", __LINE__);
    }
    addr.sin_addr.s_addr = atoi("localhost");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    saLen = sizeof(struct sockaddr);

    if (connect(clientSocket, (struct sockaddr *)&addr, saLen) < 0)
    {
        err("connect error", __LINE__);
    }

    /* 初始界面 */
    printf("***********************************************\n");
    printf("*                NWCHAT                       *\n");
    printf("*               a. login                      *\n");
    printf("*              b. register                    *\n");
    printf("*               ESC. quit                     *\n");
    printf("***********************************************\n");

    do
    {
        choice = getch();
        if (choice == 'a')
        {
            login();
            break;
        }
        if (choice == 'b')
        {
            registerID();
            break;
        }
        if (choice == KEY_ESC)
        {
            exit(0);
        }
    } while (choice != 'a' && choice != 'b' && choice != KEY_ESC);
}

/* 收包线程 */
void *recvFun(void *arg)
{
    printf("in recvFun\n");
    // 进行收包
    while (1)
    {
        cJSON *root = recvPack();
        analysis(root);
    }
}

/* 收包函数 */
cJSON *recvPack(void)
{
    int len;
    recv(clientSocket, &len, sizeof(int), 0);

    int index = 0;
    char *data = (char *)malloc(len + 1);
    data[len] = 0;

    while (index != len)
    {
        index += recv(clientSocket, data + index, len - index, 0);
    }

    return cJSON_Parse(data);
}

/* 将json数据转换成字符串 */
int cJSON_ToPackage(cJSON *root, char **sendPack)
{
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

/* getch函数 */
int getch(void)
{
    char ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt); // 保留终端设置
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);        // 标识相应标识位不回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 设置标识位
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 将标识位恢复

    return ch;
}

/* 错误输出函数 */
void err(char *msg, int len)
{
    fprintf(stderr, "%s %s in %d", msg, strerror(errno), len);
    exit(1);
}

int main(void)
{

    start(1024);

    return 0;
}
