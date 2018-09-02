#include "client.h"

using namespace std;

#define IN_MSG 1
#define OTHER 2

vector<cJSON *> noticeBox;
map<int, vector<cJSON *>> msgBox;

typedef struct STAT
{
    int now;
    int ctlID;
} STAT;

STAT NOW;

pthread_mutex_t grpListLock = PTHREAD_MUTEX_INITIALIZER;

void recvFile(cJSON *data)
{
    char path[512] = "";
    strcat(path, cJSON_GetObjectItem(data, "filename")->valuestring);
    FILE *frecv = fopen(path, "a+");
    fprintf(frecv, "%s", cJSON_GetObjectItem(data, "block")->valuestring);
    fflush(frecv);
    fclose(frecv);
    
    if (cJSON_GetObjectItem(data, "EOF")->valuestring)
    {
        printf("传输文件完成");
        frecv = fopen(path, "a+");
        FILE *fresult = fopen("result", "a+");
        fseek(frecv, 0, SEEK_SET);
        decode(frecv, fresult);
        printf("传输完成\n");
    }
}

void sendFile(int ctlID)
{
    char path[256];
    printf("\033[2J");
    printf("请输入文件路径:\n");
    scanf("%s", path);
    fflush(stdin);
    FILE *fi = fopen(path, "r");
    strcat(path, "_temp");
    FILE *fo = fopen(path, "w+");
    encode(fi, fo);
    char buf[513];
    buf[512] = 0;
    int num = 0;
    fseek(fo, 0, SEEK_SET);
    while ((num = fread(buf, 1, 512, fo)) != 0)
    {
        int eof = num == 512 ? 0 : 1;
        cJSON *data = cJSON_CreateObject();
        buf[num] = 0;
        int len;
        char *sendPack;
        cJSON_AddNumberToObject(data, "type", SEND_FILE);
        cJSON_AddStringToObject(data, "filename", path);
        if (eof)
        {
            cJSON_AddNumberToObject(data, "EOF", 1);
        }
        else
        {
            cJSON_AddNumberToObject(data, "EOF", 0);
        }
        cJSON_AddNumberToObject(data, "sendID", myID);
        cJSON_AddNumberToObject(data, "recvID", ctlID);
        cJSON_AddStringToObject(data, "block", buf);
        printf("send %s\n", cJSON_PrintUnformatted(data));
        len = cJSON_ToPackage(data, &sendPack);
        send(clientSocket, sendPack, len, 0);
        printf("读取了%d\n", num);
        usleep(50000);
    }
    getch();
}

void freshGrp(cJSON *root)
{
    printf("进入了freshGrp\n");
    int i;
    for (i = 0; i < GRP_MAX; i++)
    {
        if (grpList[i].groupID == 0)
        {
            break;
        }
    }
    strcpy(grpList[i].name, cJSON_GetObjectItem(root, "name")->valuestring);
    printf("1\n");
    grpList[i].groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
    printf("2\n");

    cJSON *memlist = cJSON_GetObjectItem(root, "memlist");
    printf("3\n");

    cJSON *item = cJSON_GetArrayItem(memlist, 0);
    printf("4\n");

    for (int j = 0; j < cJSON_GetArraySize(memlist); j++)
    {
        printf("5\n");

        grpList[i].memList[j].userID = cJSON_GetObjectItem(item, "ID")->valueint;
        grpList[i].memList[j].status = cJSON_GetObjectItem(item, "status")->valueint;
        grpList[i].memList[j].online = cJSON_GetObjectItem(item, "online")->valueint;
        strcpy(grpList[i].memList[j].name, cJSON_GetObjectItem(item, "name")->string);
        item = item->next;
    }
}

