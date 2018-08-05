#include <iostream>
#include <string>
#include <queue>

using namespace std;

int main() {
    int a;
    while(cin>>a) {
        if(a == 0) {
            break;
        }
        queue<unsigned long long> que;
        unsigned long long   b = 1;
        que.push(b);
        while(!que.empty()) {
            b = que.front();
            que.pop();
            if(b % a != 0) {
                que.push(b * 10);
                que.push(b * 10 + 1);
            }else {
                cout<<b<<endl;
                break;
            }
        }
    }

    return 0;
}