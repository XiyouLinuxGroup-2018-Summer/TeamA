/*********************第一题原始代码*********************/
typedef struct tag_unimportant {
    char *name; // 使用指针, 而不是数组
    int num;
} A;
void func(A *a) // 并不能给a复制  应该为 void fun (A **a)
{
    a = (A *)malloc(sizeof(A)); // *a = (A *)malloc(sizeof(A));
    // 给a的成员 name 分配空间
    (*a)->name = (char *)malloc(sizeof(char) * 256);
    strcpy(a->name, "Xiyou Linux Group");
    a->num = 20180728;
}
int main(int argc, char *argv[])
{
    A *a;
    func(a); // func(&a);
    printf("%s %d\n", a->name, a->num);
    return 0;
}

/**********************第二题代码***********************/

int convert(char *num) {
    int len = strlen(num);
    int toInt = 0;

    for(int i = 0; i < len; i++) {
        toInt *= 10;
        num[i] = num[i] - '0';
        toInt += num[i];
    }

    return toInt;
}



/*********************第三题原始代码*********************/
struct strange { // 创建一个结构体
    char a, b, c, d, e, zero;
}; // 因为字节对齐  所以strange为8字节

int main(int argc, char *argv[])
{
    struct strange xp[3]; // 创建一个结构体数组
    int characters[6] = {'a' , 'b', 'c', 'd', 'e'}给一个int数组里面存放 "abcde" 的值

// 内存地址 高->低 
// characters[5] = 0 0 0 0
// characters[4] = 0 0 0 e
// characters[3] = 0 0 0 d
// characters[2] = 0 0 0 c
// characters[1] = 0 0 0 b
// characters[0] = 0 0 0 a


    for (int i = 0; i < 6; i++) {
        *((char *)(xp + 0) + i) = characters[i]; // 将xp[0][i] = characters[i];
    }

    // step 1 : a 0 0 0 0 0 0 0
    // step 2 : a b 0 0 0 0 0 0
    // step ... 
    // step 5 : a b c d e 0 0 0

    // 所以xp[1] = a b c d e 0 0 0

    strcpy((char *)&xp[1], (char *)characters); 
     // 将characters 看做一个存放32个字符的数组 
     // 然后将其值 赋给xp[1] 
     // 因为cahracters 只是 'a' 所以后面的值是随机的

    memcpy(&xp[2], characters, sizeof(struct strange)); 
    // xp[2] : abcde
    // 因为memcpy 先将低字节的数据存放到目标
    // 因此 xp2 : a 0 0 0 b 0 0 0 (即 characters[0] characters[1])

    printf("%zu\n", strlen((char *)characters)); // 1

    for (int i = 0; i < 3; i++) {
        printf("%c\n", xp[i].e); // e 任意值 b
    }
}