void addGrp(void)
{
    int grpID = 0;
    int len;
    char *sendPack;
    printf("\033[2J");
    printf("请输入要添加的群ID:");
    scanf("%d", &grpID);
    grpID = -grpID;
    fflush(stdin);

    cJSON *data = cJSON_CreateObject();
    cJSON_AddNumberToObject(data, "type", REQUEST_ADD_GRP);
    cJSON_AddNumberToObject(data, "ctlID", grpID);
    cJSON_AddNumberToObject(data, "sendID", myID);
    len = cJSON_ToPackage(data, &sendPack);
    send(clientSocket, sendPack, len, 0);
}

void initMem(cJSON *root)
{
    pthread_mutex_lock(&grpListLock);
    printf("initMem : JSON : %s\n", cJSON_PrintUnformatted(root));
    int i;
    for (i = 0; i < GRP_MAX; i++)
    {
        printf("正在查找: %s\n", grpList[i].name);
        if (grpList[i].groupID == cJSON_GetObjectItem(root, "grpID")->valueint)
        {
            break;
        }
    }
    printf("initMem : 找到的群%s\n", grpList[i].name);
    grpList[i].memList[0].online = cJSON_GetObjectItem(root, "online")->valueint;
    printf("1\n");
    grpList[i].memList[0].status = cJSON_GetObjectItem(root, "status")->valueint;
    printf("2\n");

    grpList[i].memList[0].userID = cJSON_GetObjectItem(root, "ID")->valueint;
    printf("3\n");

    strcpy(grpList[i].memList[0].name, cJSON_GetObjectItem(root, "name")->valuestring);
    pthread_mutex_unlock(&grpListLock);
}

void initNewGrp(cJSON *root)
{
    int i;
    printf("initNewGrp : %s\n", cJSON_PrintUnformatted(root));
    int grpID = cJSON_GetObjectItem(root, "grpID")->valueint;
    pthread_mutex_lock(&grpListLock);
    for (i = 0; i < GRP_MAX; i++)
    {
        if (grpList[i].groupID == 0)
        {
            break;
        }
    }
    printf("插入位置 %d\n", i);
    grpList[i].groupID = grpID;
    strcpy(grpList[i].name, cJSON_GetObjectItem(root, "name")->valuestring);
    vector<cJSON *> temp;
    msgBox[grpID] = temp;
    pthread_mutex_unlock(&grpListLock);
}

void createGrp(void)
{
    printf("\033[2J");
    if (grpList[GRP_MAX - 1].groupID == 0)
    {
        int len;
        char name[32];
        char *sendPack;
        printf("请输入群名称:");
        scanf("%s", name);
        cJSON *data = cJSON_CreateObject();
        cJSON_AddStringToObject(data, "name", name);
        cJSON_AddNumberToObject(data, "sendID", myID);
        cJSON_AddNumberToObject(data, "type", CREATE_GRP);
        len = cJSON_ToPackage(data, &sendPack);
        send(clientSocket, sendPack, len, 0);
    }
    else
    {
        printf("群数量已满\n");
        sleep(1);
    }
}

void delMem(cJSON *root)
{
    printf("~~%s\n", cJSON_PrintUnformatted(root));
    int ctlID = cJSON_GetObjectItem(root, "ctlID")->valueint;
    int groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
    int i, j;
    for (i = 0; i < GRP_MAX; i++)
    {
        if (grpList[i].groupID == groupID)
        {
            printf("已查到groupID = %d\n", groupID);
            break;
        }
    }

    for (j = 0; j < MEM_MAX; j++)
    {
        if (ctlID == grpList[i].memList[j].userID)
        {
            printf("已查到ID = %d\n", ctlID);
            break;
        }
    }
    printf("%s 被删\n", grpList[i].memList[j].name);
    grpList[i].memList[j].userID = 0;
}

void quitGrp(int ctlID)
{
    cJSON *data = cJSON_CreateObject();
    int len;
    char *sendPack;
    cJSON_AddNumberToObject(data, "sendID", myID);
    cJSON_AddNumberToObject(data, "ctlID", ctlID);
    cJSON_AddNumberToObject(data, "type", QUIT_GRP);
    printf("%s\n", cJSON_PrintUnformatted(data));
    len = cJSON_ToPackage(data, &sendPack);
    int num = send(clientSocket, sendPack, len, 0);
    printf("num = %d\n", num);
    free(sendPack);
}

