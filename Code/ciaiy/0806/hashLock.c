#include <stdlib.h>
#include <pthread.h>

struct node *pnode;

pthread_mutex_t next_lock;

struct node {
    int data; // 数据
    pthread_mutex_t data_lock; // 保护数据的锁
    struct node * pnext;    // 指向下一节点
}

struct node * node_alloc() {
    struct node *new = malloc(sizeof(struct node));
    pthread_mutex_lock(next_lock);  // 先让 *pnode 的 pnexe 指向 new
    pnode->pnext = new;
    pthread_mutex_lock(&data_lock); // 在next_lock锁定的情况下,锁定数据锁
    pthread_mutex_unlock(&next_lock);   // 将next锁解开
    // input your data
    pthread_mutex_unlock(&data_lock);   // 结束后将数据锁解开

    return new;
}