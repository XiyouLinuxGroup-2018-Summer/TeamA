#include <iostream>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <vector>

struct cup {
    int size;
    int now;
};

struct status {
    struct cup cola;
    struct cup a;
    struct cup b;
    int num;
};

using namespace std;

bool hasread(vector<struct status> vec, struct status val);
void swap(struct cup &a, struct cup &b, vector<struct status> vec);

void swap(struct cup &a, struct cup &b, vector<struct status> vec) {
    if(a.now <=  b.size - b.now) {
        b.now += a.now;
        a.now = 0;
    }else {
        a.now = a.now - (b.size - b.now);
        b.now = b.size;
    }
}

bool hasread(vector<struct status> vec, struct status val) {
    for(int i = 0; i < vec.size(); i++) {
        if(val.a.now == vec[i].a.now && val.b.now == vec[i].b.now && val.cola.now == vec[i].cola.now) {
            return true;
        }
    }
    return false;
}

int main(void) {
    int cola, a, b;
    while(cin>>cola>>a>>b) {
        if(cola % 2 != 0) {
            cout<<"NO"<<endl;
            continue;
        }
        if(a + b < cola) {
            cout<<"NO"<<endl;
            continue;
        }

        if(a == 0 && b == 0 && cola == 0) {
            break;
        }

        queue<struct status> que;
        vector<struct status> vec;
        struct status stat;

        stat.cola.size = cola;
        stat.cola.now = cola;
        stat.a.size = a;
        stat.a.now = 0;
        stat.b.size = b;
        stat.b.now = 0;
        stat.num = 0;

        que.push(stat);
        vec.push_back(stat);

        while(!que.empty()) {
            struct status temp = que.front();
            que.pop();
            if((temp.b.now == temp.cola.size / 2 && temp.a.now == temp.cola.size / 2) || (temp.a.now == temp.cola.size / 2 && temp.cola.now == temp.cola.size / 2) || (temp.b.now == temp.cola.size / 2 && temp.cola.now == temp.cola.size / 2)) {              
                cout<<temp.num<<endl;
                break;
            }else {

                struct status arr[6];
                for(int i = 0; i < 6; i++) {
                    arr[i] = temp;
                }
                // a --> b
                swap(arr[0].a, arr[0].b, vec);
                if(!hasread(vec, arr[0])) {
                    arr[0].num++;
                    vec.push_back(arr[0]);
                    que.push(arr[0]);
                }
                // a --> cola
                swap(arr[1].a, arr[1].cola, vec);
                if(!hasread(vec, arr[1])) {
                    arr[1].num++;
                    vec.push_back(arr[1]);
                    que.push(arr[1]);
                }

                // b --> a
                swap(arr[2].b, arr[2].a, vec);
                if(!hasread(vec, arr[2])) {
                    arr[2].num++;
                    vec.push_back(arr[2]);
                    que.push(arr[2]);
                }
                // b --> cola
                swap(arr[3].b, arr[3].cola, vec);
                if(!hasread(vec, arr[3])) {
                    arr[3].num++;
                    vec.push_back(arr[3]);
                    que.push(arr[3]);
                }
                // cola --> a
                swap(arr[4].cola, arr[4].a, vec);
                if(!hasread(vec, arr[4])) {
                    arr[4].num++;
                    vec.push_back(arr[4]);
                    que.push(arr[4]);
                }
                // cola --> b
                swap(arr[5].cola, arr[5].b, vec);
                if(!hasread(vec, arr[5])) {
                    arr[5].num++;
                    vec.push_back(arr[5]);
                    que.push(arr[5]);
                }
            }

        }
    }
}