void freshMem(cJSON *root)
{
    int groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
    printf("1\n");
    int status = cJSON_GetObjectItem(root, "status")->valueint;
    printf("2\n");
    int online = cJSON_GetObjectItem(root, "online")->valueint;
    printf("3\n");

    int ID = cJSON_GetObjectItem(root, "ID")->valueint;
    printf("4\n");

    char name[32];
    printf("lallalla\n");
    strcpy(name, cJSON_GetObjectItem(root, "name")->valuestring);

    for (int i = 0; i < GRP_MAX; i++)
    {
        if (grpList[i].groupID != groupID)
        {
            continue;
        }
        for (int j = 0; j < MEM_MAX; j++)
        {
            if (grpList[i].memList[j].userID == ID)
            {
                grpList[i].memList[j].status = status;
                grpList[i].memList[j].online = online;
                strcpy(grpList[i].memList[j].name, name);
            }
        }
    }
    cJSON_Delete(root);
}

void showMem(int ctlID)
{
    int index;
    printf("%d\n", ctlID);
    for (index = 0; index < GRP_MAX; index++)
    {
        if (grpList[index].groupID == ctlID)
        {
            printf("%s\n", grpList[index].name);
            break;
        }
    }
    printf("%d\n", index);
    for (int i = 0; i < MEM_MAX; i++)
    {
        if (grpList[index].memList[i].userID == 0)
        {
            continue;
        }
        if (grpList[index].memList[i].online)
        {
            printf("\33[35m%10s [%7d]\033[0m\n", grpList[index].memList[i].name, grpList[index].memList[i].userID);
        }
        else
        {
            printf("%10s [%7d]\n", grpList[index].memList[i].name, grpList[index].memList[i].userID);
        }
    }
}

void ctlGrp(int ctlID)
{
    int ch;
    do
    {
        printf("\033[2J");
        printf("a.查看群成员\n");
        printf("b.群聊\n");
        printf("c.管理成员\n");
        printf("d.退出群聊\n");
        printf("e.返回\n");

        ch = getch();

        if (ch == 'a')
        {
            showMem(ctlID);
            getch();
        }
        if (ch == 'b')
        {
            sendMsg(ctlID);
        }
        if (ch == 'c')
        {
            printf("正在完成中\n");
        }
        if (ch == 'd')
        {
            quitGrp(ctlID);
        }
        if (ch == 'e')
        {
            return;
        }
    } while (ch != 'a' && ch != 'b' && ch != 'c' && ch != 'd' && ch != 'e');
}

int hasGrp(int ctlID)
{
    for (int i = 0; i < GRP_MAX; i++)
    {
        if (grpList[i].groupID == ctlID)
        {
            return 1;
        }
    }
    return 0;
}

void grpFun(void)
{
    int ctlID;
    while (1)
    {

        printf("\033[2J");
        for (int i = 0; grpList[i].groupID != 0; i++)
        {
            printf("%s [%d]\n", grpList[i].name, -grpList[i].groupID);
        }
        printf("请选择要操作的群ID(0 退出):");
        scanf("%d", &ctlID);
        if (ctlID == 0)
        {
            return;
        }
        ctlID = -ctlID;
        if (hasGrp(ctlID))
        {
            ctlGrp(ctlID);
        }
        else
        {
            printf("%d 没有找到该群\n", -ctlID);
        }
    }
}

void ctlBlockFrd(int ctlID, int flag)
{
    for (int i = 0; i < MEM_MAX; i++)
    {
        if (frdList[i].userID == ctlID)
        {
            frdList[i].status = flag == BLOCK_FRD ? 1 : 0;
            break;
        }
    }
    int len;
    char *sendPack;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "sendID", myID);
    cJSON_AddNumberToObject(root, "ctlID", ctlID);
    cJSON_AddNumberToObject(root, "type", flag);
    len = cJSON_ToPackage(root, &sendPack);
    send(clientSocket, sendPack, len, 0);
    free(sendPack);
}

