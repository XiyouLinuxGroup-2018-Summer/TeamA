#include <iostream>

using namespace std;

int sum = 0;
char map[8][8] = {0};
bool cow[8];

void fun(int high, int n, int num);

void fun(int high, int n, int num) {
    for(int i = high + 1; i < n; i++) {
        for(int j = 0; j < n; j++) {
            //cout<<map[i][j]<<"\t"<<cow[j]<<"\t"<<i<<"\t"<<j<<"\t"<<num<<endl;
            if(map[i][j] == '#' && cow[j] == true) {
                if(num == 1) {
                   // cout<<"!!!"<<sum + 1<<endl;
                    sum++;
                }else {

                //cout<<"正在查看"<<i<<", "<<j<<"号元素"<<endl;
                cow[j] = false;
                fun(i, n, num - 1);
                cow[j] = true;
                }
            }
        }
    }
}

int main(void) {
    int n, k;   // 尺寸
    while(cin>>n>>k) {
        int num = k;
        if(n == -1 && k == -1) {
            break;
        }        
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                cin>>map[i][j];
            }
        }
        for(int i = 0; i < 8; i++) {
            cow[i] = true;
        }
    // for(int i = 0; i < n; i++) {
    //     for(int j = 0; j < n; j++) {
    //         cout<<map[i][j];
    //     }
    //     cout<<endl;
    // }
        for(int i = 0; i <= n - k; i++) { // 确定第一颗棋子的位置
            for(int j = 0; j < n; j++) {
                if(map[i][j] == '#') {
                    cow[j] = false;
                    //cout<<"进入fun, i = "<<i<<endl;
                    if(num - 1 == 0) {
                        sum += 1;
                    }else {
                        fun(i, n, num - 1);;
                    }
                    cow[j] = true;
                }
            }
        }
    cout<<sum<<endl;
    sum = 0;

    }
    return 0;
}