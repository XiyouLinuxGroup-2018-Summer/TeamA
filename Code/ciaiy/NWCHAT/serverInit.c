/* 本文件包含的是服务器的初始化工作 */

#include "server.h"

/* 开启服务器函数 */
void start(int port)
{

    /* 初始化数据库 */
    mysql_init(&sql);
    mysql_real_connect(&sql, "localhost", "root", "132134", "chat", 0, NULL, 0);
    serr(&sql, "connect", __LINE__);
    mysql_real_query(&sql, "use chat;", strlen("use chat;"));
    serr(&sql, "use database", __LINE__);
    mysql_real_query(&sql, "delete from onlineList;", strlen("delete from onlineList;"));
    serr(&sql, "del onlineList", __LINE__);
    /* 初始化serverSocket */
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err("serverSocket error", __LINE__);
    }
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    setNonblock(serverSocket);
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        err("bind", __LINE__);
    }
    if (listen(serverSocket, SERVER_WAIT_MAX) < 0)
    {
        err("listen", __LINE__);
    }

    /* 初始化epoll */
    epfd = epoll_create(EPOLL_WAIT_MAX);
    ev.data.fd = serverSocket;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serverSocket, &ev);

    while (1)
    {
        int num = epoll_wait(epfd, events, EPOLL_WAIT_MAX, 500);
        for (int i = 0; i < num; i++)
        {

            /* 新用户连接 */
            if (events[i].data.fd == serverSocket)
            {
                printf("一个新连接\n");

                struct sockaddr_in clientAddr;
                socklen_t len = sizeof(struct sockaddr);
                int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &len);

                if (clientSocket < 0)
                {
                    err("accept error", __LINE__);
                }

                ev.data.fd = clientSocket;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientSocket, &ev);

                continue;
            }
            int size;

            /* 用户下线 */
            if (recv(events[i].data.fd, &size, 4, 0) == 0)
            {
                int userID = sql_get_ID_by_fd(events[i].data.fd);
                printf("退出时的userID : %d\n", userID);
                sprintf(sqlMsg, "delete from onlineList where fd = %d;", events[i].data.fd);
                sql_run(&sql, 0, sqlMsg);
                serr(&sql, "user off line", __LINE__);
                if (userID)
                {
                    sendGrpOnline(userID);
                    printf("退出时 执行了它\n");
                    sendFrdOnline(userID);
                }

                ev.data.fd = events[i].data.fd;
                ev.events = EPOLL_CTL_DEL;
                epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
            }
            else
            {

                /* 用户发包 */
                char *data = (char *)malloc(size + 1);
                data[size] = '\0';
                printf("收到一个来自%d的包", events[i].data.fd);
                if (recv(events[i].data.fd, data, size, 0) < 0)
                {
                    err("recv data error", __LINE__);
                }

                cJSON *root = cJSON_Parse(data);
                printf("包内容：%s\n", cJSON_PrintUnformatted(root));
                free(data);
                pthread_t th;
                analysisArg_t *arg = (analysisArg_t *)malloc(sizeof(analysisArg_t));

                arg->data = root;
                arg->fd = events[i].data.fd;
                pthread_create(&th, NULL, analysis, (void *)arg);
            }
        }
    }
}

/* 错误输出函数 */
void err(char *msg, int len)
{
    fprintf(stderr, "%s %s in %d", msg, strerror(errno), len);
    exit(1);
}

/* 将serverSocket设置为非阻塞 */
void setNonblock(int serverSocket)
{
    int opts = fcntl(serverSocket, F_GETFL);
    if (opts < 0)
    {
        err("opts error", __LINE__);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(serverSocket, F_SETFL, opts) < 0)
    {
        err("serverSocket fcntl error", __LINE__);
    }
}