void ctlMsg(cJSON *data)
{
    int type = cJSON_GetObjectItem(data, "type")->valueint;
    int ID;
    int sendID = cJSON_GetObjectItem(data, "sendID")->valueint;

    if (type == GROUP_MSG)
    {
        ID = cJSON_GetObjectItem(data, "ctlID")->valueint;
        msgBox[ID].push_back(data);
    }
    else
    {
        ID = cJSON_GetObjectItem(data, "sendID")->valueint;
        msgBox[ID].push_back(data);
    }

    if (NOW.now == IN_MSG && NOW.ctlID == ID && sendID != myID)
    {
        printf("\33[34m%d :%s \n\033[0m", cJSON_GetObjectItem(data, "sendID")->valueint, cJSON_GetObjectItem(data, "msg")->valuestring);
    }
}

void sendMsg(int ctlID)
{
    char str[256];
    char *sendPack;
    int len;

    NOW.now = IN_MSG;
    NOW.ctlID = ctlID;
    printf("当前聊天对象ID : %d\n", ctlID);
    int num = msgBox[ctlID].size();
    for (int i = 0; i < num; i++)
    {
        int sendID = cJSON_GetObjectItem(msgBox[ctlID].at(i), "sendID")->valueint;
        char *msg = cJSON_GetObjectItem(msgBox[ctlID].at(i), "msg")->valuestring;
        printf("%d :%s\n", sendID, msg);
    }
    while (1)
    {
        printf("%d :", myID);
        fgets(str, 256, stdin);
        str[strlen(str) - 1] = 0;
        if (strcmp(str, "bye") == 0)
        {
            NOW.now = OTHER;
            break;
        }
        cJSON *data = cJSON_CreateObject();
        cJSON *temp = cJSON_CreateObject();
        cJSON_AddNumberToObject(temp, "sendID", myID);
        cJSON_AddStringToObject(temp, "msg", str);
        msgBox[ctlID].push_back(temp);
        cJSON_AddNumberToObject(data, "sendID", myID);
        cJSON_AddNumberToObject(data, "recvID", ctlID);
        cJSON_AddStringToObject(data, "msg", str);
        cJSON_AddNumberToObject(data, "type", ctlID > 0 ? PRIVATE_MSG : GROUP_MSG);
        len = cJSON_ToPackage(data, &sendPack);
        send(clientSocket, sendPack, len, 0);
        free(sendPack);
    }
}

void ctlFrd(int ctlID)
{
    printf("a. 发消息\n");
    printf("b. 屏蔽好友\n");
    printf("c. 解除屏蔽\n");
    printf("d. 发文件\n");
    printf("x. 退出\n");
    char ch;
    do
    {
        ch = getch();
    } while (ch != 'a' && ch != 'b' && ch != 'c' && ch != 'x' && ch != 'd');
    printf("%c\n", ch);
    if (ch == 'a')
    {
        sendMsg(ctlID);
    }
    if (ch == 'b')
    {
        ctlBlockFrd(ctlID, BLOCK_FRD);
    }
    if (ch == 'c')
    {
        ctlBlockFrd(ctlID, UNBLOCK_FRD);
    }
    if (ch == 'd')
    {
        sendFile(ctlID);
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
            printf("\33[35m%7s [%7d] %8s\033[0m\n", frdList[i].name, frdList[i].userID, frdList[i].status ? "<blocked>" : "");
        }
        else
        {
            printf("%7s [%7d] %8s\n", frdList[i].name, frdList[i].userID, frdList[i].status ? "<blocked>" : "");
        }
    }
    printf("请选择要操作的好友ID(0退出):");
    scanf("%d", &ctlID);
    fflush(stdin);
    if (ctlID == 0)
    {
        return;
    }
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
    while (1)
    {
        printf("\033[2J");
        printf("a. 好友列表\n");
        printf("b. 群列表\n");
        printf("c. 创建群\n");
        printf("d. 添加好友\n");
        printf("e. 添加群\n");
        printf("f. 新通知 (%d)\n", noticeBox.size());
        printf("x. 退出\n");

        printf("运行getch\n");
        ch = getch();
        printf("getch 得到的是 : '%c'\n", ch);
        switch (ch)
        {
        case 'a':
            frdFun();
            break;
        case 'b':
            grpFun();
            break;
        case 'c':
            createGrp();
            break;
        case 'd':
            addFrd();
            break;
        case 'e':
            addGrp();
            break;
        case 'f':
            pthread_mutex_lock(&noticeLock);
            showNotice();
            pthread_mutex_unlock(&noticeLock);
            break;
        case 'x':
            exit(0);
        }
    }
}

