#include <iostream>
#include <queue>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef struct point {
    int num;    // 第几步
    int index;  // 当前位置
}NODE;

int main(void) {
    int x, y;

    while(cin>>x>>y) {

        int road[100005] = {0}; // 整个路径 
        queue<NODE> que;    // 队列
        NODE now, next; // 保存当前位置 和 通向

        // 初始化
        now.index = x;
        now.num = 0;
        road[now.index] = 1;
        que.push(now);


        while(!que.empty()) { // 队列不为空
            now = que.front(); // 取队首元素
            que.pop();  // 将旧的队首元素PoP

            if(now.index == y) { // 如果符合条件
                cout<<now.num<<endl;
                break;
            }
            
            // 不符合
            if(now.index - 1 >= 0 && now.index - 1 <= 100000 && road[now.index - 1] != 1) {
                next.num = now.num + 1;
                next.index = now.index - 1;
                road[next.index] = 1;
                que.push(next);
            }
            if(now.index + 1 >= 0 && now.index + 1 <= 100000 && road[now.index + 1] != 1) {
                next.num = now.num + 1;
                next.index = now.index + 1;
                road[next.index] = 1;
                que.push(next);
            }
            if(now.index * 2 >= 0 && now.index * 2 <= 100000 && road[now.index * 2] != 1) {
                next.num = now.num + 1;
                next.index = now.index * 2;
                road[next.index] = 1;
                que.push(next);
            }
        }
    }

}