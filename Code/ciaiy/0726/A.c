#include <stdio.h>
#include <string.h>

#define PI 3.1415927

int main(void) {
    double a, b, c;
    int sub, sum;
    int i;
    int m;

    scanf("%d", &m);
    for(i = 0; i < m; i++) {
        scanf("%lf%lf%lf", &a,&b,&c);
    if(a > 1000 || b > 1000 || c> 1000) {
        printf("NO\n");
        continue;
    }
    if(a == 0 || b == 0 || c == 0) {
        printf("NO\n");
        continue;
    }
        if(a+b <= c) {
            printf("NO\n");
            continue;
        }
        if(c+b <= a) {
            printf("NO\n");
            continue;
        }
        if(a+c <= b) {
            printf("NO\n");
            continue;
        }
        


        if(a -b > 0) {
            sub = a-b;
        }else {
            sub = b-a;
        }
        if(sub >= c) {
            printf("NO\n");
            continue;
        }


        if(c -b > 0) {
            sub = c-b;
        }else {
            sub = b-c;
        }
        if(sub >= a) {
            printf("NO\n");
            continue;
        }

        if(a - c > 0) {
            sub = a-c;
        }else {
            sub = c-a;
        }
        if(sub >= b) {
            printf("NO\n");
            continue;
        }

printf("YES\n");
    }
    return 0;
}