/* 返回状态 */
void returnRequest(int groupID, int recvID, int sign, int ret)
{
    cJSON *root = cJSON_CreateObject();
    char *sendPack;
    int len;
    if (groupID)
    {
        cJSON_AddNumberToObject(root, "groupID", groupID);
    }
    cJSON_AddNumberToObject(root, "type", sign);
    cJSON_AddNumberToObject(root, "recvID", recvID);
    cJSON_AddNumberToObject(root, "return", ret);
    cJSON_AddNumberToObject(root, "sendID", myID);
    printf("发送: %s\n", cJSON_PrintUnformatted(root));
    len = cJSON_ToPackage(root, &sendPack);
    send(clientSocket, sendPack, len, 0);
    printf("结束发送\n");
}

void ctlAddGrp(cJSON *root)
{
    int ch;
    char *name = cJSON_GetObjectItem(root, "name")->valuestring;
    int recvID = cJSON_GetObjectItem(root, "sendID")->valueint;
    int ctlID = cJSON_GetObjectItem(root, "ctlID")->valueint;
    printf("加群申请:\n%s(%d)请求加群 %d [Y/N]\n", name, recvID, ctlID);
    do
    {
        ch = getch();
    } while (ch != 'Y' && ch != 'N');
    returnRequest(ctlID, recvID, RETURN_ADD_GRP, (ch == 'Y') ? 1 : 0);
}

/* 添加好友功能 */
void ctlAddFrd(cJSON *root)
{
    int ch;
    char *name = cJSON_GetObjectItem(root, "name")->valuestring;
    int recvID = cJSON_GetObjectItem(root, "sendID")->valueint;
    printf("添加好友:\n%s(%d)请求加你为好友[Y/N]\n", name, recvID);
    do
    {
        ch = getch();
    } while (ch != 'Y' && ch != 'N');
    returnRequest(0, recvID, RETURN_ADD_FRD, (ch == 'Y') ? 1 : 0);
}

/* 显示通知 */
void showNotice()
{
    printf("\033[2J");
    if (noticeBox.size() == 0)
    {
        printf("这里好像什么都没有\n");
        sleep(1);
        return;
    }
    for (int i = 0; i < noticeBox.size(); i++)
    {
        printf("size %d, when i = %d\n", noticeBox.size(), i);
        printf("%s\n", cJSON_PrintUnformatted(noticeBox.at(i)));
        if (cJSON_GetObjectItem(noticeBox.at(i), "type")->valueint == REQUEST_ADD_FRD)
        {
            ctlAddFrd(noticeBox.at(i));
            noticeBox.erase(noticeBox.begin() + i); // 删除该数据
        }
        else if (cJSON_GetObjectItem(noticeBox.at(i), "type")->valueint == REQUEST_ADD_GRP)
        {
            ctlAddGrp(noticeBox.at(i));
            noticeBox.erase(noticeBox.begin() + i);
        }
        else if (cJSON_GetObjectItem(noticeBox.at(i), "type")->valueint == ADD_FRD_SUCCESS)
        {
            freshfrd(noticeBox.at(i));
            noticeBox.erase(noticeBox.begin() + i);
        }
        else if (cJSON_GetObjectItem(noticeBox.at(i), "type")->valueint == ADD_GRP_SUCCESS)
        {
            freshGrp(noticeBox.at(i));
            noticeBox.erase(noticeBox.begin() + i);
        }
    }
    printf("lllllllll\n");
}

