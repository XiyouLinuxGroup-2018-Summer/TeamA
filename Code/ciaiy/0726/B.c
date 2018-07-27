#include <stdio.h>

int main(void) {
    char str[100];
    
    int i;
    while(fgets(str, 100, stdin) != NULL) {
if(str[0]<= 'z' && str[0] >= 'a') {
        str[0] -= 'z' - 'Z';
    }

    for(i = 0; str[i] != '\0'; i++) {
        if(str[i] == ' ') {
                if(str[i+1]<= 'z' && str[i+1] >= 'a') {
        str[i+1] -= 'z' - 'Z';
    }
        }
    }

    printf("%s", str);
    
    }
    return 0;
}