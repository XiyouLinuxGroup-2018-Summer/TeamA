#include <iostream>
#include <cstdio>

using namespace std;
int arr[5][5];
int seq[5][5];

const int INF = 7777777;

bool done = false;
void recout(int x = 4, int y = 4)
{
    if (done) return;
    if (x == 0 && y == 0) {
        done = true;
        printf("(%d, %d)\n", x, y);
        return;
    }
    if (x - 1 >= 0 && x - 1 < 5 && seq[x - 1][y] == seq[x][y] - 1 && !done) {
        recout(x - 1, y);
        if (done) printf("(%d, %d)\n", x, y);
    }
    if (y - 1 >= 0 && y - 1 < 5 && seq[x][y - 1] == seq[x][y] - 1 && !done)
    {
        recout(x, y - 1);
        if (done) printf("(%d, %d)\n", x, y);
    }
}

void dp() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i == 0 && j == 0) continue;
            seq[i][j] = INF;
            if (arr[i][j] == 0) {
                if (i - 1 >= 0 && i - 1 < 5)
                    seq[i][j] = min(seq[i][j], seq[i - 1][j] + 1);
                if (j - 1 >= 0 && j - 1 < 5)
                    seq[i][j] = min(seq[i][j], seq[i][j - 1] + 1);
            }
        }
    }
    recout();
}


int main()
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            scanf("%d", &arr[i][j]);

    dp();

    return 0;
}