/* 添加好友函数 */
void addFrd(void)
{
    int ctlID;
    int len;
    cJSON *root = cJSON_CreateObject();
    char *sendPack;

    //# system("clear"); \033[2J
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
    printf("进入initGrp\n");
    cJSON *grplist = cJSON_GetObjectItem(root, "grplist");
    cJSON *grpItem = cJSON_GetArrayItem(grplist, 0);
    int grpnum = cJSON_GetArraySize(grplist);
    printf("grpnum %d\n", grpnum);
    for (int i = 0; i < grpnum; i++)
    {
        printf("进入for\n");
        grpList[i].groupID = cJSON_GetObjectItem(grpItem, "groupID")->valueint;
        printf("1\n");
        strcpy(grpList[i].name, cJSON_GetObjectItem(grpItem, "name")->valuestring);
        printf("2\n");
        cJSON *memlist = cJSON_GetObjectItem(grpItem, "memlist");
        printf("3\n");
        printf("memlist : %s\n", cJSON_PrintUnformatted(memlist));
        int memNum = cJSON_GetArraySize(memlist);
        cJSON *item = cJSON_GetArrayItem(memlist, 0);
        for (int j = 0; j < memNum; j++)
        {
            grpList[i].memList[j].userID = cJSON_GetObjectItem(item, "ID")->valueint;
            grpList[i].memList[j].status = cJSON_GetObjectItem(item, "status")->valueint;
            grpList[i].memList[j].online = cJSON_GetObjectItem(item, "online")->valueint;
            strcpy(grpList[i].memList[j].name, cJSON_GetObjectItem(item, "name")->valuestring);
            item = item->next;
        }

        grpItem = grpItem->next;
        printf("继续for\n");
    }
    printf("初始化结束\n");
    NOW.now = INITSUCCESS;
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
}

/* 分析函数 */
void analysis(cJSON *root)
{
    int type = cJSON_GetObjectItem(root, "type")->valueint;
    printf("收到了 %s\n", cJSON_PrintUnformatted(root));
    if (type == INITFRD)
    {
        initFrd(root);
    }
    if (type == INITGRP)
    {
        initGrp(root);
    }
    if (type == REQUEST_ADD_FRD || type == REQUEST_ADD_GRP)
    {
        system("play notice.mp3 2>/dev/null &");
        pthread_mutex_lock(&noticeLock);
        noticeBox.push_back(root);
        pthread_mutex_unlock(&noticeLock);
    }
    if (type == FRESHFRD)
    {
        system("play online.mp3 2>/dev/null &");
        freshfrd(root);
    }

    if (type == PRIVATE_MSG || type == GROUP_MSG)
    {
        system("play msg.mp3 2>/dev/null &");
        ctlMsg(root);
    }
    if (type == FRESH_GRP_MEM)
    {
        system("play online.mp3 2>/dev/null &");
        freshMem(root);
    }
    if (type == C_QUIT_GRP)
    {
        delMem(root);
    }
    if (type == INIT_MEM)
    {
        printf("analysis : %s\n", cJSON_PrintUnformatted(root));
        initMem(root);
    }
    if (type == INIT_GRP)
    {
        initNewGrp(root);
    }
    if (type == ADD_GRP_FAILD || type == ADD_GRP_SUCCESS || type == ADD_GRP_SUCCESS || type == ADD_FRD_SUCCESS)
    {
        system("play notice.mp3 2>/dev/null &");
        pthread_mutex_lock(&noticeLock);
        noticeBox.push_back(root);
        pthread_mutex_unlock(&noticeLock);
    }
    if (type == SEND_FILE)
    {
        recvFile(root);
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

        printf("正在登录\n");
        for (int i = 0; NOW.now != INITSUCCESS; i++)
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
void start(char *ch_addr, char *ch_port)
{
    int choice;
    int port = atoi(ch_port);
    /* 初始化socket部分 */
    printf("%s %d\n", ch_addr, port);
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        err("socket error", __LINE__);
    }
    addr.sin_addr.s_addr = atoi(ch_addr);
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
        fflush(stdin);
        printf("getch\n");
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
        // printf("~~~~%s\n", cJSON_PrintUnformatted(root));
        analysis(root);
    }
}

/* 收包函数 */
cJSON *recvPack(void)
{
    printf("处理一次recv\n");
    int len;
    if(recv(clientSocket, &len, sizeof(int), 0) == 0) {
        printf("和服务器失去连接\n");
        exit(1);
    }

    int index = 0;
    fprintf(stderr, "Length = %d\n", len);
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
    len = strlen(temp) + 5;
    *sendPack = (char *)malloc(len);
    (*sendPack)[len - 1] = 0;
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

int main(int argc, char *argv[])
{
    start(argv[1], argv[2]);

    return 0;
}

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

const char *base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const unsigned char *bindata, char *base64, int binlength)
{
    int i, j;
    unsigned char current;

    for (i = 0, j = 0; i < binlength; i += 3)
    {
        current = (bindata[i] >> 2);
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];

        current = ((unsigned char)(bindata[i] << 4)) & ((unsigned char)0x30);
        if (i + 1 >= binlength)
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ((unsigned char)(bindata[i + 1] >> 4)) & ((unsigned char)0x0F);
        base64[j++] = base64char[(int)current];

        current = ((unsigned char)(bindata[i + 1] << 2)) & ((unsigned char)0x3C);
        if (i + 2 >= binlength)
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ((unsigned char)(bindata[i + 2] >> 6)) & ((unsigned char)0x03);
        base64[j++] = base64char[(int)current];

        current = ((unsigned char)bindata[i + 2]) & ((unsigned char)0x3F);
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return base64;
}

int base64_decode(const char *base64, unsigned char *bindata)
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
    for (i = 0, j = 0; base64[i] != '\0'; i += 4)
    {
        memset(temp, 0xFF, sizeof(temp));
        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i])
                temp[0] = k;
        }
        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i + 1])
                temp[1] = k;
        }
        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i + 2])
                temp[2] = k;
        }
        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i + 3])
                temp[3] = k;
        }

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2)) & 0xFC)) |
                       ((unsigned char)((unsigned char)(temp[1] >> 4) & 0x03));
        if (base64[i + 2] == '=')
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4)) & 0xF0)) |
                       ((unsigned char)((unsigned char)(temp[2] >> 2) & 0x0F));
        if (base64[i + 3] == '=')
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6)) & 0xF0)) |
                       ((unsigned char)(temp[3] & 0x3F));
    }
    return j;
}

void encode(FILE *fp_in, FILE *fp_out)
{
    unsigned char bindata[2050];
    char base64[4096];
    size_t bytes;
    while (!feof(fp_in))
    {
        bytes = fread(bindata, 1, 2049, fp_in);
        base64_encode(bindata, base64, bytes);
        fprintf(fp_out, "%s", base64);
    }
}

void decode(FILE *fp_in, FILE *fp_out)
{
    int i;
    unsigned char bindata[2050];
    char base64[4096];
    size_t bytes;
    while (!feof(fp_in))
    {
        for (i = 0; i < 2048; i++)
        {
            base64[i] = fgetc(fp_in);
            if (base64[i] == EOF)
                break;
            else if (base64[i] == '\n' || base64[i] == '\r')
                i--;
        }
        bytes = base64_decode(base64, bindata);
        fwrite(bindata, bytes, 1, fp_out);
    }
}

void help(const char *filepath)
{
    fprintf(stderr, "Usage: %s [-d] [input_filename] [-o output_filepath]\n", filepath);
    fprintf(stderr, "\t-d\tdecode data\n");
    fprintf(stderr, "\t-o\toutput filepath\n\n");
}